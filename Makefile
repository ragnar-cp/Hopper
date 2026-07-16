# Makefile for HopperOS

CC = gcc
LD = ld
ASM = nasm

SRC_DIR = src
BUILD_DIR = build

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -c
LDFLAGS = -m elf_i386 -T $(SRC_DIR)/kernel/linker.ld --oformat binary
ASM_FLAGS = -f elf32

BOOT_SRC = $(SRC_DIR)/boot/boot.asm
GDT_SRC = $(SRC_DIR)/boot/gdt.asm
KERNEL_ENTRY_SRC = $(SRC_DIR)/kernel/kernel_entry.asm
KERNEL_C_SRC = $(SRC_DIR)/kernel/kernel.c
KERNEL_IDT_SRC = $(SRC_DIR)/kernel/idt.c
KERNEL_INTERRUPT_SRC = $(SRC_DIR)/kernel/interrupt.asm
KERNEL_TIMER_SRC = $(SRC_DIR)/kernel/timer.c
KERNEL_PAGING_SRC = $(SRC_DIR)/kernel/paging.c

BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_IDT_OBJ = $(BUILD_DIR)/idt.o
KERNEL_INTERRUPT_OBJ = $(BUILD_DIR)/interrupt.o
KERNEL_TIMER_OBJ = $(BUILD_DIR)/timer.o
KERNEL_PAGING_OBJ = $(BUILD_DIR)/paging.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/os-image.bin

all: $(OS_IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(BOOT_SRC) $(GDT_SRC) | $(BUILD_DIR)
	$(ASM) -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_SRC) | $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(KERNEL_ENTRY_SRC) -o $(KERNEL_ENTRY_OBJ)

$(KERNEL_OBJ): $(KERNEL_C_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(KERNEL_C_SRC) -o $(KERNEL_OBJ)

$(KERNEL_IDT_OBJ): $(KERNEL_IDT_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR)/kernel $(KERNEL_IDT_SRC) -o $(KERNEL_IDT_OBJ)

$(KERNEL_INTERRUPT_OBJ): $(KERNEL_INTERRUPT_SRC) | $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(KERNEL_INTERRUPT_SRC) -o $(KERNEL_INTERRUPT_OBJ)

$(KERNEL_TIMER_OBJ): $(KERNEL_TIMER_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR)/kernel $(KERNEL_TIMER_SRC) -o $(KERNEL_TIMER_OBJ)

$(KERNEL_PAGING_OBJ): $(KERNEL_PAGING_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR)/kernel $(KERNEL_PAGING_SRC) -o $(KERNEL_PAGING_OBJ)

$(KERNEL_BIN): $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(KERNEL_IDT_OBJ) $(KERNEL_INTERRUPT_OBJ) $(KERNEL_TIMER_OBJ) $(KERNEL_PAGING_OBJ)
	$(LD) -o $(KERNEL_BIN) $(LDFLAGS) $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(KERNEL_IDT_OBJ) $(KERNEL_INTERRUPT_OBJ) $(KERNEL_TIMER_OBJ) $(KERNEL_PAGING_OBJ)

$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	truncate -s +10K $(OS_IMAGE)

clean:
	rm -rf $(BUILD_DIR)

run: $(OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(OS_IMAGE)

.PHONY: all clean run
