#pragma once

#include <stdint.h>

namespace cosmo
{
/*!
 * \namespace pic
 * \brief 8259 Programmable Interrupt Controller configuration functions.
 */
namespace pic
{
    constexpr int kDefaultMasterOffset = 0x20; /*!< Default master PIC offset. */
    constexpr int kDefaultSlaveOffset  = 0x28; /*!< Default slave PIC offset. */

    /*!
     * This enum aliases the PIC master and slave port addresses and their
     * control registers.
     */
    enum PicPort
    {
        kPic1        = 0x20,      /*!< IO base address of master PIC. */
        kPic2        = 0xA0,      /*!< IO base address of slave PIC. */
        kPic1Command = kPic1,     /*!< Master PIC command port. */
        kPic1Data    = kPic1 + 1, /*!< Master PIC data port. */
        kPic2Command = kPic2,     /*!< Slave PIC command port. */
        kPic2Data    = kPic2 + 1, /*!< Slave PIC data port. */
        kPicReadIrr  = 0x0A,      /*!< OCW3 IRQ ready next command read. */
        kPicReadIsr  = 0x0B       /*!< OCW3 IRQ service next command read. */
    }; // end PicPort

    /*!
     * This enum aliases the relevant initialization command words used during
     * remapping of the PIC.
     */
    enum Icw
    {
        kIcw1Icw4      = 0x01, /*!< ICW4 (not) needed. */
        kIcw1Single    = 0x02, /*!< Single (cascade) mode. */
        kIcw1Interval4 = 0x04, /*!< Call address interval 4 (8). */
        kIcw1Level     = 0x08, /*!< Level triggered (edge) mode. */
        kIcw1Init      = 0x10, /*!< Intitialization - required! */
        kIcw48086      = 0x01, /*!< 8086/88 (MCS-80/85) mode. */
        kIcw4Auto      = 0x02, /*!< Auto (normal) EOI. */
        kIcw4BufSlave  = 0x08, /*!< Buffered mode/slave. */
        kIcw4BufMaster = 0x0C, /*!< Buffered mode/master. */
        kIcw4Sfnm      = 0x10  /*!< Special fully nested (not). */
    }; // end Icw

    /*!
     * \brief Return the PIC master/slave word specified by \a ocw3.
     *
     * \return A 16-bit word where the upper 8-bits represent the response read
     *         for the slave PIC and the lower 8-bits represent the response
     *         read for the master PIC.
     */
    uint16_t GetIrqReg(int ocw3);

    /*!
     * \brief Send an acknowledgment of \a irq to the PIC.
     *
     * \param irq An IRQ number in the range [0,7] for the master PIC or
     *            [8,15] for the slave PIC.
     */
    void SendEOI(uint8_t irq);

    /*!
     * \brief Initialize the master/slave PIC with the parameter offsets.
     *
     * \param offset1 Vector offset for master PIC.
     * \param offset2 Vector offset for slave PIC.
     */
    void Init(int offset1=kDefaultMasterOffset,
              int offset2=kDefaultSlaveOffset);

    /*!
     * \brief Mask a master/slave PIC IRQ line.
     *
     * \param irq_line An IRQ number in the range [0,7] for the master PIC or
     *                 [8,15] for the slave PIC.
     */
    void SetMask(uint8_t irq_line);

    /*!
     * \brief Clear the mask on a master/slave PIC IRQ line.
     *
     * \param irq_line An IRQ number in the range [0,7] for the master PIC or
     *                 [8,15] for the slave PIC.
     */
    void ClearMask(uint8_t irq_line);

    /*!
     * \brief Get the master/slave PIC Interrupt Request Register contents.
     *
     * \return A 16-bit word where the upper 8 bits correspond to the IRR for
     *         the slave PIC and the lower 8 bits the IRR for the master PIC.
     */
    uint16_t GetIrr();

    /*!
     * \brief Get the master/slave PIC In-service Register contents.
     *
     * \return A 16-bit word where the upper 8 bits correspond to the ISR for
     *         the slave PIC and the lower 8 bits the ISR for the master PIC.
     */
    uint16_t GetIsr();
} // end Pic
} // end cosmo
