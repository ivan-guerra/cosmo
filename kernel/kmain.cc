#include <stdint.h>
#include <string.h>

#include "Logger.h"
#include "FrameBuffer.h"
#include "multiboot.h"
#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"
#include "InterruptHandler.h"
#include "ProgrammableInterruptController.h"
#include "PhysicalFrameAllocator.h"

void Halt()
{
    for (;;)
        __asm__ volatile("hlt");
}

void PrintLogo()
{
    auto& fb = cosmo::FrameBuffer::GetInstance();
    fb.ClearScreen();
    fb.SetColor(cosmo::FrameBuffer::FrameBufferColor::kLight_cyan,
                cosmo::FrameBuffer::FrameBufferColor::kBlack);

    const char* logo_lines[] = {
        "          ____ ___  ____  __  __  ___         \n",
        " __/\\__  / ___/ _ \\/ ___||  \\/  |/ _ \\  __/\\__\n",
        " \\    / | |  | | | \\___ \\| |\\/| | | | | \\    /\n",
        " /_  _\\ | |__| |_| |___) | |  | | |_| | /_  _\\\n",
        "   \\/    \\____\\___/|____/|_|  |_|\\___/    \\/  \n"
    };
    for (const auto* line : logo_lines)
        fb.PrintString(line, strlen(line));

    int nblanks = 3;
    while (nblanks--)
        fb.PrintString("\n", 1);

    fb.SetColor(cosmo::FrameBuffer::FrameBufferColor::kWhite,
                cosmo::FrameBuffer::FrameBufferColor::kBlack);
}

void InitGdt()
{
    auto& gdt = cosmo::GlobalDescriptorTable<5>::GetInstance();
    gdt.SetGate(0, 0, 0, 0, 0);                /* Null segment. */
    gdt.SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Code segment. */
    gdt.SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Data segment. */
    gdt.SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* User mode code segment. */
    gdt.SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* User mode data segment. */
    gdt.FlushGdt();
}

void InitIdt()
{
    auto& idt = cosmo::InterruptDescriptorTable::GetInstance();
    /* Register any additional interrupt vectors here with calls to
       idt.SetGate(...). */
    idt.FlushIdt();

    /* The IDT has been setup and registered. Enable interrupts. */
    __asm__ volatile("sti");
}

void InitPic()
{
    /* Remap the master/slave PIC to have the correct vector offsets. */
    cosmo::pic::Init();

    for (uint8_t i = 0; i < 8; ++i) {
        /* Mask all IRQs. Note, we even mask IRQ 2 which corresponds to the
           slave PIC effectively masking all slave IRQs. */
        cosmo::pic::SetMask(i);
    }

    /* Clear the mask on the keyboard IRQ. */
    cosmo::pic::ClearMask(cosmo::interrupt::Irq::kKeyboard);
}

void InitPhysicalFrameAllocator(const multiboot_info_t* mboot_hdr,
                                const cosmo::vmem::KernelDescriptor& kernel_desc)
{
    size_t mem_size_kb = 1024 + mboot_hdr->mem_upper;
    auto& falloc = cosmo::vmem::PhysicalFrameAllocator::GetInstance();
    falloc.Init(mboot_hdr,
                kernel_desc.kernel_physical_end,
                mem_size_kb,
                kernel_desc);
}

extern "C" int kernel_main(uint32_t kernel_physical_start,
                           uint32_t kernel_physical_end,
                           uint32_t kernel_virtual_start,
                           uint32_t kernel_virtual_end,
                           uint32_t kernel_virtual_base,
                           uint32_t mboot_magic,
                           uint32_t mboot_header)
{
    PrintLogo();

    LOG_INFO("Verifying multiboot bootloader magic value...\n");
    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        LOG_ERROR("error, not booted by a Multiboot compliant bootloader!\n");
        Halt();
    }
    LOG_INFO("MULTIBOOT_BOOTLOADER_MAGIC found!\n");

    multiboot_info_t* mboot_hdr =
        reinterpret_cast<multiboot_info_t*>(mboot_header);

    LOG_INFO("Verifying multiboot info includes memory info...\n");
    if (!(mboot_hdr->flags & MULTIBOOT_INFO_MEMORY)) {
        LOG_ERROR("error, multiboot header does not contain meminfo!\n");
        Halt();
    }
    LOG_INFO("MULTIBOOT_INFO_MEMORY was set!\n");

    LOG_INFO("Initializing the GDT...\n");
    InitGdt();
    LOG_INFO("GDT setup succeeded!\n");

    LOG_INFO("Initializing the IDT...\n");
    InitIdt();
    LOG_INFO("IDT setup succeeded!\n");

    LOG_INFO("Initializing PIC...\n");
    InitPic();
    LOG_INFO("PIC setup succeeded!\n");

    LOG_INFO("Initializing Physical Frame Allocator...\n");
    struct cosmo::vmem::KernelDescriptor kernel_desc = {
        .kernel_physical_start = kernel_physical_start,
        .kernel_physical_end   = kernel_physical_end,
        .kernel_virtual_start  = kernel_virtual_start,
        .kernel_virtual_end    = kernel_virtual_end,
        .kernel_virtual_base   = kernel_virtual_base
    };
    InitPhysicalFrameAllocator(mboot_hdr, kernel_desc);
    LOG_INFO("Physical Frame Allocator setup succeeded!\n");

    /* Keep the kernel from exiting. */
    Halt();

    /* Should never make it here... */
    return 0xDEADBEEF;
}
