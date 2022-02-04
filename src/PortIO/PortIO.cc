#include "PortIO.h"

void io_wait()
{
    /* See https://wiki.osdev.org/Inline_Assembly/Examples#IO_WAIT
      for details. */
    outb(0x80, 0);
}
