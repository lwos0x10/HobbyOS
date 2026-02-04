#include "kernel.h"

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
    
    // Initialize Screen
    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
    screen_init(fb);
    screen_setcolor(255, 255, 255);

    gdt_init();

    hcf();
}
