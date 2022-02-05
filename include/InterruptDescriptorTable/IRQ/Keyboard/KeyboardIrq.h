#pragma once

#include <stdint.h>

namespace cosmo
{
namespace interrupt
{
namespace irq
{
namespace kbd
{
    /*!
     * \brief Read a keyboard scan code.
     *
     * \return The scan code read from the KBD input buffer.
     */
    uint8_t ReadScanCode();

    /*!
     * \brief Print the ASCII character referred to by \a scan_code to screen.
     *
     * \param scan_code A KBD scan code read via a call to ReadScanCode().
     */
    void PrintAsciiChar(uint8_t scan_code);
} // end kbd
} // end irq
} // end interrupt
} // end cosmo
