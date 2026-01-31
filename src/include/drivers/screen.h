#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include <limine.h>

// --- PSF2 Font Structure ---
#define PSF2_MAGIC 0x864ab572

struct psf2_header {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t length;
    uint32_t charsize;
    uint32_t height;
    uint32_t width;
};

// --- Driver Functions ---
void screen_fill(uint8_t red, uint8_t green, uint8_t blue);
void screen_init(struct limine_framebuffer *fb);
void screen_setcolor(uint8_t red, uint8_t green, uint8_t blue);
void screen_putc(char c);
void screen_print(const char *str);

#endif
