#ifndef ARCH_H
#define ARCH_H

#include <stdint.h>

// Halt and Catch Fire
static inline void hcf(void) {
    for (;;) {
        #if defined(__x86_64__)
            __asm__ __volatile__ ("hlt");
        #elif defined(__riscv) || defined(__aarch64__)
            __asm volatile ("wfi");
        #endif
    }
}

static inline void enable_sse() {
    uint64_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 &= ~(1 << 2);
    cr0 |= (1 << 1);
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));

    uint64_t cr4;
    __asm__ volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 |= (1 << 9);
    cr4 |= (1 << 10);
    __asm__ volatile("mov %0, %%cr4" :: "r"(cr4));
}

#endif
