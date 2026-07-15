# Hopper OS

**A hobby operating system built from scratch** — currently in early development.

Hopper is my personal journey into low-level systems programming, kernel development, and understanding how operating systems work under the hood.

![Hopper Boot Screen](https://github.com/user-attachments/assets/e5baa939-c6cb-456e-88c1-99fbfad11535)

---

## Current Status

**Day 1** — Basic Bootloader + 32-bit Protected Mode Kernel

- Boot sector successfully loads and displays "Hopper"
- Transitioned into 32-bit protected mode
- Basic kernel running with direct VGA text output

---

## Features Implemented

- **Bootloader** (NASM)
  - Prints OS name on boot
  - Loads kernel

- **Kernel** (C + Assembly)
  - Kernel entry point (`kernel_entry.asm`)
  - `kernel_main()` in C
  - Direct VGA text buffer access (`0xB8000`)
  - Screen clearing and text printing

- **Build System**
  - Cross-compilation setup
  - Linker script for memory layout

---

## Project Structure

```bash
Hopper/
├── boot/                  # Bootloader code
│   └── boot.nasm
├── kernel/                # Kernel source
│   ├── kernel_entry.asm
│   └── kernel.c
├── linker.ld              # Linker script
├── Makefile               # Build automation
├── iso/                   # For creating bootable ISO (future)
└── README.md
