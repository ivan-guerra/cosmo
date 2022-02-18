/* This linker script was directly taken from
   https://wiki.osdev.org/Higher_Half_x86_Bare_Bones#linker.ld */
ENTRY(_loader)
OUTPUT_FORMAT(elf32-i386)

SECTIONS {
   /* The kernel will live at 3GB + 1MB in the virtual
      address space, which will be mapped to 1MB in the
      physical address space. */
   . = 0xC0100000;

   _kernel_virtual_start  = .;
   _kernel_physical_start = . - 0xC0000000;

   .text ALIGN (4K) : AT(ADDR(.text) - 0xC0000000) {
       *(.text)
       *(.rodata*)
   }

   .data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
       *(.data)
   }

   .bss ALIGN (4K) : AT(ADDR(.bss) - 0xC0000000) {
       _sbss = .;
       *(COMMON)
       *(.bss)
       _ebss = .;
   }

   _kernel_virtual_end  = .;
   _kernel_physical_end = . - 0xC0000000;
}
