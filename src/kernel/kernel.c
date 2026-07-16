#include "idt.h"
#include "paging.h"
#include "timer.h"

void kernel_main() {
  // Pointer to the VGA text buffer (32-bit protected mode)
  char *video_memory = (char *)0xb8000;

  // A simple message to display
  const char *message = "Hopper Kernel is running in 32-bit Protected Mode!";

  // Clear the screen first with light grey text on a black background (0x07)
  for (int i = 0; i < 80 * 25; i++) {
    video_memory[i * 2] = ' ';
    video_memory[i * 2 + 1] = 0x07;
  }

  // Write the message in light green text on a black background (0x0A)
  int i = 0;
  while (message[i] != '\0') {
    video_memory[i * 2] = message[i];
    video_memory[i * 2 + 1] = 0x0A;
    i++;
  }

  // Initialize the IDT
  idt_init();

  // Initialize Paging
  init_paging();

  // Initialize PIT Timer at 50 Hz
  init_timer(50);

  // Enable CPU interrupts globally
  __asm__ __volatile__("sti");

  // Optional: Uncomment the following lines to trigger a page fault for
  // testing! (Accessing unmapped memory at virtual address 0xA0000000)
  // uint32_t *ptr = (uint32_t *)0xA0000000;
  // uint32_t do_fault = *ptr;
}
