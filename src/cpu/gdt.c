#include <cpu/gdt.h>
#include <lib/printf.h>

extern void load_gdt(struct gdt_ptr* ptr);
extern void load_tss(uint16_t sel);

struct gdt_entry gdt[7];
struct tss_entry tss_instance;

void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_mid    = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access      = access;
}

void write_tss_into_gdt(int num) {
    uint64_t base = (uintptr_t)&tss_instance;
    uint32_t limit = sizeof(tss_instance) - 1;

    // The low 8 bytes
    gdt[num].limit_low  = limit & 0xFFFF;
    gdt[num].base_low   = base & 0xFFFF;
    gdt[num].base_mid   = (base >> 16) & 0xFF;
    gdt[num].access     = 0x89; // Present, Executable, Accessed (TSS type)
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].base_high  = (base >> 24) & 0xFF;

    // The high 8 bytes
    uint64_t* upper_half = (uint64_t*)&gdt[num + 1];
    *upper_half = (base >> 32);
}

void gdt_init() {
    printf("GDT Initialize ...\n");
    // NULL
    gdt_set_entry(0, 0, 0, 0, 0);

    // Kernel Code(0x08): Access: 09xA (Present, Ring 0, Exec/Read)
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0x20);

    // Kernel Data(0x10): Access 0x92 (Present, Ring 0, Read/Write)
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0x00);
    // User Data (0x1B): Access 0xF2 (Present, Ring 3, Read/Write)
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xF2, 0x00);

    // User Code (0x23): Access 0xFA (Present, Ring 3, Exec/Read)
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xFA, 0x20);

    // TSS Descriptor
    for(int i = 0; i < (int)sizeof(tss_instance); i++) ((uint8_t*)&tss_instance)[i] = 0; 

    // Kernel Stack for TSS
    static uint8_t kernel_stack[16384];
    tss_instance.rsp0 = (uintptr_t)&kernel_stack[16384];

    write_tss_into_gdt(5);

    // Call Assembly
    struct gdt_ptr gdtr;

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base  = (uint64_t)&gdt;

    load_gdt(&gdtr);
    load_tss(0x28);
    printf("GDT: TSS loaded.\n");
}
