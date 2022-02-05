; The code that follows defines generic interrupt handler code using macros
; that call a common handler function. There are two macros: one for interrupts
; with error codes and one for interrupts without error codes.
; See https://wiki.osdev.org/Interrupts_tutorial#ISRs for more details.

%macro ISR_NOERRCODE 1  ; Define a macro, taking one parameter.
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

; This isr_common_stub implementation was taken from
; https://gitorious.org/sortie/myos/?p=sortie:myos.git;a=blob;f=kernel/arch/i386/interrupt.c;h=11633cebf286733b345b3f8f3c594733f991ee38;hb=7012b0897328cd2bab57a9f7c971a238e424e14b
; Many examples and tutorials will have you save register context to the stack
; and then call the C/C++ handler function. This is bad because the compiler
; need not honor the state of the stack (i.e., during function execution, the
; compiler can change the state of the stack). A simple solution is to
; pass a pointer to the register context. It's okay if the compiler changes
; the pointer, the underlying registers will remain unchanged.
extern isr_handler
isr_common_stub:
    push eax
    push ecx
    push edx
    push ebx
    push ebp
    push esi
    push edi

    mov ebp, ds
    push ebp
    mov ebp, es
    push ebp
    mov ebp, fs
    push ebp
    mov ebp, gs
    push ebp

    mov ebp, 0x10
    mov ds, ebp
    mov es, ebp
    mov fs, ebp
    mov gs, ebp

    mov ebp, cr2
    push ebp

    mov ebx, esp
    sub esp, 4
    and esp, 0xFFFFFFF0 ; 16-byte align the stack.
    mov [esp], ebx

    call isr_handler ; Trigger the C++ interrupt handler.

    mov esp, ebx

    pop ebp
    mov cr2, ebp

    pop ebp
    mov gs, ebp
    pop ebp
    mov fs, ebp
    pop ebp
    mov es, ebp
    pop ebp
    mov ds, ebp

    pop edi
    pop esi
    pop ebp
    pop ebx
    pop edx
    pop ecx
    pop eax

    add esp, 8
    iret

; Use the above macros to define the 32 CPU exception handlers.
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_ERRCODE   30
ISR_NOERRCODE 31
