#pragma once

#include <stddef.h>
#include <stdint.h>

#include "FlushGDT.h"

namespace cosmo
{
/*!
 * \class GlobalDescriptorTable
 * \brief The GlobalDescriptorTable class implements GDT setup.
 *
 * GlobalDescriptorTable provides a handle by which the User can setup the
 * global descriptor table (GDT). This is a template class accepting one
 * template parameter, \a N, which specifies how many entries the GDT shall
 * hold. The meat of the GDT setup code was taken from
 * <a href="http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html">
 * James Malloy's Kernel Development Tutorial</a>.
 *
 * \tparam N The number of GDT entries that shall be allocated.
 */
template <size_t N>
class GlobalDescriptorTable
{
public:
    /*!
     * \struct GdtEntry
     *
     * This struct defines the contents of a GDT entry.
     * See Section 3.4.5 of the
     * <a href="https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.html/">
     * Intel Manual</a> for details.
     */
    struct __attribute__((packed)) GdtEntry
    {
        uint16_t limit_low;   /*! Lower 16-bits of the limit. */
        uint16_t base_low;    /*! The lower 16 bits of the base. */
        uint8_t  base_middle; /*! Next 8 bits of the base. */
        uint8_t  access;      /*! Access flags (mainly used to set DPL). */
        uint8_t  granularity; /*! Sets the scaling of the segment limit. */
        uint8_t  base_high;   /*! Last 8 bits of the base. */
    }; // end GdtEntry

    /*!
     * \brief Set the GDT base address and table limit.
     */
    GlobalDescriptorTable();
    ~GlobalDescriptorTable() = default;

    /* Disable copy construction and copy assignment. */
    GlobalDescriptorTable(const GlobalDescriptorTable&) = delete;
    GlobalDescriptorTable& operator=(const GlobalDescriptorTable&) = delete;

    /* Disable move construction and move assignment. */
    GlobalDescriptorTable(GlobalDescriptorTable&&) = delete;
    GlobalDescriptorTable& operator=(GlobalDescriptorTable&&) = delete;

    /*!
     * \brief Return the GdtEntry at GDT index \a i.
     */
    const GdtEntry& operator[](int i) const { return gdt_entries_[i]; }
    GdtEntry& operator[](int i) { return gdt_entries_[i]; }

    /*!
     * \brief Configure the segment descriptor.
     *
     * Indexing a segment descriptor beyond the bounds of the GDT will lead
     * to undefined behavior.
     *
     * \param i GDT entry offset.
     * \param base Base address of segment.
     * \param limit Segment limit.
     * \param access Segment access bytes (see Intel Manual for details).
     * \param granularity Segment granularity byte (see Intel Manual for
     *                    details).
     */
    void SetGate(int i, uint32_t base, uint32_t limit, uint8_t access,
                 uint8_t granularity);

    /*!
     * \brief Flush the current GDT to the appropriate CPU segment registers.
     */
    void FlushGdt() { flush_gdt(reinterpret_cast<uintptr_t>(&gdtr_)); }

    /*!
     * \brief Return the number of GDT entries in this GlobalDescriptorTable.
     */
    size_t NumEntries() const { return N; }

private:
    /*!
     * \struct GdtRegister
     *
     * This struct defines the base address and limit of the GDT.
     */
    struct __attribute__((packed)) GdtRegister
    {
        uint16_t limit; /*! Last valid address in the GDT. */
        uint32_t base;  /*! The address of the first descriptor. */
    }; // end GdtRegister

    struct GdtRegister gdtr_;           /*!< GDT register. */
    struct GdtEntry    gdt_entries_[N]; /*!< GDT entries. */
}; // end GlobalDescriptorTable

template <size_t N>
GlobalDescriptorTable<N>::GlobalDescriptorTable()
{
    gdtr_.limit = (sizeof(GdtEntry) * N) - 1;
    gdtr_.base  = reinterpret_cast<uintptr_t>(&gdt_entries_);
}

template <size_t N>
void GlobalDescriptorTable<N>::SetGate(int i, uint32_t base, uint32_t limit,
                                       uint8_t access, uint8_t granularity)
{
    gdt_entries_[i].base_low    = (base & 0xFFFF);
    gdt_entries_[i].base_middle = (base >> 16) & 0xFF;
    gdt_entries_[i].base_high   = (base >> 24) & 0xFF;

    gdt_entries_[i].limit_low   = (limit & 0xFFFF);
    gdt_entries_[i].granularity = (limit >> 16) & 0x0F;

    gdt_entries_[i].granularity |= granularity & 0xF0;
    gdt_entries_[i].access       = access;
}
} // end cosmo
