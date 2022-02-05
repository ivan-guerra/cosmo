#include "InterruptHandler.h"
#include "PortIO.h"
#include "ProgrammableInterruptController.h"
#include "Logger.h"
#include "FrameBuffer.h"
#include "IRQ/Keyboard/KeyboardIrq.h"

namespace cosmo
{
void interrupt::isr_handler(struct InterruptContext* int_context)
{
    cosmo::Logger logger;
    logger.LogError(cosmo::FrameBuffer::GetInstance(),
                    "Exception %X occurred.",
                    static_cast<unsigned int>(int_context->int_no));
}

void interrupt::irq_handler(struct InterruptContext* int_context)
{
    cosmo::Logger logger;
    switch(int_context->int_no) {
        case Irq::kKeyboard:
            /* Print the ASCII character that corresponds to the keypress. */
            irq::kbd::PrintAsciiChar(irq::kbd::ReadScanCode());
            break;
        default:
            logger.LogError(cosmo::FrameBuffer::GetInstance(),
                            "error, no handler for IRQ %d.",
                            static_cast<int>(int_context->int_no));
    }
}
} // end cosmo
