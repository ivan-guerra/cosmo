#include "InterruptHandler.h"
#include "PortIO.h"
#include "ProgrammableInterruptController.h"
#include "Logger.h"
#include "FrameBuffer.h"

namespace cosmo
{
void interrupt::isr_handler(struct InterruptContext* int_context)
{
    cosmo::FrameBuffer fb;
    cosmo::Logger logger;
    logger.LogError(fb, "Exception %X occurred.",
                    static_cast<unsigned int>(int_context->int_no));
}

void interrupt::irq_handler(struct InterruptContext* int_context)
{
    cosmo::FrameBuffer fb;
    cosmo::Logger logger;
    /* TODO: Implement a complete switch statement. */
    static int i = 0;
    switch(int_context->int_no) {
        case Irq::kKeyboard:
            i++;
            logger.LogError(fb, "KBD Interrupt %d occurred.", i);
            cosmo::Pic::SendEOI(1);
            break;
        default:
            logger.LogError(fb, "IRQ %d occurred.",
                            static_cast<int>(int_context->int_no));
    }
}
} // end cosmo
