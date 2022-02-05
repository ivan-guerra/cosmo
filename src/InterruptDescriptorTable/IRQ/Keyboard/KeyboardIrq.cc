#include <stdint.h>

#include "IRQ/Keyboard/KeyboardIrq.h"
#include "InterruptHandler.h"
#include "PortIO.h"
#include "ProgrammableInterruptController.h"
#include "FrameBuffer.h"

namespace cosmo
{
namespace interrupt
{
namespace irq
{
uint8_t kbd::ReadScanCode()
{
    static const int kKbdDataPort = 0x60;

    /* Read in the KBD scan code. */
    uint8_t scan_code = inb(kKbdDataPort);

    /* Let the master PIC know we have finished servicing the interrupt. */
    cosmo::pic::SendEOI(cosmo::interrupt::Irq::kKeyboard);

    return scan_code;
}

void kbd::PrintAsciiChar(uint8_t scan_code)
{
    constexpr int kNumKeys = 128;
    if (scan_code >= kNumKeys)
        /* Avoid trying to index the key map with an invalid scan code. */
        return;

    static const char kUsKeyboardLayout[kNumKeys] =
    {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        '\b',
        '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
        '\n',
        0, /* CTRL */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
        '*',
        0,  /* Alt */
        ' ',  /* Space */
        0,  /* Caps Locks */
        0,  /* 59 - F1 key ... */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,  /* ... F10 */
        0,  /* Numlock */
        0,  /* Scroll Lock */
        0,  /* Home Key */
        0,  /* Up Arrow */
        0,  /* Page Up */
        '-',
        0,  /* Left Arrow */
        0,
        0,  /* Right Arrow */
        '+',
        0,  /* End Key */
        0,  /* Down Arrow */
        0,  /* Page Down */
        0,  /* Insert Key */
        0,  /* Delete Key */
        0,   0,   0,
        0,  /* F11 Key */
        0,  /* F12 Key */
        0  /* All other keys are undefined. */
    };

    cosmo::FrameBuffer::GetInstance().PrintChar(kUsKeyboardLayout[scan_code]);
}
} // end irq
} // end interrupt
} // end cosmo
