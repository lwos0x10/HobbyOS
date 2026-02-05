#include <lib/printf.h>
#include <cpu/idt.h>

typedef struct {
    uint64_t r11, r10, r9, r8, rdi, rsi, rdx, rcx, rax; 
    uint64_t interrupt, error_code;                    
    uint64_t rip, cs, rflags, rsp, ss;                 
} __attribute__((packed)) interrupt_frame_t;

__attribute__((aligned(0x10))) 
static struct idt_entry idt[256];

static struct idtr idtr_reg;

extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    uintptr_t addr = (uintptr_t)isr;

    idt[vector].isr_low    = addr & 0xFFFF;
    idt[vector].kernel_cs  = 0x08;
    idt[vector].ist        = 0;
    idt[vector].attributes = flags;
    idt[vector].isr_mid    = (addr >> 16) & 0xFFFF;
    idt[vector].isr_high   = (addr >> 32) & 0xFFFFFFFF;
    idt[vector].reserved   = 0;
}

void exception_handler(__attribute__((unused)) interrupt_frame_t* frame) {
    for(;;) {
        __asm__ ("hlt");
    }
}

void idt_init() {
    printf("IDT Initialize...\n");
    idtr_reg.base = (uintptr_t)&idt[0];
    idtr_reg.limit = (uint16_t)sizeof(idt) - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr_reg));
    printf("IDT LOADED.\n");
}
