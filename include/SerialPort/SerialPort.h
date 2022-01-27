#pragma once

#include <stddef.h>
#include <stdint.h>

#include "PortIO.h"

namespace cosmo
{
/*!
 * \class SerialPort
 * \brief The SerialPort class provides a handle for writing to a COM port.
 *
 * The current implementation only allows writing to one of the four COM ports.
 * The User can select their COM port and baud rate during initialization.
 * SerialPort objects oprate in 8N1 mode and poll the transmit buffer before
 * sending data one byte at a time.
 */
class SerialPort
{
public:
    /*!
     * This enum aliases the four serial COM port base addresses.
     */
    enum COMPort
    {
        kUndefinedPort = -1,    /*!< Indicates an undefined COM port. */
        kCOM1          = 0x3F8, /*!< COM1 address. */
        kCOM2          = 0x2F8, /*!< COM2 address. */
        kCOM3          = 0x3E8, /*!< COM3 address. */
        kCOM4          = 0x2E8  /*!< COM4 address. */
    };

    /*!
     * This enum aliases all supported baud rates.
     */
    enum BaudRate
    {
        kUndefinedBaud = -1, /*!< Indicates an undefined baud rate. */
        kB1200         = 96, /*!< 1200 baud. */
        kB2400         = 48, /*!< 2400 baud. */
        kB4800         = 24, /*!< 4800 baud. */
        kB9600         = 12, /*!< 9600 baud. */
        kB19200        = 6,  /*!< 19200 baud. */
        kB38400        = 3,  /*!< 38400 baud. */
        kB57600        = 2,  /*!< 57600 baud. */
        kB115200       = 1   /*!< 115200 baud. */
    };

    /*!
     * \brief Default constructs a SerialPort.
     *
     * To initialize the SerialPort object to use a specific COM port and baud,
     * the User must call Init().
     */
    SerialPort();

    /*!
     * \brief Default destruct the SerialPort.
     */
    ~SerialPort() = default;

    /* Default copy construction and copy assignment are allowed. */
    SerialPort(const SerialPort&) = default;
    SerialPort& operator=(const SerialPort&) = default;

    /* Default move construction and move assignment are allowed. */
    SerialPort(SerialPort&&) = default;
    SerialPort& operator=(SerialPort&&) = default;

    /*!
     * \brief Initialize the SerialPort with a COM and baud.
     *
     * As part of the initialization process, the serial chip's operation is
     * verified using the builtin loopback facility.
     *
     * \param com A COM port address.
     * \param baud The port baud rate.
     *
     * \return \c true if the \a com is initialized with \a baud.
     */
    bool Init(COMPort com=COMPort::kCOM1, BaudRate baud=BaudRate::kB9600);

    /*!
     * \brief Get this SerialPort's COM.
     *
     * \return Return the COM of this SerialPort object. If the SerialPort has
     *         not been successfully initialized via a call to Init(),
     *         COMPort::kUndefinedPort is returned.
     */
    COMPort GetCOMPort() const { return port_; }

    /*!
     * \brief Get this SerialPort's baud rate.
     *
     * \return Return the baud rate of this SerialPort object. If the
     *         SerialPort has not been successfully initialized via a call to
     *         Init(), BaudRate::kUndefinedBaud is returned.
     */
    BaudRate GetBaudRate() const { return baud_; }

    /*!
     * \brief Return \c true if the port has been successfully initialized.
     */
    bool ActivePort() const { return initialized_; }

    /*!
     * \brief Print \a c to the serial port.
     *
     * If PrintChar is called on an uninitialized SerialPort object, PrintChar
     * immediately returns.
     */
    void PrintChar(char c) const;

    /*!
     * \brief Print the string \a str to the serial port.
     *
     * Passing a \a len value that extends beyond the end of \a str causes
     * undefined behavior.
     *
     * \param str C-style character string.
     * \param len Length of \a str.
     */
    template <typename T>
    void PrintString(T* str, size_t len) const;

private:
    /*!
     * This enum aliases the offset to different registers tied to the serial
     * port base address. See
     * <a href="https://wiki.osdev.org/Serial_ports#Port_Addresses">Port Addresses</a>
     * for details.
     */
    enum PortOffset
    {
        kInterruptEnableOffset = 1, /*!< Offset to the interrupt enable reg. */
        kFIFOPortOffset        = 2, /*!< Offset to FIFO config register. */
        kLineCommandOffset     = 3, /*!< Offset to line command register. */
        kModemCommandOffset    = 4, /*!< Offset to modem command register. */
        kLineStatusOffset      = 5  /*!< Offset to line status register. */
    };

    /*!
     * \brief Returns \c true if the transmit buffer is empty.
     *
     * If TransmitBufferEmpty is called on an uninitialized SerialPort object,
     * its behavior is undefined and the result mustn't be used.
     */
    bool TransmitBufferEmpty() const { return (inb(port_ + PortOffset::kLineStatusOffset) & 0x20); }

    COMPort  port_;        /*!< The COM port. */
    BaudRate baud_;        /*!< The baud rate. */
    bool     initialized_; /*!< Initialization marker. */
}; // end SerialPort

template <typename T>
void SerialPort::PrintString(T* str, size_t len) const
{
    /* NOOP if given an empty string or if the serial port is uninitialized. */
    if (!str || !initialized_)
        return;

    for (size_t i = 0; i < len; ++i)
        PrintChar(str[i]);
}
} // end cosmo
