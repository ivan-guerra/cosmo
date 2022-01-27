#include "Logger.h"
#include "FrameBuffer.h"

extern "C" int kernel_main(void)
{
    cosmo::FrameBuffer fb;
    fb.ClearScreen();

    /* Sample values to be logged. */
    char         cvalue = '#';
    int          ivalue = -42;
    unsigned int uvalue = 0xffffffff;
    const char*  svalue = "cosmo OS";

    cosmo::Logger logger;
    logger.LogInfo(fb,
        "cvalue = %c\nivalue = %d\nuvalue = %u\nuvalue(hex) = %X\nsvalue = %s\n",
        cvalue, ivalue, uvalue, uvalue, svalue);

    return 0;
}
