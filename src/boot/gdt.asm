; GDT (Global Descriptor Table) definition for flat 32-bit Protected Mode

gdt_start:

gdt_null: ; Mandatory null descriptor (8 bytes of 0)
    dd 0x0
    dd 0x0

gdt_code: ; Code segment descriptor
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10011010b ; 1st flags, type flags
    db 11001111b ; 2nd flags, Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

gdt_data: ; Data segment descriptor
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10010010b ; 1st flags, type flags
    db 11001111b ; 2nd flags, Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

gdt_end:

; GDT Descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of GDT (16-bit)
    dd gdt_start               ; Address of GDT (32-bit)

; Constants for segment descriptor offsets
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
