ALIGNMENT equ (1<<0)                ; Align loaded modules on page boundaries.
MEMINFO   equ (1<<1)                ; Provide memory map.
FLAGS     equ (ALIGNMENT | MEMINFO) ; This is the Multiboot flag field.
MAGIC     equ 0x1BADB002            ; Magic number tells the bootloader where to find the header.
CHECKSUM  equ -(MAGIC + FLAGS)      ; Checksum of above, proves we are multiboot.

; Declare a Multiboot header that marks the program as a kernel.
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

; Configure the 16KiB kernel stack.
section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section .text
global _loader
_loader:
    mov esp, stack_top ; Setup the kernel stack.

    extern kernel_main ; See kernel_main.cc.
    call kernel_main   ; Enter the kernel.

    ; The kernel returned and the system has nothing else to do. Clear
    ; interrupts and halt the machine.
    cli
halt_kernel: hlt
    jmp halt_kernel
