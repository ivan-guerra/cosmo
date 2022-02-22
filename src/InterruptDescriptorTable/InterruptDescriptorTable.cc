#include <stdint.h>
#include <string.h>

#include "InterruptHandler.h"
#include "InterruptDescriptorTable.h"

namespace cosmo
{
InterruptDescriptorTable::InterruptDescriptorTable()
{
    idtr_.base  = reinterpret_cast<uintptr_t>(&idt_entries_[0]);
    idtr_.limit = (sizeof(IdtEntry) * kMaxIdtEntries) - 1;

    /* Zero out the IDT entry fields. */
    memset(idt_entries_, 0, sizeof(IdtEntry) * kMaxIdtEntries);

    /* Register 32 ISR handlers. See InterruptHandler.[h,cc,nasm]
       for details. */
    SetGate(0,  reinterpret_cast<uintptr_t>(interrupt::isr0));
    SetGate(1,  reinterpret_cast<uintptr_t>(interrupt::isr1));
    SetGate(2,  reinterpret_cast<uintptr_t>(interrupt::isr2));
    SetGate(3,  reinterpret_cast<uintptr_t>(interrupt::isr3));
    SetGate(4,  reinterpret_cast<uintptr_t>(interrupt::isr4));
    SetGate(5,  reinterpret_cast<uintptr_t>(interrupt::isr5));
    SetGate(6,  reinterpret_cast<uintptr_t>(interrupt::isr6));
    SetGate(7,  reinterpret_cast<uintptr_t>(interrupt::isr7));
    SetGate(8,  reinterpret_cast<uintptr_t>(interrupt::isr8));
    SetGate(9,  reinterpret_cast<uintptr_t>(interrupt::isr9));
    SetGate(10, reinterpret_cast<uintptr_t>(interrupt::isr10));
    SetGate(11, reinterpret_cast<uintptr_t>(interrupt::isr11));
    SetGate(12, reinterpret_cast<uintptr_t>(interrupt::isr12));
    SetGate(13, reinterpret_cast<uintptr_t>(interrupt::isr13));
    SetGate(14, reinterpret_cast<uintptr_t>(interrupt::isr14));
    SetGate(15, reinterpret_cast<uintptr_t>(interrupt::isr15));
    SetGate(16, reinterpret_cast<uintptr_t>(interrupt::isr16));
    SetGate(17, reinterpret_cast<uintptr_t>(interrupt::isr17));
    SetGate(18, reinterpret_cast<uintptr_t>(interrupt::isr18));
    SetGate(19, reinterpret_cast<uintptr_t>(interrupt::isr19));
    SetGate(20, reinterpret_cast<uintptr_t>(interrupt::isr20));
    SetGate(21, reinterpret_cast<uintptr_t>(interrupt::isr21));
    SetGate(22, reinterpret_cast<uintptr_t>(interrupt::isr22));
    SetGate(23, reinterpret_cast<uintptr_t>(interrupt::isr23));
    SetGate(24, reinterpret_cast<uintptr_t>(interrupt::isr24));
    SetGate(25, reinterpret_cast<uintptr_t>(interrupt::isr25));
    SetGate(26, reinterpret_cast<uintptr_t>(interrupt::isr26));
    SetGate(27, reinterpret_cast<uintptr_t>(interrupt::isr27));
    SetGate(28, reinterpret_cast<uintptr_t>(interrupt::isr28));
    SetGate(29, reinterpret_cast<uintptr_t>(interrupt::isr29));
    SetGate(30, reinterpret_cast<uintptr_t>(interrupt::isr30));
    SetGate(31, reinterpret_cast<uintptr_t>(interrupt::isr31));

    /* Register 16 IRQ handlers. See InterruptHandler.[h,cc,nasm]
       for details. */
    SetGate(32, reinterpret_cast<uintptr_t>(interrupt::irq0));
    SetGate(33, reinterpret_cast<uintptr_t>(interrupt::irq1));
    SetGate(34, reinterpret_cast<uintptr_t>(interrupt::irq2));
    SetGate(35, reinterpret_cast<uintptr_t>(interrupt::irq3));
    SetGate(36, reinterpret_cast<uintptr_t>(interrupt::irq4));
    SetGate(37, reinterpret_cast<uintptr_t>(interrupt::irq5));
    SetGate(38, reinterpret_cast<uintptr_t>(interrupt::irq6));
    SetGate(39, reinterpret_cast<uintptr_t>(interrupt::irq7));
    SetGate(40, reinterpret_cast<uintptr_t>(interrupt::irq8));
    SetGate(41, reinterpret_cast<uintptr_t>(interrupt::irq9));
    SetGate(42, reinterpret_cast<uintptr_t>(interrupt::irq10));
    SetGate(43, reinterpret_cast<uintptr_t>(interrupt::irq11));
    SetGate(44, reinterpret_cast<uintptr_t>(interrupt::irq12));
    SetGate(45, reinterpret_cast<uintptr_t>(interrupt::irq13));
    SetGate(46, reinterpret_cast<uintptr_t>(interrupt::irq14));
    SetGate(47, reinterpret_cast<uintptr_t>(interrupt::irq15));

    /* Mark remaining interrupt vectors as disabled/undefined. */
    for (int i = 48; i < kMaxIdtEntries; ++i)
        enabled_vectors_[i] = false;
}

InterruptDescriptorTable& InterruptDescriptorTable::GetInstance()
{
    static InterruptDescriptorTable idt;
    return idt;
}

void InterruptDescriptorTable::SetGate(uint8_t vector, uint32_t isr,
                                       uint16_t selector, uint8_t flags)
{
    idt_entries_[vector].isr_low    = isr & 0xFFFF;
    idt_entries_[vector].cs         = selector;
    idt_entries_[vector].attributes = flags;
    idt_entries_[vector].isr_high   = isr >> 16;
    idt_entries_[vector].reserved   = 0;

    enabled_vectors_[vector] = true;
}
} // end cosmo
