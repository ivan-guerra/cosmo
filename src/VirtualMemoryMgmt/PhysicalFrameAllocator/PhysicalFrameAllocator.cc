#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "multiboot.h"
#include "PhysicalFrameAllocator.h"

namespace cosmo
{
namespace vmem
{
void BitmapSet(uint32_t *bitmap, int bit) {
    bitmap[bit / 32] |= (1 << (bit % 32));
}

void BitmapUnset(uint32_t *bitmap, int bit) {
    bitmap[bit / 32] &= ~(1 << (bit % 32));
}

int BitmapTest(uint32_t *bitmap, int bit) {
    return bitmap[bit / 32] & (1 << (bit % 32));
}

int BitmapFirstUnset(uint32_t *bitmap, size_t size) {

    uint32_t rem_bits = size % 32;

    for (uint32_t i = 0; i < size / 32; i++) {
        if (bitmap[i] != 0XFFFFFFFF) {
            for (int j = 0; j < 32; j++) {
                if (!(bitmap[i] & (1 << j)))
                    return (i * 32) + j;
            }
        }
    }

    if (rem_bits) {
        for (uint32_t j = 0; j < rem_bits; j++) {
            if (!(bitmap[size / 32] & (1 << j)))
                return ((size / 32) * 32) + j;
        }
    }
    return -1;
}

PhysicalFrameAllocator::PhysicalFrameAllocator() :
    mem_size_(0),
    max_frames_(0),
    used_frames_(0),
    pmmap_size_(0),
    pmmap_(nullptr)
{

}

void PhysicalFrameAllocator::InitRegion(uint32_t base, size_t size)
{
    size_t   frames = size / kFrameSize;
    uint32_t align  = base / kFrameSize;
    for (size_t i = 0; i < frames; i++) {
        BitmapUnset(pmmap_, align++);
        used_frames_--;
    }

    BitmapSet(pmmap_, 0);
}

void PhysicalFrameAllocator::FreeRegion(uint32_t base, size_t size)
{
    size_t   frames = (size < kFrameSize) ? 1 : (size / kFrameSize);
    uint32_t align  = base / kFrameSize;
    for (size_t i = 0; i < frames; i++) {
        BitmapSet(pmmap_, align++);
        used_frames_++;
    }
}

void PhysicalFrameAllocator::InitAvailableRegions(uint32_t mmap_addr,
                                                  uint32_t mmap_end_addr)
{
    multiboot_memory_map_t* mmap     =
        reinterpret_cast<multiboot_memory_map_t *>(mmap_addr);
    multiboot_memory_map_t* mmap_end =
        reinterpret_cast<multiboot_memory_map_t *>(mmap_end_addr);
    while (mmap < mmap_end) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            InitRegion(static_cast<uint32_t>(mmap->addr_low),
                       static_cast<size_t>(mmap->len_low));
        }
        mmap++;
    }
}

void PhysicalFrameAllocator::DeinitKernel(uint32_t kernel_start,
                                          uint32_t kernel_end)
{
    size_t   kernel_size        = kernel_end - kernel_start;
    uint32_t pmmap_size_aligned = pmmap_size_;
    if (!IsAligned(pmmap_size_aligned, kFrameSize))
        pmmap_size_aligned = Align(pmmap_size_aligned, kFrameSize);

    /* No need to align kernel_size since kernel_start/end alignment
       is done in the link.ld script. */
    FreeRegion(kernel_start, kernel_size);
    FreeRegion(kernel_end, pmmap_size_aligned);
}

PhysicalFrameAllocator& PhysicalFrameAllocator::GetInstance()
{
    static PhysicalFrameAllocator allocator;
    return allocator;
}

void PhysicalFrameAllocator::Init(const multiboot_info_t* mb_info,
                                  uint32_t pmmap_addr,
                                  size_t pmmap_size,
                                  struct KernelDescriptor kernel_desc)
{
    if (!mb_info)
        return;

    mem_size_    = pmmap_size;
    max_frames_  = KilobytesToFrames(mem_size_);
    used_frames_ = max_frames_;
    pmmap_       =
        reinterpret_cast<uint32_t*>(
            PhysicalToVirtual(pmmap_addr, kernel_desc.kernel_virtual_start));

    pmmap_size_ = max_frames_ / kFramesPerDword;
    if (max_frames_ % kFramesPerDword)
        pmmap_size_++;

    /* Set all bits in the bitmap to 1 (i.e., mark all of memory as in
       use. Subsequently, we will free memory as directed by the Multiboot
       info we recved from GRUB. */
    memset(pmmap_, 0xFF, pmmap_size_);

    /* Initialize (i.e., mark as ready for use) those frames indicated
       as available by the Multiboot multiboot_memory_map_t structs. */
    InitAvailableRegions(
        PhysicalToVirtual(mb_info->mmap_addr, kernel_desc.kernel_virtual_base),
        PhysicalToVirtual(mb_info->mmap_addr + mb_info->mmap_length,
                          kernel_desc.kernel_virtual_base));

    /* Mark kernel frames as in use. */
    DeinitKernel(kernel_desc.kernel_physical_start,
                 kernel_desc.kernel_physical_end);
}

void* PhysicalFrameAllocator::AllocFrame()
{
    if (used_frames_ - max_frames_ <= 0)
        /* Uses properties of unsigned integers to detect allocation limit. */
        return nullptr;

    int p_index = BitmapFirstUnset(pmmap_, max_frames_);
    if (-1 == p_index)
        return nullptr;

    BitmapSet(pmmap_, p_index);
    used_frames_++;

    return reinterpret_cast<void *>(kFrameSize * p_index);
}

void PhysicalFrameAllocator::FreeFrame(void* frame)
{
    if (!frame)
        /* NOOP if given a NULL frame. */
        return;

    uint32_t frame_addr = reinterpret_cast<uintptr_t>(frame);

    int index = frame_addr / kFrameSize;
    BitmapUnset(pmmap_, index);
    used_frames_--;
}
} // end vmem
} // end cosmo
