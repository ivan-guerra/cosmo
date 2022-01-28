#pragma once

#include <stdint.h>

/*!
 * \brief Flush the current GDT and set its new address to \a gdt_address.
 *
 * \param gdt_address 32-bit pointer to the address of the new GDT.
 */
extern "C" void flush_gdt(uint32_t gdt_address);
