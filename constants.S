%ifndef CONSTANTS_S
%define CONSTANTS_S

KHALF equ 0xC0000000
KVA   equ KHALF + 0x100000

%define V2P(x) ((x) - KHALF)
%define P2V(x) ((x) + KHALF)

MBOOT_MAGIC equ 0x1BADB002
MBOOT_FLAGS equ 0x00000003
MBOOT_CKSUM equ 0 - (MBOOT_MAGIC + MBOOT_FLAGS)

CR4_PSE equ 0x10
PTE_PW  equ 0x03            ; page table entry is Present and Writeable
PTE_PSE equ 0x80            ; page table entry refers to a 4 MiB page

CR0_PAGE equ 0x80000000
CR0_WP equ 0x010000         ; read/write applies to both user and kernel

SEG_UCODE equ 3
SEG_UDATA equ 4
DPL_USER equ 3

%endif
