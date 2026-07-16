#include "paging.h"

// Page Directory and the first Page Table (each aligned to 4KB boundary)
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

// Page Fault Handler
static void page_fault_handler(registers_t regs) {
    // The faulting address is stored in control register CR2
    uint32_t faulting_address;
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code tells us why the fault occurred
    int present = !(regs.err_code & 0x1); // 0 = page not present, 1 = protection violation
    int write = regs.err_code & 0x2;      // 0 = read, 1 = write
    int user = regs.err_code & 0x4;       // 0 = supervisor/kernel, 1 = user mode

    char *video_memory = (char *) 0xb8000;
    
    // Clear screen with a blue background (0x1F) for Page Fault panic screen
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = 0x1F;
    }

    const char *panic_msg = "!!! KERNEL PANIC: PAGE FAULT !!!";
    int i = 0;
    while (panic_msg[i] != '\0') {
        video_memory[i * 2] = panic_msg[i];
        video_memory[i * 2 + 1] = 0x1F;
        i++;
    }

    // Print faulting address in hex
    const char *addr_msg = "Faulting Virtual Address: 0x";
    int offset = 80 * 2 * 2;
    i = 0;
    while (addr_msg[i] != '\0') {
        video_memory[offset + i * 2] = addr_msg[i];
        video_memory[offset + i * 2 + 1] = 0x1F;
        i++;
    }
    offset += i * 2;

    // Convert address to hex characters
    char hex_chars[] = "0123456789ABCDEF";
    for (int j = 7; j >= 0; j--) {
        uint8_t nibble = (faulting_address >> (j * 4)) & 0xF;
        video_memory[offset] = hex_chars[nibble];
        video_memory[offset + 1] = 0x1F;
        offset += 2;
    }

    // Print details of the violation
    const char *reason_msg = "Reason: ";
    offset = 80 * 4 * 2;
    i = 0;
    while (reason_msg[i] != '\0') {
        video_memory[offset + i * 2] = reason_msg[i];
        video_memory[offset + i * 2 + 1] = 0x1F;
        i++;
    }
    offset += i * 2;

    const char *details = present ? "Page Not Present" : "Protection Violation";
    i = 0;
    while (details[i] != '\0') {
        video_memory[offset + i * 2] = details[i];
        video_memory[offset + i * 2 + 1] = 0x1F;
        i++;
    }
    offset += i * 2;

    const char *access = write ? " (Write Access)" : " (Read Access)";
    i = 0;
    while (access[i] != '\0') {
        video_memory[offset + i * 2] = access[i];
        video_memory[offset + i * 2 + 1] = 0x1F;
        i++;
    }
    offset += i * 2;

    const char *mode = user ? " in User Mode" : " in Supervisor Mode";
    i = 0;
    while (mode[i] != '\0') {
        video_memory[offset + i * 2] = mode[i];
        video_memory[offset + i * 2 + 1] = 0x1F;
        i++;
    }

    // Halt the CPU
    while (1) {
        __asm__ __volatile__("cli; hlt");
    }
}

void init_paging() {
    // Register the page fault handler (Interrupt 14)
    register_interrupt_handler(14, page_fault_handler);

    // Map the first 4 MB of memory to itself (Identity Mapping)
    // 1024 entries * 4 KB = 4 MB
    for (int i = 0; i < 1024; i++) {
        // Attribute 3: Present, Writable, Supervisor/Kernel
        first_page_table[i] = (i * 4096) | 3;
    }

    // Put the first page table into the page directory
    // Attribute 3: Present, Writable, Supervisor/Kernel
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    // Mark the other 1023 page tables as not present
    for (int i = 1; i < 1024; i++) {
        // Attribute 2: Writable, but not Present
        page_directory[i] = 0 | 2;
    }

    // Load the page directory address into CR3
    __asm__ __volatile__("mov %0, %%cr3" : : "r"(page_directory));

    // Enable the PG (Paging) bit (bit 31) in CR0
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Set PG bit
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));
}
