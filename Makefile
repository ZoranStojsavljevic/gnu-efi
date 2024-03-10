CC	= gcc
AS	= as
LD	= ld.bfd
AR	= ar
RANLIB	= ranlib
OBJCOPY	= objcopy

TARGETS = listacpi.efi

all : $(TARGETS)

%.efi : %.so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j \
	.rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 listacpi.so listacpi.efi

### Real listacpi linking .so example

%.so: %.o
	$(LD) -shared -Bsymbolic -L./gnu-efi-3.0.17/x86_64/lib \
	-L./gnu-efi-3.0.17/x86_64/gnuefi \
	-T./gnu-efi-3.0.17/gnuefi/elf_x86_64_efi.lds \
	./gnu-efi-3.0.17/x86_64/gnuefi/crt0-efi-x86_64.o \
	listacpi.o -o listacpi.so -lgnuefi -lefi

### Real listacpi compilation .o example

%.o: %.c
	$(CC) -I ./gnu-efi-3.0.17/inc \
	-ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar \
	-fPIC -mno-red-zone -maccumulate-outgoing-args -c listacpi.c -o listacpi.o

clean:
	@rm -rf *.o *.a *.so $(TARGETS)

