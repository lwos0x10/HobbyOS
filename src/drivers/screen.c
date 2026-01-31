#include <drivers/screen.h>

static struct limine_framebuffer *screen_fb;
static uint8_t *base_address;
static uint64_t pitch;
static uint64_t cursor_x;  // Renamed for clarity
static uint64_t cursor_y;
static uint64_t screen_h;  // Was screen_row (Height)
static uint64_t screen_w;  // Was screen_col (Width)

static uint64_t screen_offset(uint64_t x, uint64_t y) {
    return (y * pitch) + (x * 4);
}

static void draw_pixel(uint64_t x, uint64_t y, uint32_t color) {
    if (x >= screen_w || y >= screen_h) return;
    uint64_t offset = screen_offset(x, y);
    *(uint32_t*)(base_address + offset) = color;
}

uint32_t rgbto32(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t alpha = 0xFF;
    return  (uint32_t)(alpha << 24) |
            (uint32_t)(red << 16) |
            (uint32_t)(green << 8) |
            (uint32_t)(blue << 0);
}

void screen_fill(uint32_t color) {
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
}
