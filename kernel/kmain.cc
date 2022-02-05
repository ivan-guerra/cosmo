#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"
#include "InterruptHandler.h"
#include "ProgrammableInterruptController.h"

extern "C" int kernel_main(void)
{
    /* Setup the GDT. */
    cosmo::GlobalDescriptorTable<5> gdt;
    gdt.SetGate(0, 0, 0, 0, 0);                /* Null segment. */
    gdt.SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Code segment. */
    gdt.SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Data segment. */
    gdt.SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* User mode code segment. */
    gdt.SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* User mode data segment. */
    gdt.FlushGdt();

    cosmo::InterruptDescriptorTable idt;
    /* Register any additional interrupt vectors here with calls to
       idt.SetGate(...). */
    idt.FlushIdt();

    /* The IDT has been setup and registered. Enable interrupts. */
    __asm__ volatile("sti");

    /* Remap the master/slave PIC to have the correct vector offsets. */
    cosmo::pic::Init();

    for (uint8_t i = 0; i < 8; ++i) {
        /* Mask all IRQs. Note, we even mask IRQ 2 which corresponds to the
           slave PIC effectively masking all slave IRQs. */
        cosmo::pic::SetMask(i);
    }

    /* Clear the mask on the keyboard IRQ. */
    cosmo::pic::ClearMask(cosmo::interrupt::Irq::kKeyboard);

    /* Keep the kernel from exiting. */
    for(;;)
        __asm__ volatile("hlt");

    /* Should never make it here... */
    return 0;
}
