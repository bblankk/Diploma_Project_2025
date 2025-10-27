#include <stdint.h>
#include "terminal.h"
#include "multiboot.h"

const char* mem_Type_Name(uint32_t type) {
    switch (type) {
        case 1: return "Available";
        case 2: return "Reserved";
        case 3: return "ACPI Reclaimable";
        case 4: return "ACPI NVS";
        case 5: return "Bad Memory";
        default: return "Unknown";
    }
}

void print_Memory_Map(multibootInfo_t* mbInfo) {
    if (!(mbInfo->flags & (1 << 6))) {
        terminal_Write("No memory map provided by GRUB.\n");
        return;
    }

    terminal_Write("=== Physical Memory Map (from GRUB) ===\n");

    uint32_t mmapAddr   = mbInfo->mmapAddr;
    uint32_t mmapLength = mbInfo->mmapLength;

    for (uint32_t offset = 0; offset < mmapLength; ) {
        multibootMmapEntry_t* entry = (multibootMmapEntry_t*)(mmapAddr + offset);

        terminal_Write("Base: 0x");
        terminal_PrintHex64(entry->addr);
        terminal_Write(" | Length: 0x");
        terminal_PrintHex64(entry->len);
        terminal_Write(" | Type: ");
        terminal_Write(mem_Type_Name(entry->type));
        terminal_Write("\n");

        offset += entry->size + sizeof(entry->size);
    }

    terminal_Write("=======================================\n");
}

