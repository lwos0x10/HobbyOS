#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include <limine.h>

void screen_fill(uint32_t color);
void screen_init(struct limine_framebuffer *fb);

uint32_t rgbto32(uint8_t red, uint8_t green, uint8_t blue);

#endif
