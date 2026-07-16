#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT Entry (Gate Descriptor) structure
struct idt_entry_struct {
    uint16_t base_lo;             // The lower 16 bits of the address to jump to
    uint16_t sel;                 // Kernel segment selector (usually 0x08)
    uint8_t  always0;             // This must always be zero
    uint8_t  flags;               // Flags (More flags info in osdev wiki)
    uint16_t base_hi;             // The upper 16 bits of the address to jump to
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// IDT Register (IDTR) structure
struct idt_ptr_struct {
    uint16_t limit;               // Size of the IDT table minus 1
    uint32_t base;                // The address of the first element in our idt_entry_t array
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// Set gate descriptor
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Initialize the IDT
void idt_init();

// C-level ISR handler
struct registers {
    uint32_t ds;                                      // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by pusha
    uint32_t int_no, err_code;                        // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;            // Pushed by the processor automatically
};
typedef struct registers registers_t;

void isr_handler(registers_t regs);

// Interrupt/IRQ handler registration
typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

// C-level IRQ handler
void irq_handler(registers_t regs);

#endif // IDT_H
