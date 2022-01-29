#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"

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
    /* TODO: Register ISRs. */
    idt.FlushIdt();

    __asm__ volatile ("int $0x3");
    __asm__ volatile ("int $0x10");

    return 0;
}
