#pragma once

#include <stdint.h>

/*!
 * \brief Send a byte to an I/O port.
 *
 * See PortIO.s for the details.
 *
 * \param port The 16-bit address of the I/O port.
 * \param data The data being sent to the port.
 */
extern "C" void outb(uint16_t port, uint8_t data);

/*!
 * \brief Read a byte from an I/O port.
 *
 * \param port The 16-bit address of the I/O port.
 * \returns The byte read from \a port.
 */
extern "C" unsigned char inb(uint16_t port);

/*!
 * \brief Wait a very small amount of time (1 to 4 microseconds, generally).
 */
void io_wait();
