

OUTDIR := build
SRCDIR := src
KERNEL := $(OUTDIR)/catOS.bin
ISO    := iso/catOS.iso

AS      := as
LD      := ld
CXX     := g++
OBJCOPY := objcopy

CXXFLAGS := -std=gnu++17 -ffreestanding -fno-exceptions -fno-rtti -nostdlib -fno-builtin -m32 -Wall -Wextra
LDFLAGS  := -m elf_i386 -T $(SRCDIR)/linker.ld --nmagic

SRCS := $(SRCDIR)/kernel.cpp $(SRCDIR)/vga.cpp
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OUTDIR)/%.o,$(SRCS)) $(OUTDIR)/boot.o

all: dirs $(KERNEL)
	@echo "catOS built: $(KERNEL)"

dirs:
	mkdir -p $(OUTDIR) iso

$(OUTDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTDIR)/boot.o: $(SRCDIR)/boot.S
	$(AS) --32 $< -o $@

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OUTDIR)/catOS.elf
	$(OBJCOPY) -O binary $(OUTDIR)/catOS.elf $(KERNEL)

iso: all
	mkdir -p iso/boot/grub
	cp $(OUTDIR)/catOS.elf iso/boot/catOS.elf
	printf '%s\n' \
	'menuentry "catOS" {' \
	'  multiboot /boot/catOS.elf' \
	'  boot' \
	'}' > iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso || true

clean:
	rm -rf build iso
info:
	@echo "Build directory: $(OUTDIR)"
	@echo "Kernel binary:   $(KERNEL)"
	@echo "ISO image:       $(ISO)"
	@echo "Source files:    $(SRCDIR)"
	@echo "CatOS a hobby kernel in C++ by Taterr"
.PHONY: all clean dirs iso
