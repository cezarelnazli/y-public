#ifndef IDE_H
#define IDE_H

#define SECTOR_SIZE   512


enum ideact {
    IDECMD_RD,
    IDECMD_WR
};

struct idecmd {
    enum ideact act;
    uint32_t sect;
    uint8_t buf[SECTOR_SIZE];
};

void ide_init(void);
void ide_read(unsigned sect, const uint8_t *buf, uint32_t bsize);
void ide_write(unsigned sect, const uint8_t *buf, uint32_t bsize);
void ide_handle_irq(void);

#endif
