#pragma once

namespace cosmo
{
/*!
 * \class FrameBuffer
 * \brief The FrameBuffer class models the VGA text mode screen buffer.
 *
 * FrameBuffer provides a handle by which the User's can output text to the
 * screen. FrameBuffer also allows the User to change the position of the on
 * screen cursor. A large portion of the implementation was lifted from this
 * tutorial:
 * <a href="http://kernelx.weebly.com/text-console.html">Text Based Console</a>
 */
class FrameBuffer
{
public:
    /*!
     * \enum FrameBuffer::FrameBufferColor
     *
     * This enum aliases the integer codes for each of the 16 colors supported
     * by VGA text mode.
     */
    enum FrameBufferColor
    {
        kBlack         = 0,
        kBlue          = 1,
        kGreen         = 2,
        kCyan          = 3,
        kRed           = 4,
        kMagenta       = 5,
        kBrown         = 6,
        kLight_grey    = 7,
        kDark_grey     = 8,
        kLight_blue    = 9,
        kLight_green   = 10,
        kLight_cyan    = 11,
        kLight_red     = 12,
        kLight_magenta = 13,
        kLight_brown   = 14,
        kWhite         = 15
    };

    static const int kNumRows; /*!< Number of rows in the screen matrix. */
    static const int kNumCols; /*!< Number of columns in the screen matrix. */

    /*!
     * \brief Constructs a FrameBuffer with the parameter FG and BG colors.
     *
     * \param fg_color Text foreground color.
     * \param bg_color Text background color.
     */
    FrameBuffer(FrameBufferColor fg_color=FrameBufferColor::kWhite,
                FrameBufferColor bg_color=FrameBufferColor::kBlack);

    /*!
     * \brief Default destruct the FrameBuffer.
     */
    ~FrameBuffer() = default;

    /* Default copy construction and copy assignment are allowed. */
    FrameBuffer(const FrameBuffer&) = default;
    FrameBuffer& operator=(const FrameBuffer&) = default;

    /* Default move construction and move assignment are allowed. */
    FrameBuffer(FrameBuffer&&) = default;
    FrameBuffer& operator=(FrameBuffer&&) = default;

    /*!
     * \brief Clear the screen of all text and reset the cursor to the origin.
     */
    void ClearScreen();

    /*!
     * \brief Move the cursor to the point (\a row, \a col) on screen.
     *
     * When an invalid \a row and/or \a col value is given, MoveCursor
     * immediately returns leaving the state of the cursor as it was before
     * the call.
     *
     * \param row Cursor row (valid values range from [0,24]).
     * \param col Cursor column (valid values range from [0, 79]).
     */
    void MoveCursor(int row, int col);

    /*!
     * \brief Print the string \a str to the screen.
     *
     * Passing a \a len value that extends beyond the end of \a str can be
     * expected to cause undefined behavior.
     *
     * \param str C-style character string.
     * \param len Length of \a str.
     */
    template <typename T>
    void PrintString(T* str, unsigned int len);

private:
    /*!
     * \enum FrameBuffer::FrameBufferIOCmd
     *
     * This enum aliases the high and low byte commands to the framebuffer.
     */
    enum FrameBufferIOCmd
    {
        kHighByteCommand = 14, /*!< Framebuffer high byte command. */
        kLowByteCommand  = 15 /*!< Framebuffer low byte command. */
    };

    /*!
     * \enum Framebuffer::FrameBufferIOPort
     *
     * This enum aliases the framebuffer command and data ports.
     */
    enum FrameBufferIOPort
    {
        kCommandPort = 0x3D4, /*!< Framebuffer command port address. */
        kDataPort    = 0x3D5 /*!< Framebuffer data port address. */
    };

    /*!
     * \struct Framebuffer::CursorPos
     * \brief The CursorPos struct contains the position of the cursor.
     */
    struct CursorPos
    {
        int x = 0; /*!< Cursor column index. */
        int y = 0; /*!< Cursor row index. */
    };

    static unsigned short* kFrameBufferAddress; /*!< VGA memory start address. */

    /*!
     * \brief Print \a c to the screen at the current cursor location.
     */
    void PrintChar(char c);

    /*!
     * \brief Scroll text if the current cursor row value exceeds the limit.
     *
     * The screen is only scrolled if the next write at the current cursor
     * position would fall outside the bounds of the VGA buffer. If the latter
     * is not true, ScrollScreen immediately returns without altering the
     * display or cursor.
     */
    void ScrollScreen();

    unsigned short* video_mem_; /*!< Start address of VGA memory. */
    unsigned char   attr_byte_; /*!< Attribute byte containing FG/BG data. */
    CursorPos       cursor_pos_; /*!< Current cursor position. */
}; // end Framebuffer

template <typename T>
void FrameBuffer::PrintString(T* str, unsigned int len)
{
    /* NOOP if given a nullptr. */
    if (!str)
        return;

    /* Print the string characters one at a time to the screen. */
    for (unsigned int i = 0; i < len; ++i)
        PrintChar(str[i]);
}
} // end cosmo