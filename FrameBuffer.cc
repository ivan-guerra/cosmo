#include "FrameBuffer.h"

/*!
 * \brief See framebuffer_asm.s for details.
 */
extern "C" void outb(unsigned short port, unsigned char data);

namespace cosmo
{
const int FrameBuffer::kNumRows = 25;
const int FrameBuffer::kNumCols = 80;
unsigned short* FrameBuffer::kFrameBufferAddress =
    reinterpret_cast<unsigned short*>(0x000B8000);

void FrameBuffer::PrintChar(char c)
{
    const int       kNumTab   = 4;
    unsigned short  attribute = attr_byte_ << 8;
    unsigned short* location  = nullptr;

    if ((0x08 == c) && cursor_pos_.x) {
        /* Handle a backspace character. Walk back a column. */
        cursor_pos_.x--;
    } else if ('\t' == c) {
        /* Push the cursor foward by the number stored in kNumTab. */
        cursor_pos_.x = (cursor_pos_.x + kNumTab) & ~(kNumTab - 1);
    } else if ('\r' == c) {
        cursor_pos_.x = 0;
    } else if ('\n' == c) {
        cursor_pos_.x = 0;
        cursor_pos_.y++;
    } else if (c >= ' ') {
        location = video_mem_ + (cursor_pos_.y * kNumCols + cursor_pos_.x);

        /* The data to written to the VGA memory cell is a 16-bit value with
           the format ATTR(8) | CHAR(8). */
        *location = c | attribute;

        /* Advance the cursor by one. */
        cursor_pos_.x++;
    }

    /* Handle the EOL case. */
    if (cursor_pos_.x >= kNumCols) {
        cursor_pos_.x = 0;
        cursor_pos_.y++;
    }

    /* Scroll the screen if necessary. */
    ScrollScreen();

    /* Update the cursor location on screen. */
    MoveCursor(cursor_pos_.y, cursor_pos_.x);
}

void FrameBuffer::ScrollScreen()
{
    /* NOOP in the case where there is room left to right in the VGA buffer. */
    if (cursor_pos_.y < kNumRows)
        return;

    /* space is an space char fitted with the User's FG/BG attributes. */
    unsigned short space = 0x20 | (attr_byte_ << 8);

    /* Move all text up by one line. */
    for (int i = 0; i < ((kNumRows - 1) * kNumCols); ++i)
        video_mem_[i] = video_mem_[i + kNumCols];

    /* Set the last line. */
    for (int i = (kNumRows - 1) * kNumCols; i < (kNumRows * kNumCols); ++i)
        video_mem_[i] = space;

    /* Update the cursor row position to point at the last line. */
    cursor_pos_.y = kNumRows - 1;
}

FrameBuffer::FrameBuffer(FrameBufferColor fg_color,
                         FrameBufferColor bg_color) :
    video_mem_(kFrameBufferAddress)
{
   attr_byte_ = (bg_color << 4) | (fg_color & 0x0F);
}

void FrameBuffer::ClearScreen()
{
    /* Fill the screen with blanks. Inclusion of attr_byte_ ensures the
       text has the correct FG and BG colors. */
    unsigned short space = 0x20 | (attr_byte_ << 8);
    for (int i = 0; i < (kNumRows * kNumCols); ++i)
        video_mem_[i] = space;

    /* Reset the cursor to the top, left most position. */
    MoveCursor(0, 0);
}

void FrameBuffer::MoveCursor(int row, int col)
{
    /* NOOP when given invalid row and/or col. */
    if ((row < 0) || (row >= kNumRows) || (col < 0) || (col >= kNumCols))
        return;

    cursor_pos_.y = row;
    cursor_pos_.x = col;
    unsigned short pos = (row * kNumCols) + col;

    /* Set the cursor low port.*/
    outb(FrameBufferIOPort::kCommandPort, FrameBufferIOCmd::kHighByteCommand);
    outb(FrameBufferIOPort::kDataPort,    (pos >> 8) & 0x00FF);

    /* Set the cursor high port. */
    outb(FrameBufferIOPort::kCommandPort, FrameBufferIOCmd::kLowByteCommand);
    outb(FrameBufferIOPort::kDataPort,    pos & 0x00FF);
}

} // end cosmo
