#include <stdint.h>

#include "ProgrammableInterruptController.h"
#include "PortIO.h"

namespace cosmo
{
uint16_t pic::GetIrqReg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
       represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain. */
    outb(PicPort::kPic1Command, ocw3);
    outb(PicPort::kPic2Command, ocw3);
    return ((inb(PicPort::kPic2Command) << 8) | inb(PicPort::kPic1Command));
}

void pic::SendEOI(uint8_t irq)
{
    /* End-of-interrupt command code. */
    static const uint8_t kEoi = 0x20;
    if (irq >= 8)
        outb(PicPort::kPic2Command, kEoi);

    outb(PicPort::kPic1Command, kEoi);
}

void pic::Init(int offset1, int offset2)
{
    /* Save masks. */
    uint8_t a1 = inb(PicPort::kPic1Data);
    uint8_t a2 = inb(PicPort::kPic2Data);

    /* Start the initialization sequence. */
    outb(PicPort::kPic1Command, Icw::kIcw1Init | Icw::kIcw1Icw4);
    io_wait();
    outb(PicPort::kPic2Command, Icw::kIcw1Init | Icw::kIcw1Icw4);
    io_wait();

    /* ICW2: Master/slave PIC offset. */
    outb(PicPort::kPic1Data, offset1);
    io_wait();
    outb(PicPort::kPic2Data, offset2);
    io_wait();

    /* ICW3: Tell Master PIC there is a slave PIC at IRQ2. */
    outb(PicPort::kPic1Data, 4);
    io_wait();

    /* ICW3: Tell Slave PIC its cascade identity. */
    outb(PicPort::kPic2Data, 2);
    io_wait();

    /* Enable Master/Slave PIC for 80x86 mode. */
    outb(PicPort::kPic1Data, Icw::kIcw48086);
    io_wait();
    outb(PicPort::kPic2Data, Icw::kIcw48086);
    io_wait();

    /* Restore masks. */
    outb(PicPort::kPic1Data, a1);
    outb(PicPort::kPic2Data, a2);
}

void pic::SetMask(uint8_t irq_line)
{
    uint16_t port = 0;
    if (irq_line < 8) {
        port = PicPort::kPic1Data;
    } else {
        port = PicPort::kPic2Data;
        irq_line -= 8;
    }

    uint8_t value = inb(port) | (1 << irq_line);
    outb(port, value);
}

void pic::ClearMask(uint8_t irq_line)
{
    uint16_t port = 0;
    if (irq_line < 8) {
        port = PicPort::kPic1Data;
    } else {
        port = PicPort::kPic2Data;
        irq_line -= 8;
    }

    uint8_t value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

uint16_t pic::GetIrr()
{
    return GetIrqReg(PicPort::kPicReadIrr);
}

uint16_t pic::GetIsr()
{
    return GetIrqReg(PicPort::kPicReadIsr);
}
} // end cosmo
