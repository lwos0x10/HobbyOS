#include <drivers/screen.h>

extern uint8_t _binary_src_drivers_font_psf_start[];

static struct limine_framebuffer *screen_fb;
static uint8_t *base_address;
static uint64_t pitch;
static uint64_t cursor_x;
static uint64_t cursor_y;
static uint64_t screen_h;
static uint64_t screen_w;

static uint64_t current_color = 0xFFFFFFFF;

static uint64_t screen_offset(uint64_t x, uint64_t y) {
    return (y * pitch / 4) + x;
}

static void draw_pixel(uint64_t x, uint64_t y, uint32_t color) {
    if (x >= screen_w || y >= screen_h) return;
    uint32_t *screen = (uint32_t *)base_address;
    uint64_t offset = screen_offset(x, y);
    screen[offset] = color;
}

static void draw_char(char c, uint64_t x, uint64_t y, uint32_t color) {
    struct psf2_header *font = (struct psf2_header *)_binary_src_drivers_font_psf_start;
    
    uint8_t *glyph = _binary_src_drivers_font_psf_start + 
                     font->header_size + 
                     (uint32_t)c * font->charsize;

    uint32_t bytes_per_line = (font->width + 7) / 8;

    for (uint32_t cy = 0; cy < font->height; cy++) {
        for (uint32_t cx = 0; cx < font->width; cx++) {
            uint8_t bit = glyph[cy * bytes_per_line + (cx / 8)] & (0x80 >> (cx % 8));
            if (bit) {
                draw_pixel(x + cx, y + cy, color);
            }
        }
    }
}

static uint32_t rgbto32(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t alpha = 0xFF;
    return  (uint32_t)(alpha << 24) |
            (uint32_t)(red << 16) |
            (uint32_t)(green << 8) |
            (uint32_t)(blue << 0);
}

void screen_setcolor(uint8_t red, uint8_t green, uint8_t blue){
    current_color = rgbto32(red, green, blue);
}

void screen_fill(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = rgbto32(red, green, blue);
    for (uint64_t y = 0; y < screen_h; y++) {
        for (uint64_t x = 0; x < screen_w; x++) {
            draw_pixel(x, y, color);
        }
    }
} 

void screen_init(struct limine_framebuffer *fb) {
   screen_fb = fb;
   pitch = fb->pitch;
   base_address = (uint8_t*)fb->address;
   screen_h = fb->height;
   screen_w = fb->width;
   
   cursor_x = 0;
   cursor_y = 0;

   screen_fill(0, 0, 0);
}

void screen_putc(char c) {
    struct psf2_header *font = (struct psf2_header *)_binary_src_drivers_font_psf_start;
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y += font->height;
        return;
    }

    if ((uint8_t)c < 32 || (uint32_t)c >= font->length) return;

    draw_char(c, cursor_x, cursor_y, current_color);

    cursor_x += font->width;

    if (cursor_x + font->width > screen_w) {
        cursor_x = 0;
        cursor_y += font->height;
    }
}

void screen_print(const char *str) {
   while(*str != '\0'){
        screen_putc(*str);
        str++;
   } 
}
