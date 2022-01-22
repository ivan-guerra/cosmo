#pragma once

/*!
 * \brief Send a byte to an I/O port.
 *
 * See PortIO.s for the details.
 *
 * \param port The 16-bit address of the I/O port.
 * \param data The data being sent to the port.
 */
extern "C" void outb(unsigned short port, unsigned char data);

/*!
 * \brief Read a byte from an I/O port.
 *
 * \param port The 16-bit address of the I/O port.
 * \returns The byte read from \a port.
 */
extern "C" unsigned char inb(unsigned short port);
