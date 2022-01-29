#include "InterruptHandler.h"
#include "Logger.h"
#include "FrameBuffer.h"

void interrupt_handler(struct InterruptContext* int_context)
{
    cosmo::FrameBuffer fb;
    fb.ClearScreen();

    cosmo::Logger logger;
    logger.LogError(fb, "Exception %X occurred.",
                    static_cast<unsigned int>(int_context->int_no));
}
