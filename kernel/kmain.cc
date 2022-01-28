#include "Logger.h"
#include "FrameBuffer.h"
#include "GlobalDescriptorTable.h"

extern "C" int kernel_main(void)
{
    /* Setup the GDT. */
    cosmo::GlobalDescriptorTable<5> gdt;
    gdt.SetGate(0, 0, 0, 0, 0);                /* Null segment. */
    gdt.SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Code segment. */
    gdt.SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Data segment. */
    gdt.SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* User mode code segment. */
    gdt.SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* User mode data segment. */
    gdt.FlushGdt();

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
