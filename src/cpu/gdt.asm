[BITS 64]
global load_gdt
global load_tss

load_gdt:
    lgdt [rdi]

    ; Reload Data Segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Reload Code Segment
    push 0x08
    lea rax, [rel .reload_cs]
    push rax
    retfq

.reload_cs:
    ret

load_tss:
    mov ax, di
    ltr ax
    ret
