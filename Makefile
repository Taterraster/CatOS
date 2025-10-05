
CXX = g++
AS = as
LD = ld

CXXFLAGS = -std=gnu++17 -ffreestanding -fno-exceptions -fno-rtti \
	-nostdlib -fno-builtin -m32 -Wall -Wextra

LDFLAGS = -m elf_i386 -T src/linker.ld --nmagic

OBJS = build/kernel.o \
	build/vga.o \
	build/boot.o \
	build/keyboard.o \
	build/catcli.o \
	build/string.o \
	build/ports.o \
	build/fs.o

ELF = build/catOS.elf
ISO = iso/CatOS.iso


BUILD_DIR = build


$(OBJS): | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


all: iso


build/boot.o: src/boot.S
	$(AS) --32 $< -o $@

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(ELF): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(ELF)


iso:$(ELF)
	mkdir -p iso/boot/grub
	cp $(ELF) iso/boot/catOS.elf
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "CatOS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/catOS.elf' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso || echo "grub-mkrescue failed; you can still run qemu with the kernel ELF"



clean:
	rm -rf build iso


run: 
	make clean
	make iso
	qemu-system-i386 -cdrom $(ISO)

.PHONY: all iso run clean
