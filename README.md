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

---

## Current Status

### Day 2 — Interrupt Handling, Hardware Timer & 32-bit Paging

HopperOS now supports CPU exception handling, hardware interrupts, the Programmable Interval Timer (PIT), and 32-bit paging with page fault detection.

---

## Features Implemented

### Interrupt Descriptor Table (IDT)

- Created a complete 32-bit Interrupt Descriptor Table
- Configured exception vectors (0–31)
- Loaded the IDT using the `lidt` instruction
- Added C-based exception handling
- Displays a kernel panic screen for CPU exceptions
- Halts the CPU after unrecoverable exceptions

### Interrupt Service Routines (ISR)

- Implemented assembly ISR wrappers (`isr0`–`isr31`)
- Properly saves and restores CPU registers
- Handles exceptions with and without CPU error codes
- Routes exceptions to a common C handler
- Returns safely using `iret`

### Programmable Interrupt Controller (PIC)

- Remapped IRQs from legacy vectors to **32–47**
- Eliminated conflicts between hardware interrupts and CPU exceptions
- Added End Of Interrupt (EOI) handling
- Added support for custom interrupt handler registration

### Hardware Interrupts (IRQ)

- Implemented IRQ assembly wrappers (`irq0`–`irq15`)
- Added common IRQ dispatcher
- Connected hardware interrupts to C handlers
- Enabled hardware interrupts using the `sti` instruction

### Programmable Interval Timer (PIT)

- Configured PIT Channel 0
- Added configurable timer frequency
- Initialized timer at **50 Hz**
- Implemented timer tick counter
- Displays live timer ticks directly through VGA memory

### 32-bit Paging

- Implemented page directory and page tables
- Enabled paging using CR3 and CR0 registers
- Identity mapped the first **4 MB** of memory
- Added page protection using non-present pages
- Implemented a custom page fault handler
- Displays detailed paging panic information including:
  - Faulting virtual address (`CR2`)
  - Error code
  - Read/Write information
  - Protection vs Non-present page faults

### Build System

- Added compilation targets for:
  - `idt.c`
  - `interrupt.asm`
  - `timer.c`
  - `paging.c`
- Automatically links all new kernel components into the final OS image

---



### Preview

Day 2 introduces full interrupt handling, hardware timer support, and virtual memory paging.

<img width="762" height="485" alt="Day 2 Preview" src="https://github.com/user-attachments/assets/d6479b8f-7a59-470b-bb65-4da1b1d8fa81" />

---

## Progress So Far

- ✅ Bootloader
- ✅ Global Descriptor Table (GDT)
- ✅ 32-bit Protected Mode
- ✅ C Kernel
- ✅ VGA Text Driver
- ✅ Interrupt Descriptor Table (IDT)
- ✅ CPU Exception Handling
- ✅ Interrupt Service Routines (ISR)
- ✅ Programmable Interrupt Controller (PIC)
- ✅ Hardware Interrupts (IRQ)
- ✅ Programmable Interval Timer (PIT)
- ✅ Timer Tick Counter
- ✅ 32-bit Paging
- ✅ Page Fault Handler

---

## Project Structure

```bash
HopperOS/
├── Makefile                 # Build config for GCC/nasm (compile, link, make run in QEMU)
├── build/                   # Output folder for compiled objects (.o) and binaries (.bin)
│   ├── boot.bin             # Compiled 16-bit bootloader binary
│   ├── idt.o                # Compiled object for IDT setup
│   ├── interrupt.o          # Compiled object for low-level assembly interrupt stubs
│   ├── kernel.bin           # Linked kernel executable binary
│   ├── kernel.o             # Compiled object for kernel main logic
│   ├── kernel_entry.o       # Compiled object for kernel entry point bootstrap
│   ├── os-image.bin         # Final bootable disk image (boot.bin + kernel.bin)
│   ├── paging.o             # Compiled object for paging setup
│   └── timer.o              # Compiled object for the PIT timer driver
└── src/                     # Directory containing all source code
    ├── boot/                # Bootloader stage code
    │   ├── boot.asm         # Main assembly bootloader (sets GDT, enters PM, loads kernel)
    │   └── gdt.asm          # Global Descriptor Table defining 32-bit flat memory segments
    └── kernel/              # Kernel stage code (runs in 32-bit Protected Mode)
        ├── idt.h            # IDT structure declarations and registers
        ├── idt.c            # IDT gates configuration & PIC interrupt remapping
        ├── interrupt.asm    # Assembly ISR/IRQ wrappers (handles stack and executes iret)
        ├── kernel.c         # Main C kernel entry (coordinates IDT, paging, and timer setup)
        ├── kernel_entry.asm # Tiny assembly wrapper that jumps to kernel_main()
        ├── linker.ld        # Linker script setting memory layout and start address (0x1000)
        ├── paging.h         # Paging initialization header
        ├── paging.c         # 32-bit Page Directory setup, identity mapping & Page Fault handler
        ├── timer.h          # Timer driver header
        └── timer.c          # PIT driver that configures timer ports & increments system ticks


