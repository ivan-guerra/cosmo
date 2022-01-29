#include <stdint.h>

#include "InterruptHandler.h"
#include "InterruptDescriptorTable.h"

namespace cosmo
{
InterruptDescriptorTable::InterruptDescriptorTable()
{
    idtr_.base  = reinterpret_cast<uintptr_t>(&idt_entries_[0]);
    idtr_.limit = (sizeof(IdtEntry) * kMaxIdtEntries) - 1;

    /* Kernel code segment selector. */
    const uint16_t kKernelCs = 0x08;

    /* 8 -> indicates entry is present and has a DPL of 0.
       E -> Required value. */
    const uint8_t  kDefaultFlag = 0x8E;

    /* Register the 32 exception handlers. See ExceptionHandler.[h,cc,nasm]
       for details. */
    SetGate(0,  reinterpret_cast<uintptr_t>(isr0),  kKernelCs, kDefaultFlag);
    SetGate(1,  reinterpret_cast<uintptr_t>(isr1),  kKernelCs, kDefaultFlag);
    SetGate(2,  reinterpret_cast<uintptr_t>(isr2),  kKernelCs, kDefaultFlag);
    SetGate(3,  reinterpret_cast<uintptr_t>(isr3),  kKernelCs, kDefaultFlag);
    SetGate(4,  reinterpret_cast<uintptr_t>(isr4),  kKernelCs, kDefaultFlag);
    SetGate(5,  reinterpret_cast<uintptr_t>(isr5),  kKernelCs, kDefaultFlag);
    SetGate(6,  reinterpret_cast<uintptr_t>(isr6),  kKernelCs, kDefaultFlag);
    SetGate(7,  reinterpret_cast<uintptr_t>(isr7),  kKernelCs, kDefaultFlag);
    SetGate(8,  reinterpret_cast<uintptr_t>(isr8),  kKernelCs, kDefaultFlag);
    SetGate(9,  reinterpret_cast<uintptr_t>(isr9),  kKernelCs, kDefaultFlag);
    SetGate(10, reinterpret_cast<uintptr_t>(isr10), kKernelCs, kDefaultFlag);
    SetGate(11, reinterpret_cast<uintptr_t>(isr11), kKernelCs, kDefaultFlag);
    SetGate(12, reinterpret_cast<uintptr_t>(isr12), kKernelCs, kDefaultFlag);
    SetGate(13, reinterpret_cast<uintptr_t>(isr13), kKernelCs, kDefaultFlag);
    SetGate(14, reinterpret_cast<uintptr_t>(isr14), kKernelCs, kDefaultFlag);
    SetGate(15, reinterpret_cast<uintptr_t>(isr15), kKernelCs, kDefaultFlag);
    SetGate(16, reinterpret_cast<uintptr_t>(isr16), kKernelCs, kDefaultFlag);
    SetGate(17, reinterpret_cast<uintptr_t>(isr17), kKernelCs, kDefaultFlag);
    SetGate(18, reinterpret_cast<uintptr_t>(isr18), kKernelCs, kDefaultFlag);
    SetGate(19, reinterpret_cast<uintptr_t>(isr19), kKernelCs, kDefaultFlag);
    SetGate(20, reinterpret_cast<uintptr_t>(isr20), kKernelCs, kDefaultFlag);
    SetGate(21, reinterpret_cast<uintptr_t>(isr21), kKernelCs, kDefaultFlag);
    SetGate(22, reinterpret_cast<uintptr_t>(isr22), kKernelCs, kDefaultFlag);
    SetGate(23, reinterpret_cast<uintptr_t>(isr23), kKernelCs, kDefaultFlag);
    SetGate(24, reinterpret_cast<uintptr_t>(isr24), kKernelCs, kDefaultFlag);
    SetGate(25, reinterpret_cast<uintptr_t>(isr25), kKernelCs, kDefaultFlag);
    SetGate(26, reinterpret_cast<uintptr_t>(isr26), kKernelCs, kDefaultFlag);
    SetGate(27, reinterpret_cast<uintptr_t>(isr27), kKernelCs, kDefaultFlag);
    SetGate(28, reinterpret_cast<uintptr_t>(isr28), kKernelCs, kDefaultFlag);
    SetGate(29, reinterpret_cast<uintptr_t>(isr29), kKernelCs, kDefaultFlag);
    SetGate(30, reinterpret_cast<uintptr_t>(isr30), kKernelCs, kDefaultFlag);
    SetGate(31, reinterpret_cast<uintptr_t>(isr31), kKernelCs, kDefaultFlag);

    /* Mark remaining interrupt vectors as disabled/undefined. */
    for (int i = 32; i < kMaxIdtEntries; ++i)
        enabled_vectors_[i] = false;
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
