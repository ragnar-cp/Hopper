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
HopperOS/
├── Makefile                     # Automatically handles compiling, linking, and running
└── src/
    ├── boot/
    │   ├── boot.asm             # 16-bit bootloader (loads kernel & enters PM)
    │   └── gdt.asm              # GDT descriptor and segmentation setup
    └── kernel/
        ├── kernel_entry.asm     # Assembly entrypoint linking to C main
        ├── kernel.c             # The C kernel main logic (clears VGA screen & prints)
        └── linker.ld            # Linker script mapping sections to 0x1000 address

