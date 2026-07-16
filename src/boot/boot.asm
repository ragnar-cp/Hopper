[org 0x7c00]
bits 16

; Memory offset where the kernel will be loaded
KERNEL_OFFSET equ 0x1000

start:
    ; Initialize segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    mov [BOOT_DRIVE], dl ; BIOS stores boot drive in DL

    ; Print loading message
    mov si, MSG_REAL_MODE
    call print_string

    ; Load the kernel from disk
    call load_kernel

    ; Switch to Protected Mode
    call switch_to_pm

    jmp $ ; Hang if it ever returns (should not)

; Include the GDT relative to compile path or current directory
%include "src/boot/gdt.asm"

bits 16
; Print a string using BIOS teletype
print_string:
    pusha
.loop:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0e
    int 0x10
    jmp .loop
.done:
    popa
    ret

; Load kernel sectors from disk
load_kernel:
    pusha
    mov si, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET ; Destination buffer address pointer (ES:BX)
    mov dh, 15            ; Number of sectors to read
    mov dl, [BOOT_DRIVE]  ; Boot drive
    mov ch, 0x00          ; Cylinder 0
    mov dh, 0x00          ; Head 0
    
    mov ah, 0x02
    mov al, 15
    mov ch, 0
    mov cl, 2
    mov dh, 0
    int 0x13
    jc disk_error

    popa
    ret

disk_error:
    mov si, MSG_DISK_ERROR
    call print_string
    jmp $

bits 16
switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT descriptor
    mov eax, cr0            ; Set PE (Protection Enable) bit in CR0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm    ; Far jump to 32-bit segment to flush pipeline

[bits 32]
; Initialize registers and stack in Protected Mode
init_pm:
    mov ax, DATA_SEG        ; Update segment registers to point to data descriptor
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Update stack pointer to top of free space
    mov esp, ebp

    call KERNEL_OFFSET      ; Jump to kernel entry point
    jmp $                   ; Hang if kernel returns

; Global Variables
BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Started in 16-bit Real Mode.", 13, 10, 0
MSG_LOAD_KERNEL db "Loading kernel from disk...", 13, 10, 0
MSG_DISK_ERROR  db "Error loading kernel from disk!", 13, 10, 0

; Pad boot sector to 510 bytes and add boot signature
times 510-($-$$) db 0
dw 0xaa55
