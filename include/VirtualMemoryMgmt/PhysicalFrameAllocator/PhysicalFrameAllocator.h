#pragma once

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

namespace cosmo
{
/*!
 * \namespace vmem
 * \brief Virtual memory management functions.
 */
namespace vmem
{
/*!
 * \struct KernelDescriptor
 * \brief This struct captures kernel addressing info provided at boot time.
 */
struct KernelDescriptor
{
    uint32_t kernel_physical_start; /*!< Kernel physical start address. */
    uint32_t kernel_physical_end;   /*!< Kernel physical end address. */
    uint32_t kernel_virtual_start;  /*!< Kernel virtual start address. */
    uint32_t kernel_virtual_end;    /*!< Kernel virtual end address. */
    uint32_t kernel_virtual_base;   /*!< Virtual base address of kernel space. */
}; // end KernelDescriptor

/*!
 * \brief Set bit \a bit in the \a bitmap.
 */
void BitmapSet(uint32_t* bitmap, int bit);

/*!
 * \brief Clear bit \a bit in the \a bitmap.
 */
void BitmapUnset(uint32_t* bitmap, int bit);

/*!
 * \brief Test bit \a bit in the \a bitmap.
 */
int BitmapTest(uint32_t* bitmap, int bit);

/*!
 * \brief Find the first unset bit in \a bitmap.
 */
int BitmapFirstUnset(uint32_t* bitmap, size_t size);

/*!
 * \class PhysicalFrameAllocator
 * \brief Manage allocation of physical memory frames.
 *
 * PhysicalFrameAllocator provides a handle for the management of physical
 * memory frames. This class uses a bitmap as its frame bookkeeping
 * data structure. The PhysicalFrameAllocator API provides a basic alloc/free
 * interface that allocates/frees a #kFrameSize chunk of memory. This is a
 * singleton class whose Init() function must only be called once on kernel
 * startup.
 */
class PhysicalFrameAllocator
{
public:
    static const uint32_t kFrameSize = 4096; /*!< Size of a page frame in bytes. */

    ~PhysicalFrameAllocator() = default;

    /* Allow default copy construction and assignment. */
    PhysicalFrameAllocator(const PhysicalFrameAllocator&) = default;
    PhysicalFrameAllocator& operator=(const PhysicalFrameAllocator&) = default;

    /* Allow default move construction and assignment. */
    PhysicalFrameAllocator(PhysicalFrameAllocator&&) = default;
    PhysicalFrameAllocator& operator=(PhysicalFrameAllocator&&) = default;

    /*!
     * \brief Return the singleton instance of PhysicalFrameAllocator.
     */
    static PhysicalFrameAllocator& GetInstance();

    /*!
     * \brief Initialize the physical frame allocator.
     *
     * Initialization of the allocator is a three step process:
     *   (1) Initialize the underlying bitmap used to bookkeep frames.
     *   (2) Mark frames as free using the Multiboot info structure for
     *       guidance.
     *   (3) Explicitly mark kernel frames as in use.
     *
     * \param mb_info GRUB Multiboot info structure.
     * \param pmmap_addr Physical address at which the physical memory
     *                   allocator will place its data structures (bitmap).
     * \param pmmap_size Size of physical memory allocator scratch space in KB.
     * \param kernel_desc Kernel descriptor.
     */
    void Init(const multiboot_info_t* mb_info,
              uint32_t pmmap_addr,
              size_t pmmap_size,
              struct KernelDescriptor kernel_desc);

    /*!
     * \brief Return the max number of #kFrameSize frames supported in memory.
     */
    size_t GetMaxFrames() const
        { return max_frames_; }

    /*!
     * \brief Return the number #kFrameSize frames that have been allocated.
     *
     * Some frames marked as used are in use by the kernel or simply
     * unavailable for allocation (i.e., reserved memory).
     */
    size_t GetUsedFrames() const
        { return used_frames_; }

