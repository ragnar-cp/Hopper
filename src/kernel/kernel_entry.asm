[bits 32]
[global _start]
[extern kernel_main] ; This must match the main function in our C kernel

_start:
    call kernel_main ; Jump to the C kernel's main function
    jmp $            ; Infinite loop if the kernel ever returns
