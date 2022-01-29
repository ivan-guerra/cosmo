#pragma once

#include <stdint.h>

#include "FlushIDT.h"

namespace cosmo
{
/*!
 * \class InterruptDescriptorTable
 * \brief The InterruptDescriptorTable class implements IDT setup.
 *
 * InterruptDescriptorTable provides a handle by which the User can setup the
 * interrupt descriptor table (IDT). InterruptDescriptorTable will always
 * setup the Intel required exception handlers (entries 0-31 in the table).
 * The User can use the InterruptDescriptorTable API to register additional
 * IRQs.
 */
class InterruptDescriptorTable
{
public:
    /*!
     * \struct IdtEntry
     *
     * This struct defines the contents of an IDT entry.
     */
    struct __attribute__((packed)) IdtEntry
    {
        uint16_t isr_low;    /*!< Lower 16 bits of the ISR's address. */
        uint16_t cs;         /*!< GDT kernel code segment selector. */
        uint8_t  reserved;   /*!< Zeroed field. */
        uint8_t  attributes; /*!< IDT entry type and attributes. */
        uint16_t isr_high;   /*!< Upper 16 bits of the ISR's address. */
    }; // end IdtEntry

    static const int kMaxIdtEntries = 256; /*!< Max number of entries allowed in the IDT. */

    /*!
     * \brief Construct an IDT with entries 0-31 pre-populated.
     *
     * InterruptDescriptorTable() registers the 32 Intel mandated CPU exception
     * handlers.
     */
    InterruptDescriptorTable();
    ~InterruptDescriptorTable() = default;

    /* Disable copy construction and copy assignment. */
    InterruptDescriptorTable(const InterruptDescriptorTable&) = delete;
    InterruptDescriptorTable& operator=(const InterruptDescriptorTable&) = delete;

    /* Disable move construction and move assignment. */
    InterruptDescriptorTable(InterruptDescriptorTable&&) = delete;
    InterruptDescriptorTable& operator=(InterruptDescriptorTable&&) = delete;

    /*!
     * \brief Return \c true if an interrupt vector has been initialized.
     */
    bool IsEnabled(uint8_t vector) const { return enabled_vectors_[vector]; }

    /*!
     * \brief Return the IdtEntry at IDT index \a i.
     */
    const IdtEntry& operator[](int i) const { return idt_entries_[i]; }
    IdtEntry& operator[](int i) { return idt_entries_[i]; }

    /*!
     * \brief Configure the gate (interrupt vector) specified by \a vector.
     *
     * \param vector Offset of the interrupt vector in the IDT.
     * \param isr Pointer to the address of the ISR.
     * \param selector 16-bit code segment selector.
     * \param flags Gate configuration flags.
     */
    void SetGate(uint8_t vector, uint32_t isr, uint16_t selector,
                 uint8_t flags);

    /*!
     * \brief Flush the current IDT to the idtr register.
     */
    void FlushIdt() { flush_idt(reinterpret_cast<uintptr_t>(&idtr_)); }

private:
    /*!
     * \struct IdtRegister
     *
     * This struct defines the base address and limit of the IDT.
     */
    struct __attribute__((packed)) IdtRegister
    {
        uint16_t limit; /*!< IDT table limit. */
        uint32_t base;  /*!< Address of base IDT entry. */
    }; // end IdtRegister

    struct IdtRegister idtr_;                            /*!< IDT register. */
    struct IdtEntry    idt_entries_[kMaxIdtEntries];     /*!< Table of IDT entries. */
    bool               enabled_vectors_[kMaxIdtEntries]; /*!< Track active vectors. */
}; // end InterruptDescriptorTable
} // end cosmo
