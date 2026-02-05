[BITS 64]
extern exception_handler
global isr_stub_table

; Macro for exceptions that don't push an error code (pushes a dummy 0)
%macro isr_no_err 1
global isr%1
isr%1:
    push qword 0      ; Dummy error code
    push qword %1     ; Interrupt number
    jmp isr_common
%endmacro

; Macro for exceptions that DO push an error code
%macro isr_err 1
global isr%1
isr%1:
    push qword %1     ; Interrupt number
    jmp isr_common
%endmacro

; Define the first 32 exceptions
isr_no_err 0
isr_no_err 1
isr_no_err 2
isr_no_err 3
isr_no_err 4
isr_no_err 5
isr_no_err 6
isr_no_err 7
isr_err    8
isr_no_err 9
isr_err    10
isr_err    11
isr_err    12
isr_err    13
isr_err    14
isr_no_err 15
isr_no_err 16
isr_err    17
isr_no_err 18
isr_no_err 19
isr_no_err 20
isr_no_err 21
isr_no_err 22
isr_no_err 23
isr_no_err 24
isr_no_err 25
isr_no_err 26
isr_no_err 27
isr_no_err 28
isr_no_err 29
isr_err    30
isr_no_err 31

isr_stub_table:
%assign i 0
%rep    32
    dq isr%+i
%assign i i+1
%endrep

isr_common:
    ; 1. Save all registers
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11

    ; 2. Call C dispatcher
    mov rdi, rsp    ; Pass pointer to stack frame
    call exception_handler

    ; 3. Restore and return
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax
    add rsp, 16     ; Clean up interrupt number and error code
    iretq           ; Return from interrupt
