OBJECTS = loader.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: cosmo.iso

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

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf cosmo.iso bochslog.txt
