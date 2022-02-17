#include <stdint.h>

#include "Logger.h"
#include "FrameBuffer.h"
#include "multiboot.h"
#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"
#include "InterruptHandler.h"
#include "ProgrammableInterruptController.h"

void Halt()
{
    for (;;)
        __asm__ volatile("hlt");
}

extern "C" int kernel_main(uint32_t mboot_magic, uint32_t mboot_header)
{
    cosmo::Logger log;

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        log.LogError(cosmo::FrameBuffer::GetInstance(),
            "error: not booted by a Multiboot compliant bootloader!\n");
        Halt();
    }

    /* Cast the pointer to a multiboot_info_t struct pointer. */
    multiboot_info_t* mboot_hdr =
        reinterpret_cast<multiboot_info_t*>(mboot_header);

    /* The specification states that bit 6 signifies the presence of the memory
       map. We can check the header flags to see if it's there */
    if ((mboot_hdr->flags & (1<<6)) == 0) {
        /* The memory map is not present, we should probably halt the system. */
        log.LogError(cosmo::FrameBuffer::GetInstance(),
            "error: no Multiboot memory map was provided!\n");
        Halt();
    }

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
    Halt();

    /* Should never make it here... */
    return 0;
}
