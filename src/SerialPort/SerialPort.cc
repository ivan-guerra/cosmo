#include "PortIO.h"
#include "SerialPort.h"

namespace cosmo
{
SerialPort::SerialPort() :
    port_(COMPort::kUndefinedPort),
    baud_(BaudRate::kUndefinedBaud),
    initialized_(false)
{

}

bool SerialPort::Init(COMPort com, BaudRate baud)
{
    /* Disable all interrupts. */
    outb(com + PortOffset::kInterruptEnableOffset, 0x00);

    /* Enable DLAB. */
    outb(com + PortOffset::kLineCommandOffset, 0x80);

    /* Set the baud divisor low byte followed by the high byte. */
    outb(com, baud & 0xFF);
    outb(com + PortOffset::kInterruptEnableOffset, (baud >> 8) & 0xFF);

    /* Configure the line to be 8N1. */
    outb(com + PortOffset::kLineCommandOffset, 0x03);

    /* Enable FIFO, clear buffer, and give them a size of 14 bytes. */
    outb(com + PortOffset::kFIFOPortOffset, 0xC7);

    /* Set RTS/DSR. */
    outb(com + PortOffset::kModemCommandOffset, 0x03);

    /* Set us in loopback mode to later test the serial chip. */
    outb(com + PortOffset::kModemCommandOffset, 0x1E);

    /* Test the serial chip (send byte 0xAE and check if you recv it back). */
    unsigned char test_byte = 0xAE;
    outb(com, test_byte);
    if (inb(com) != test_byte)
        return false;

    /* The serial is not faulty. Set it to operate in normal mode. */
    outb(com + PortOffset::kModemCommandOffset, 0x0F);

    /* We succeeded in setting up the serial chip. Save the serial settings. */
    port_        = com;
    baud_        = baud;
    initialized_ = true;

    return initialized_;
}

void SerialPort::PrintChar(unsigned char c) const
{
    /* Avoid trying to send to an uninitialized serial port. */
    if (!initialized_)
        return;

    /* Poll until the transmit buffer is empty. */
    while (!TransmitBufferEmpty());

    outb(port_, c);
}
} // end cosmo
