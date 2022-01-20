#include "FrameBuffer.h"

void FillStr(char* str, unsigned int len, char fill)
{
    if (!str)
        return;

    for (unsigned int i = 0; i < len; ++i)
        str[i] = fill;
}

unsigned int StrLen(const char* str) {
    if (!str)
        return 0;

    unsigned int len = 0;
    for (int i = 0; str[i]; ++i)
        len++;

    return len;
}

int main(void)
{
    cosmo::FrameBuffer fb;
    fb.ClearScreen();

    const char* text = "Welcome to cosmo OS!";
    fb.PrintString(text, StrLen(text));

    return 0;
}
