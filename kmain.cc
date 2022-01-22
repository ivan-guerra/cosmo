#include "SerialPort.h"
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

    const char* kWelcomeText = "Welcome to cosmo OS!";
    fb.PrintString(kWelcomeText, StrLen(kWelcomeText));

    const char* kSerialWriteNotice = "\nWriting serial data...";
    fb.PrintString(kSerialWriteNotice, StrLen(kSerialWriteNotice));

    const char* kSerialMsg = "Hello, world.\nThis is cosmo OS speaking!";
    const char* kSerialErrorMsg = "\nUnable to initialize serial port!";
    const char* kSerialSuccessMsg = "\nData sent to serial port successfully!";

    cosmo::SerialPort sp;
    if (!sp.Init()) {
        fb.PrintString(kSerialErrorMsg, StrLen(kSerialErrorMsg));
    } else {
        fb.PrintString(kSerialSuccessMsg, StrLen(kSerialSuccessMsg));
        sp.PrintString(kSerialMsg, StrLen(kSerialMsg));
    }

    return 0;
}
