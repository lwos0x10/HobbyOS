#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <arch.h>
#include <lib/string.h>
#include <drivers/screen.h>
#include <lib/printf.h>

// --- Limine Base Revision ---
__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

// --- Framebuffer Request ---
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// --- Request Markers ---
__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;



// --- Kernel Entry Point ---
void kernel_main(void) {
    enable_sse();

    // Validation Bootloader Revision
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    // Validation Framebuffer
    if (framebuffer_request.response == NULL || 
        framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
    screen_init(fb);
    screen_setcolor(255, 255, 255);
    char character = 'A';
    char string[] = "ABCD";
    int pos_number = 123456;
    int neg_number = -123456;
    double not_round = 1.0 / 3.0;
    uint64_t hex_number = 0x123456ABCDEF;
    printf("%c\n%s\n%d\n%d\n", character, string, pos_number, neg_number);
    printf("%f\n", not_round);
    printf("%x\n", hex_number);
    hcf();
}
