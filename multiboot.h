#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1

#define MULTIBOOT_INFO_MEM_MAP 0x00000040

typedef unsigned char multiboot_uint8_t;
typedef unsigned short multiboot_uint16_t;
typedef unsigned int multiboot_uint32_t;
typedef unsigned long long multiboot_uint64_t;

struct multiboot_aout_symbol_table
{
  multiboot_uint32_t tabsize;
  multiboot_uint32_t strsize;
  multiboot_uint32_t addr;
  multiboot_uint32_t reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

struct multiboot_elf_section_header_table
{
  multiboot_uint32_t num;
  multiboot_uint32_t size;
  multiboot_uint32_t addr;
  multiboot_uint32_t shndx;
};

struct multiboot_info
{
  multiboot_uint32_t flags;

  multiboot_uint32_t mem_lower;
  multiboot_uint32_t mem_upper;

  multiboot_uint32_t boot_device;

  multiboot_uint32_t cmdline;

  multiboot_uint32_t mods_count;
  multiboot_uint32_t mods_addr;

  union
  {
    multiboot_aout_symbol_table_t aout_sym;
    struct multiboot_elf_section_header_table elf_sec;
  } u;

  multiboot_uint32_t mmap_length;
  multiboot_uint32_t mmap_addr;

  multiboot_uint32_t drives_length;
  multiboot_uint32_t drives_addr;

  multiboot_uint32_t config_table;

  multiboot_uint32_t boot_loader_name;

  multiboot_uint32_t apm_table;

  multiboot_uint32_t vbe_control_info;
  multiboot_uint32_t vbe_mode_info;
  multiboot_uint16_t vbe_mode;
  multiboot_uint16_t vbe_interface_seg;
  multiboot_uint16_t vbe_interface_off;
  multiboot_uint16_t vbe_interface_len;

  multiboot_uint64_t framebuffer_addr;
  multiboot_uint32_t framebuffer_pitch;
  multiboot_uint32_t framebuffer_width;
  multiboot_uint32_t framebuffer_height;
  multiboot_uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
  multiboot_uint8_t framebuffer_type;
  union
  {
    struct
    {
      multiboot_uint32_t framebuffer_palette_addr;
      multiboot_uint16_t framebuffer_palette_num_colors;
    };
    struct
    {
      multiboot_uint8_t framebuffer_red_field_position;
      multiboot_uint8_t framebuffer_red_mask_size;
      multiboot_uint8_t framebuffer_green_field_position;
      multiboot_uint8_t framebuffer_green_mask_size;
      multiboot_uint8_t framebuffer_blue_field_position;
      multiboot_uint8_t framebuffer_blue_mask_size;
    };
  };
};
typedef struct multiboot_info multiboot_info_t;

struct multiboot_mmap_entry
{
  multiboot_uint32_t size;
  multiboot_uint32_t addr;
  multiboot_uint32_t addr0;
  multiboot_uint32_t len;
  multiboot_uint32_t len0;
#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED 2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS 4
#define MULTIBOOT_MEMORY_BADRAM 5
  multiboot_uint32_t type;
} GRUB_PACKED;
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

#endif
