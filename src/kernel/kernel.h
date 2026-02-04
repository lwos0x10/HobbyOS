#ifndef KERNEL_H
#define KERNEL_H

// Assembly Commands
#include <limine.h>
#include <arch.h>

// Cross Complier Library
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Drivers
#include <drivers/screen.h>

// Hobby OS Library
#include <lib/string.h>
#include <lib/printf.h>

/*Initialize Operating System*/

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

#endif
