#ifndef ARCH_H
#define ARCH_H

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

#endif
