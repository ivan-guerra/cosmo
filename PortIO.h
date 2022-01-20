#pragma once

/*!
 * \brief Send a byte to an I/O port.
 *
 * See PortIO.s for the details.
 *
 * \port The 16-bit address of the I/O port.
 * \data The data being sent to the port.
 */
extern "C" void outb(unsigned short port, unsigned char data);
