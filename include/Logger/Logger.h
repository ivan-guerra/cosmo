#pragma once

#include "FrameBuffer.h"
#include "SerialPort.h"

namespace cosmo
{
/*!
 * \class Logger
 * \brief The Logger class allows logging to the screen or COM ports.
 *
 * Logger provides a handle by which the User can log to the VGA framebuffer or
 * the serial COM ports. The Logger interface defines four log levels:
 * INFO, WARN, ERROR, and DEBUG. When outputting messages at any level, the
 * User can use printf style syntax to output formatted strings. Logger only
 * provides a small subset of the format specifiers and functionality of
 * normal printf. The format specifiers have the dumbed down form %[flag]
 * with the following flags supported:\n\n
 *
 *   \%d - Signed decimal integer.\n
 *   \%u - Unsigned decimal integer.\n
 *   \%X - Unsigned hexadecimal integer (uppercase).\n
 *   \%s - String of characters.\n
 */
class Logger
{
public:
    Logger();
    ~Logger() = default;

    /* Default copy construction and copy assignment are allowed. */
    Logger(const Logger&) = default;
    Logger& operator=(const Logger&) = default;

    /* Default move construction and move assignment are allowed. */
    Logger(Logger&&) = default;
    Logger& operator=(Logger&&) = default;

    /*!
     * \brief Log information level data.
     *
     * \tparam T The writer object. This type should be one of
     *           cosmo::FrameBuffer or cosmo::SerialPort.
     * \tparam Args Parameter pack of arguments.
     * \param writer Handle to an initialized \a T type object.
     * \param fmt printf style format string (see Logger class description for
     *            more details).
     * \param args Parameter pack of zero or more arguments used to "fill out"
     *             the \a fmt string.
     */
    template <typename T, typename... Args>
    void LogInfo(T& writer, const char* fmt, const Args&... args)
        { Log(writer, LogLevel::kInfo, fmt, args...); }

    /*!
     * \brief Log warning level data.
     *
     * \tparam T The writer object. This type should be one of
     *           cosmo::FrameBuffer or cosmo::SerialPort.
     * \tparam Args Parameter pack of arguments.
     * \param writer Handle to an initialized \a T type object.
     * \param fmt printf style format string (see Logger class description for
     *            more details).
     * \param args Parameter pack of zero or more arguments used to "fill out"
     *             the \a fmt string.
     */
    template <typename T, typename... Args>
    void LogWarn(T& writer, const char* fmt, const Args&... args)
        { Log(writer, LogLevel::kWarn, fmt, args...); }

    /*!
     * \brief Log error level data.
     *
     * \tparam T The writer object. This type should be one of
     *           cosmo::FrameBuffer or cosmo::SerialPort.
     * \tparam Args Parameter pack of arguments.
     * \param writer Handle to an initialized \a T type object.
     * \param fmt printf style format string (see Logger class description for
     *            more details).
     * \param args Parameter pack of zero or more arguments used to "fill out"
     *             the \a fmt string.
     */
    template <typename T, typename... Args>
    void LogError(T& writer, const char* fmt, const Args&... args)
        { Log(writer, LogLevel::kError, fmt, args...); }

    /*!
     * \brief Log debug level data.
     *
     * \tparam T The writer object. This type should be one of
     *           cosmo::FrameBuffer or cosmo::SerialPort.
     * \tparam Args Parameter pack of arguments.
     * \param writer Handle to an initialized \a T type object.
     * \param fmt printf style format string (see Logger class description for
     *            more details).
     * \param args Parameter pack of zero or more arguments used to "fill out"
     *             the \a fmt string.
     */
    template <typename T, typename... Args>
    void LogDebug(T& writer, const char* fmt, const Args&... args)
        { Log(writer, LogLevel::kDebug, fmt, args...); }
private:
    /*!
     * This enum aliases the different log levels.
     */
    enum class LogLevel
    {
        kInfo,  /*!< Informational level. */
        kWarn,  /*!< Warning level. */
        kError, /*!< Error level. */
        kDebug  /*!< Debug level. */
    };

    /*!
     * \class Arg
     * \brief The Arg class captures primitive type data.
     *
     * The Arg class is a helper class of Logger. On construction, an Arg
     * object captures the constructor argument's data type and assigned value.
     * This class is useful when forwarding parameters from Logger::Log() to
     * Logger::Printf().
     */
    class Arg
    {
    public:
        /*!
         * This enum aliases the different typs support by Arg.
         */
        enum class DataType
        {
            kChar,  /*!< char type. */
            kInt,   /*!< int type. */
            kUInt,  /*!< unsigned int type. */
            kStr    /*!< char* type. */
        };

        /*!
         * \brief Default disallowed.
         *
         * It does not make sense to default construct an Arg object because
         * that would mean the Arg object is not tracking any arg data.
         */
        Arg() = delete;

        /*!
         * \brief Construct a char Arg.
         */
        Arg(char value) : type_(DataType::kChar)
            { data_union_.char_value = value; }

        /*!
         * \brief Construct a int Arg.
         */
        Arg(int value) : type_(DataType::kInt)
            { data_union_.int_value = value; }

        /*!
         * \brief Construct a unsigned int Arg.
         */
        Arg(unsigned int value) : type_(DataType::kUInt)
            { data_union_.uint_value = value; }

        /*!
         * \brief Construct a const char* Arg.
         */
        Arg(const char* value);

        ~Arg() = default;

        /* Default copy construction and copy assignment are allowed. */
        Arg(const Arg&) = default;
        Arg& operator=(const Arg&) = default;

        /* Default move construction and move assignment are allowed. */
        Arg(Arg&&) = default;
        Arg& operator=(Arg&&) = default;

