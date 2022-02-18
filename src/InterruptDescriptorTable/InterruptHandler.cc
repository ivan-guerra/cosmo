#include "InterruptHandler.h"
#include "ProgrammableInterruptController.h"
#include "IRQ/Keyboard/KeyboardIrq.h"
#include "Logger.h"

namespace cosmo
{
void interrupt::isr_handler(struct InterruptContext* int_context)
{
    LOG_ERROR("error, unhandled exception %X\n",
              static_cast<unsigned int>(int_context->int_no));

    /* Halt the machine on an unhandled CPU exception. */
    for (;;)
        __asm__ volatile("hlt");
}

void interrupt::irq_handler(struct InterruptContext* int_context)
{
    switch(int_context->int_no) {
        case Irq::kKeyboard:
            /* Print the ASCII character that corresponds to the keypress. */
            irq::kbd::PrintAsciiChar(irq::kbd::ReadScanCode());
            break;
        default:
            LOG_ERROR("error, unhandled IRQ %X\n",
                      static_cast<unsigned int>(int_context->int_no));
    }
}
} // end cosmo
