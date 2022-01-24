#include "Logger.h"

namespace cosmo
{
Logger::Arg::Arg(const char* value) :
    type_(Logger::Arg::DataType::kStr)
{
    /* Copy value to the Arg string buffer. */
    int i = 0;
    for (; value[i] != '\0'; ++i)
        str_buffer_[i] = value[i];
    str_buffer_[i] = '\0';
}

unsigned int Logger::Arg::GetStrLen() const
{
    unsigned int len = 0;
    for (int i = 0; str_buffer_[i] != '\0'; ++i)
        len++;

    return len;
}

unsigned int Logger::LogBufferLen() const
{
    unsigned int len = 0;
    for (int i = 0; log_buffer_[i] != '\0'; ++i)
        len++;

    return len;
}

void Logger::LogBufferReverse(int i, int j)
{
    for (; i < j; i++, j--) {
        char tmp = log_buffer_[i];
        log_buffer_[i] = log_buffer_[j];
        log_buffer_[j] = tmp;
    }
}

int Logger::SetLogBuffer(int n, int digits_required)
{
    /* Keep track of whether the value is negative. */
    bool is_negative = (n < 0);
    if (is_negative)
        n *= -1;

    /* Extract each digit from right to left. */
    int digit = 0;
    int i     = 0;
    while (n) {
        digit = n % 10;
        n /= 10;
        log_buffer_[i++] = static_cast<char>('0' + digit);
    }

    /* Pad the value with 0's if necessary. */
    while (i < digits_required)
        log_buffer_[i++] = '0';

    /* Insert the - sign if necessary. */
    if (is_negative)
        log_buffer_[i++] = '-';

    /* Mark the end of integer string. */
    log_buffer_[i] = '\0';

    /* Reverse the digits in the log buffer to get the correct ordering. */
    LogBufferReverse(0, i - 1);

    return i;
}

int Logger::SetLogBuffer(unsigned int n, int digits_required)
{
    /* Extract each digit from right to left. */
    int digit = 0;
    int i     = 0;
    while (n) {
        digit = n % 10;
        n /= 10;
        log_buffer_[i++] = static_cast<char>('0' + digit);
    }

    /* Pad the value with 0's if necessary. */
    while (i < digits_required)
        log_buffer_[i++] = '0';

    /* Mark the end of integer string. */
    log_buffer_[i] = '\0';

    /* Reverse the digits in the log buffer to get the correct ordering. */
    LogBufferReverse(0, i - 1);

    return i;
}

int Logger::SetLogBufferHex(unsigned int n)
{
    /* Handle the special case input of 0. */
    if (0 == n) {
        log_buffer_[0] = '0';
        log_buffer_[1] = 'x';
        log_buffer_[2] = '0';
        log_buffer_[3] = '\0';

        return 1;
    }

    /* Bulk of the work to convert digits to hex chars. */
    int i = 0;
    int digit = 0;
    while (n) {
        digit = n % 16;
        if (digit < 10)
            log_buffer_[i++] = digit + 48;
        else
            log_buffer_[i++] = digit + 55;
        n /= 16;
    }

    /* Append the '0x' prefix.*/
    log_buffer_[i++] = 'x';
    log_buffer_[i++] = '0';
    log_buffer_[i] = '\0';

    /* Reverse the buffer contents to get the digits in the right order. */
    LogBufferReverse(0, i - 1);

    return i;
}

Logger::Logger()
{
    /* Initialize the log buffer with NULL terminators. */
    for (int i = 0; i < kLogBufferSize; ++i)
        log_buffer_[i] = '\0';
}
} // end cosmo