        /*!
         * \brief Return the type of this arg.
         */
        DataType GetType() const { return type_; }

        /*!
         * \brief Return the value of this arg.
         *
         * Mismatching the type \a T template parameter and the type used at
         * construction will lead to undefined behavior.
         *
         * \tparam T The C++ type of this Arg object. For example, if you
         *           construct a char Arg object (i.e., Arg my_arg('c')),
         *           then you would call GetValue as my_arg.GetValue<char>().
         */
        template <typename T>
        T GetValue() const;

        /*!
         * \brief Return the length of the string stored in str_buffer_.
         */
        unsigned int GetStrLen() const;

        /*!
         * \brief Return a pointer to the buffered string argument.
         */
        const char* GetStr() const { return str_buffer_; }

    private:
        static const int kStrBufferSize = 256; /*!< String buffer size. */

        union {
            char         char_value;  /*!< char data. */
            int          int_value;   /*!< int data. */
            unsigned int uint_value;  /*!< unsigned int data. */
        } data_union_;  /*!< Union capturing the arg's assigned value. */
        DataType type_; /*!< The data type of the arg. */
        char str_buffer_[kStrBufferSize]; /*!< String arg buffer. */
    }; // end Arg

    static const int kLogBufferSize    = 64; /*!< Logger scratch space size. */

    /*!
     * \brief Return the length of the C-style string in #log_buffer_.
     */
    unsigned int LogBufferLen() const;

    /*!
     * \brief Reverse the data from index i to j in #log_buffer_ (inclusive).
     */
    void LogBufferReverse(int i, int j);

    /*!
     * \brief Store \a n as a C-style string in #log_buffer_.
     *
     * \param n Decimal integer value.
     * \param digits_required Number of digits required in the output string.
     *                        If the string representation of \a n has less
     *                        digits than specified by \a digits_required, then
     *                        \a n's string representation is prefixed with
     *                        0s until \a digits_required digits are present.
     *
     * \return The length of the string representation of \a n.
     */
    int SetLogBuffer(int n, int digits_required=0);

    /*!
     * \brief Store \a n as a C-style string in #log_buffer_.
     *
     * \param n Unsigned decimal integer value.
     * \param digits_required Number of digits required in the output string.
     *                        If the string representation of \a n has less
     *                        digits than specified by \a digits_required, then
     *                        \a n's string representation is prefixed with
     *                        0s until \a digits_required digits are present.
     *
     * \return The length of the string representation of \a n.
     */
    int SetLogBuffer(unsigned int n, int digits_required=0);

    /*!
     * \brief Store \a n as a hexadecimal C-style string in #log_buffer_.
     *
     * \param n Unsigned decimal integer value.
     *
     * \return The length of the string representation of \a n including the
     *         0x prefix.
     */
    int SetLogBufferHex(unsigned int n);

    /*!
     * \brief Print the log level and forward args to Printf().
     */
    template <typename T, typename... Args>
    void Log(T& writer, LogLevel level, const char* fmt,
             const Args&... args);

    /*!
     * \brief Implement printf style printing.
     */
    template <typename T>
    void Printf(T& writer, const char* fmt, const Arg* args);

    char log_buffer_[kLogBufferSize]; /*!< Logger scratchspace. */
}; //end Logger

template <typename T>
T Logger::Arg::GetValue() const
{
    if (DataType::kChar == type_)
        return data_union_.char_value;
    else if (DataType::kInt == type_)
        return data_union_.int_value;
    else if (DataType::kUInt == type_)
        return data_union_.uint_value;
    else
        /* UB caused by this return is documented. */
        return 0;
}

template <typename T, typename... Args>
void Logger::Log(T& writer, LogLevel level, const char* fmt,
                 const Args&... args)
{
    /* Output the log level prefix string. */
    if (LogLevel::kInfo == level)
        writer.PrintString("[INFO] ", 7);
    else if (LogLevel::kWarn == level)
        writer.PrintString("[WARN] ", 7);
    else if (LogLevel::kError == level)
        writer.PrintString("[ERROR] ", 8);
    else if (LogLevel::kDebug == level)
        writer.PrintString("[DEBUG] ", 8);

    Arg arg_array[] = {args...};
    Printf(writer, fmt, arg_array);
}

template <typename T>
void Logger::Printf(T& writer, const char* fmt, const Arg* args)
{
    int arg_index = 0;
    for (int i = 0; fmt[i] != '\0'; ++i) {
        while ((fmt[i] != '\0') && (fmt[i] != '%')) {
            writer.PrintChar(fmt[i]);
            i++;
        }

        /* Guard against reading beyond the end of the format string. */
        if (fmt[i] == '\0')
            return;

        /* Process the format specifier (i.e., char after the '%'). */
        i++;
        switch (fmt[i]) {
            case 'c':
                writer.PrintChar(args[arg_index].GetValue<char>());
                arg_index++;
                break;
            case 'd':
                SetLogBuffer(args[arg_index].GetValue<int>());
                arg_index++;
                writer.PrintString(log_buffer_, LogBufferLen());
                break;
            case 'u':
                SetLogBuffer(args[arg_index].GetValue<unsigned int>());
                arg_index++;
                writer.PrintString(log_buffer_, LogBufferLen());
                break;
            case 'X':
                SetLogBufferHex(args[arg_index].GetValue<unsigned int>());
                arg_index++;
                writer.PrintString(log_buffer_, LogBufferLen());
                break;
            case 's':
                writer.PrintString(args[arg_index].GetStr(),
                                   args[arg_index].GetStrLen());
                arg_index++;
                break;
        }
    }
}
} // end cosmo
