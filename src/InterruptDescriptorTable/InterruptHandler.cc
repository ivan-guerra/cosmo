#include "InterruptHandler.h"
#include "Logger.h"
#include "FrameBuffer.h"

namespace cosmo
{
void interrupt::isr_handler(struct InterruptContext* int_context)
{
    cosmo::FrameBuffer fb;
    fb.ClearScreen();

    cosmo::Logger logger;
    logger.LogError(fb, "Exception %X occurred.",
                    static_cast<unsigned int>(int_context->int_no));
}
} // end cosmo
