global loader                   ; The entry symbol for ELF.

MAGIC_NUMBER equ 0x1BADB002     ; Define the magic number constant.
FLAGS        equ 0x0            ; Multiboot flags.
CHECKSUM     equ -MAGIC_NUMBER  ; Calculate the checksum
                                ; (magic number + checksum + flags should
                                ; equal 0).
KERNEL_STACK_SIZE equ 4096      ; Size of stack in bytes.

section .bss
align 4                         ; Align at 4 bytes.
kernel_stack:                   ; Label points to beginning of memory.
    resb KERNEL_STACK_SIZE      ; Reserve stack for the kernel.

section .text                   ; Start of the text (code) section.
align 4                         ; The code must be 4 byte aligned.
    dd MAGIC_NUMBER             ; Write the magic number to the machine code.
    dd FLAGS                    ; Write flags.
    dd CHECKSUM                 ; Write the checksum.

loader:                         ; loader label (entry point in the ld script).
    mov esp, kernel_stack + KERNEL_STACK_SIZE ; point esp to the start of the
                                              ; kstack (end of memory area).
    extern main                 ; main defined in kmain.c.
    call main                   ; Call main (just puts 0xDEADBEEF in eax).
    .loop:
        jmp .loop               ; Loop forever.