    /*!
     * \brief Allocate a page frame.
     *
     * \return The address of a #kFrameSize bytes frame of memory. If no frames
     *         are available, nullptr is returned.
     */
    void* AllocFrame();

    /*!
     * \brief Free a frame previously allocated by a call to AllocFrame().
     *
     * Passing FreeFrame() an address to a frame not previously allocated
     * via a call to AllocFrame() leads to undefined behavior.
     *
     * \param frame Address returned by a preceding call to AllocFrame().
     */
    void FreeFrame(void* frame);

private:
    static const uint32_t kFramesPerDword = 32; /*!< Number of frames stored in each double word. */

    /*!
     * \brief Default initializes PhysicalFrameAllocator members.
     */
    PhysicalFrameAllocator();

    /*!
     * \brief Convert a size in KB to number of frames.
     */
    uint32_t KilobytesToFrames(uint32_t mem_size_kb) const
        { return ((mem_size_kb * 1024) / kFrameSize); }

    /*!
     * \brief Return \c true if \a addr is \a alignment aligned.
     */
    bool IsAligned(uint32_t addr, uint32_t alignment) const
        { return !((addr) & ~((alignment) - 1)); }

    /*!
     * \brief Return \a addr \a alignment byte aligned.
     */
    uint32_t Align(uint32_t addr, uint32_t alignment) const
        { return (((addr) & ~((alignment) - 1)) + (alignment)); }

    /*!
     * \brief Convert \a physical_address to a virtual address.
     *
     * \param physical_address A physical memory address.
     * \param virtual_base_address A virtual base address.
     */
    uint32_t PhysicalToVirtual(uint32_t physical_address,
                               uint32_t virtual_base_address) const
        { return physical_address + virtual_base_address; }

    /*!
     * \brief Initialize a region of memory.
     *
     * InitRegion() updates the PhysicalFrameAllocator bitmap to mark frames
     * in the range [base, base+size] as in use. Initialization is
     * automatically frame size aligned.
     *
     * \param base A virtual base address.
     * \param size Size of the memory region in bytes.
     */
    void InitRegion(uint32_t base, size_t size);

    /*!
     * \brief Free a region of memory.
     *
     * FreeRegion updates the PhysicalFrameAllocator bitmap to mark frames
     * in the range [base, base+size) as free.
     *
     * \param base A virtual base address.
     * \param size Size of the memory region in bytes.
     */
    void FreeRegion(uint32_t base, size_t size);

    /*!
     * \brief Mark available frames according to Multiboot memory map.
     *
     * InitAvailableRegions() scans the parameter memory map and initializes
     * any regions that Multiboot indicates are available.
     *
     * \param mmap_addr Memory map buffer address provided by Multiboot info
     *                  struct.
     * \param mmap_end_addr End address of the memory map buffer. This can be
     *                      computed using the mmap_addr param and the
     *                      mmap_length field of the Multiboot info struct.
     */
    void InitAvailableRegions(uint32_t mmap_addr,
                              uint32_t mmap_end_addr);

    /*!
     * \brief Mark kernel memory as in use.
     *
     * The memory used by the kernel is marked as available for use by the
     * Multiboot structure. Therefore, a call to InitAvailableRegions() would
     * mark kernel frames as free when they are actually in use. DeinitKernel()
     * solves this by explicitly bookkeeping kernel frames and marking them
     * as in use.
     *
     * \param kernel_start Kernel virtual base address.
     * \param kernel_end Kernel virtual end address.
     */
    void DeinitKernel(uint32_t kernel_start, uint32_t kernel_end);

    size_t    mem_size_;    /*!< Size of memory in KB. */
    size_t    max_frames_;  /*!< Maximum number of page frames supported. */
    size_t    used_frames_; /*!< Number of allocated page frames. */
    size_t    pmmap_size_;  /*!< Number of DWORDs used to store page data. */
    uint32_t* pmmap_;       /*!< Pointer to a bitmap of page frames. */
}; // end PhysicalFrameAllocator
} // end vmem
} // end cosmo
