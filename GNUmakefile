# Nuke built-in rules.
.SUFFIXES:

# Target architecture to build for. Default to x86_64.
ARCH := x86_64

# This is the name that our final executable will have.
override OUTPUT := kernel
override IMAGE_NAME := HobbyOS-$(ARCH)

# Directory structure
BUILD_DIR := build
SRC_DIR   := src
INC_DIR   := $(SRC_DIR)/include

# User controllable toolchain
TARGET := x86_64-elf
CC := $(TARGET)-gcc
LD := $(TARGET)-ld

# Compilation flags
CFLAGS = -g -O2 -pipe \
         -ffreestanding \
         -nostdlib \
         -fno-stack-protector \
         -fno-stack-check \
         -fno-lto \
         -fno-PIE \
         -m64 \
         -march=x86-64 \
         -mno-80387 \
         -mno-mmx \
         -mno-sse \
         -mno-sse2 \
         -mno-red-zone \
         -mcmodel=kernel \
         -Isrc/include
LDFLAGS := -nostdlib -z max-page-size=0x1000 -T $(SRC_DIR)/linker/$(ARCH).lds

# 1. Automatic Source Discovery (Recursive)
# Finds all .c and .S files in src/kernel/ and its subdirectories
SRCS := $(shell find $(SRC_DIR)/kernel -name '*.c' -o -name '*.S')

# 2. Object Mapping
# Maps src/kernel/arch/x86_64/file.c -> build/src/kernel/arch/x86_64/file.c.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Default target
.PHONY: all
all: $(BUILD_DIR)/$(IMAGE_NAME).iso

# --- Kernel Linking ---
$(BUILD_DIR)/$(OUTPUT): $(OBJS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# --- Compilation Rules ---
# Rule for C files
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for Assembly (.S) files
$(BUILD_DIR)/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Image Creation (ISO) ---
$(BUILD_DIR)/$(IMAGE_NAME).iso: $(BUILD_DIR)/$(OUTPUT) limine.conf
	@mkdir -p $(BUILD_DIR)/iso_root/boot/limine
	cp $(BUILD_DIR)/$(OUTPUT) $(BUILD_DIR)/iso_root/boot/
	cp limine.conf $(BUILD_DIR)/iso_root/boot/limine/
	cp limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin $(BUILD_DIR)/iso_root/boot/limine/
	@mkdir -p $(BUILD_DIR)/iso_root/EFI/BOOT
	cp limine/BOOTX64.EFI $(BUILD_DIR)/iso_root/EFI/BOOT/
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(BUILD_DIR)/iso_root -o $@
	./limine/limine bios-install $@

# --- Utility Targets ---
.PHONY: run
run: $(BUILD_DIR)/$(IMAGE_NAME).iso
	qemu-system-$(ARCH) -M q35 -cdrom $< $(QEMUFLAGS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
