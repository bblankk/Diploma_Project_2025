#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED  2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS       4
#define MULTIBOOT_MEMORY_BADRAM    5

// Multiboot memory map entry (from GRUB)
typedef struct multiboot_Memory_Map_Entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed)) multibootMmapEntry_t;

// Multiboot information structure (GRUB passes pointer in EBX)
typedef struct multiboot_Info {
    uint32_t flags;
    uint32_t memLower;
    uint32_t memUpper;
    uint32_t bootDevice;
    uint32_t cmdline;
    uint32_t modsCount;
    uint32_t modsAddr;
    uint32_t syms[4];
    uint32_t mmapLength;
    uint32_t mmapAddr;
} __attribute__((packed)) multibootInfo_t;

#endif

