#pragma once

#include <stdint.h>

/*!
 * \brief Tell the CPU where the IDT begins.
 *
 * \param ldt_address 32-bit pointer to the address of the first entry in the
 *                    IDT.
 */
extern "C" void flush_idt(uint32_t idt_address);
