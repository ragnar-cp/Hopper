#include "timer.h"

uint32_t tick = 0;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Timer interrupt callback
static void timer_callback(registers_t regs) {
    tick++;
    
    char *video_memory = (char *) 0xb8000;
    
    // Print label "Timer Ticks: " at row 4, col 0
    const char *label = "Timer Ticks: ";
    int offset = 80 * 4 * 2;
    
    int i = 0;
    while (label[i] != '\0') {
        video_memory[offset + i * 2] = label[i];
        video_memory[offset + i * 2 + 1] = 0x0E; // Light Yellow
        i++;
    }
    offset += i * 2;

    // Convert tick count to decimal string
    char buf[16];
    int temp = tick;
    int len = 0;
    if (temp == 0) {
        buf[0] = '0';
        len = 1;
    } else {
        while (temp > 0 && len < 15) {
            buf[len++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    // Reverse the string
    for (int j = 0; j < len / 2; j++) {
        char c = buf[j];
        buf[j] = buf[len - j - 1];
        buf[len - j - 1] = c;
    }
    buf[len] = '\0';

    // Print tick value
    i = 0;
    while (buf[i] != '\0') {
        video_memory[offset + i * 2] = buf[i];
        video_memory[offset + i * 2 + 1] = 0x0E; // Light Yellow
        i++;
    }
    
    // Clear any trailing digits from previous ticks
    video_memory[offset + i * 2] = ' ';
    video_memory[offset + i * 2 + 1] = 0x0E;
}

void init_timer(uint32_t frequency) {
    // Register timer_callback for IRQ 0 (Interrupt 32)
    register_interrupt_handler(32, timer_callback);

    // The PIT base frequency is 1193182 Hz
    uint32_t divisor = 1193182 / frequency;

    // Send command byte (0x36) to control port 0x43
    outb(0x43, 0x36);

    // Send divisor bytes to Channel 0 data port 0x40
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
    outb(0x40, l);
    outb(0x40, h);
}
