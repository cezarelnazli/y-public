#include <stdint.h>
#include "ide.h"
#include "vm.h"
#include "x86.h"
#include "pm.h"
#include "string.h"
#include "kprintf.h"

#define IDE_BUSY      0x80
#define IDE_DRDY      0x40

#define IDE_CMD_READ  0x20
#define IDE_CMD_WRITE 0x30

#define NIDECMD 0x10


static struct idecmd *cmd_queue[NIDECMD];
static uint8_t cmdq_head, cmdq_tail;
static int16_t cmdq_tail_prev = -1;


static void ide_wait(void)
{
    while ((inb(0x1F7) & (IDE_BUSY | IDE_DRDY)) != IDE_DRDY)
        ;
}


void ide_init(void)
{
    ide_wait();
}


static void ide_rdrw(void)
{
    struct idecmd *cmd = cmd_queue[cmdq_tail++];
    int i;

    cmdq_tail_prev = cmdq_tail - 1;
    if (cmdq_tail == NIDECMD) {
        cmdq_tail = 0;
    }

    ide_wait();
    /*outb(0x3F6, 0);*/

    outb(0x1F6, 0xE0 | ((cmd->sect >> 24) & 0x0F));
    outb(0x1F2, 1);
    outb(0x1F3, cmd->sect & 0xff);
    outb(0x1F4, (cmd->sect >> 8) & 0xFF);
    outb(0x1F5, (cmd->sect >> 16) & 0xFF);

    if(cmd->act == IDECMD_WR) {
        outb(0x1F7, IDE_CMD_WRITE);
        outsb(0x1F0, cmd->buf, SECTOR_SIZE);
    } else {
        outb(0x1F7, IDE_CMD_READ);
    }
}


void ide_read(unsigned sect, const uint8_t *buf, uint32_t bsize)
{
    struct idecmd *cmd = (struct idecmd *) P2KH(pm_alloc_frame());

    cmd->act = IDECMD_RD;
    cmd->sect = sect;
    memset(cmd->buf, 0x00, SECTOR_SIZE);
    memcpy(cmd->buf, buf, bsize);

    cmd_queue[cmdq_head++] = cmd;
    if (cmdq_head == NIDECMD) {
        cmdq_head = 0;
    }

    ide_rdrw();
}


void ide_write(unsigned sect, const uint8_t *buf, uint32_t bsize)
{
    struct idecmd *cmd = (struct idecmd *) P2KH(pm_alloc_frame());

    cmd->act = IDECMD_WR;
    cmd->sect = sect;
    memset(cmd->buf, 0x00, SECTOR_SIZE);
    memcpy(cmd->buf, buf, bsize);

    cmd_queue[cmdq_head++] = cmd;
    if (cmdq_head == NIDECMD) {
        cmdq_head = 0;
    }

    ide_rdrw();
}

void ide_handle_irq(void)
{
    kprintf(OUTDEV_COM, "IDE IRQ\n");

    if (cmdq_tail_prev == -1) {
        kprintf(OUTDEV_COM, "no previous cmd");
        return;
    }

    if (cmd_queue[cmdq_tail_prev]->act == IDECMD_RD) {
        insb(0x1F0, cmd_queue[cmdq_tail_prev]->buf, SECTOR_SIZE);
    }

    ide_rdrw();
}

