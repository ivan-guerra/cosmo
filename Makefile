OBJECTS = loader.o PortIO.o FrameBuffer.o SerialPort.o kmain.o
CC = g++
CPPFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
           -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -std=c++14
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: cosmo.iso

docs:
	doxygen doxygen_conf.txt

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

cosmo.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                      \
        -b boot/grub/stage2_eltorito    \
        -no-emul-boot                   \
        -boot-load-size 4               \
        -A cosmo                        \
        -input-charset utf8             \
        -quiet                          \
        -boot-info-table                \
        -o cosmo.iso                    \
        iso

%.o: %.cc
	$(CC) $(CPPFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf cosmo.iso bochslog.txt docs com*.out
