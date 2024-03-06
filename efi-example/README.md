# Install GNU EFI environment in 6 easy steps

## Summary of the steps

After cloning the gnu-efi git, the following steps are required!

	[1] .../gnu-efi$ cd efi-example
	[2] .../gnu-efi/efi-example$ tar -xjvf gnu-efi-3.0.17.tar.bz2
	[3] .../gnu-efi/efi-example$ cd gnu-efi-3.0.17
	### Make the UEFI compatible/linkable environment
	[4] .../gnu-efi/efi-example/gnu-efi-3.0.17$ make
	### Then get back to gnu-efi/efi-example directory
	[5] .../gnu-efi/efi-example/gnu-efi-3.0.17$ cd ..
	### Make .efi executable
	[6] .../gnu-efi/efi-example/gnu-efi-3.0.17$ make

THAT'S IT, FOLKS!

All this is outlined in the script: .../gnu-efi/install.sh

Further down, just some more detailed instructions.

## Install GNU EFI environment and make listacpi.efi

### Install gnu-efi (I use Fedora 39) package

	### Fedora
	$ sudo dnf install gnu-efi

	Package gnu-efi-1:3.0.11-14.fc39.x86_64 is already installed.
	Dependencies resolved.
	Nothing to do.
or
	### Debian & Ubuntu
	$ sudo apt install gnu-efi

### Download and compile Nigel Croxon’s gnu-efi library

Download and compile Nigel Croxon’s gnu-efi library:

	### Download Nigel Croxon’s gnu-efi library and create the home directory
	### From the browser to ~/Downlosd/
	https://sourceforge.net/projects/gnu-efi/files/latest/download

	~$ cp ~/Downloads/gnu-efi-3.0.17.tar.bz2 .../gnu-efi/efi-example

	### Untar the Nigel Croxon’s gnu-efi library
	.../gnu-efi/efi-example$ tar -xjvf gnu-efi-3.0.17.tar.bz2

	.../gnu-efi/efi-example$ cd gnu-efi-3.0.17

	### Make the UEFI compliable/linkable environment
	.../gnu-efi/efi-example/gnu-efi-3.0.17$ make

	### Then get back to ap directory
	.../gnu-efi/efi-example/gnu-efi-3.0.17$ cd ..

	.../gnu-efi/efi-example$ ls -al
	total 192
	drwxr-xr-x. 3 vuser vusers   4096 Mar  8 17:22 .
	drwxr-xr-x. 4 vuser vusers   4096 Mar  8 16:52 ..
	drwxr-xr-x. 7 vuser vusers   4096 Mar  8 17:22 gnu-efi-3.0.17
	-rw-r--r--. 1 vuser vusers 165568 Mar  8 16:53 gnu-efi-3.0.17.tar.bz2
	-rw-r--r--. 1 vuser vusers   4167 Mar  8 16:57 listacpi.c
	-rw-r--r--. 1 vuser vusers   1217 Mar  8 16:56 Makefile
	-rw-r--r--. 1 vuser vusers   1987 Mar  8 17:33 README.md

### Headers using the minimal headers in from gnu-efi-3.0.17/ inc/

	.../gnu-efi/gnu-efi-3.0.17/inc

### Creating an EFI executable

Please, execute makefile as:

	.../gnu-efi/efi-example/gnu-efi-3.0.17$ make clean
	.../gnu-efi/efi-example/gnu-efi-3.0.17$ make

For me, according to the mockup absolute paths Makefile (NOT one
TRUE relative paths Makefile I am using, this is just Makefile
on sticks and ropes/on apparatuses):

	gcc -I /home/vuser/projects/github/gnu-uefi/gnu-efi-3.0.17/inc \
	-ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar \
	-fPIC -mno-red-zone -maccumulate-outgoing-args -c listacpi.c -o listacpi.o
	ld.bfd -shared -Bsymbolic -L/home/vuser/projects/github/gnu-uefi/gnu-efi-3.0.17/x86_64/lib \
	-L/home/vuser/projects/github/gnu-uefi/gnu-efi-3.0.17/x86_64/gnuefi \
	-T/home/vuser/projects/github/gnu-uefi/gnu-efi-3.0.17/gnuefi/elf_x86_64_efi.lds \
	/home/vuser/projects/github/gnu-uefi/gnu-efi-3.0.17/x86_64/gnuefi/crt0-efi-x86_64.o \
	listacpi.o -o listacpi.so -lgnuefi -lefi
	objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j \
	.rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 listacpi.so listacpi.efi
	rm listacpi.o listacpi.so

### After creating an EFI executable

You will find listacpi.elf in the ./

	~/projects/github/gnu-efi/efi-example$ ls -al
	total 244
	drwxr-xr-x. 3 vuser vusers   4096 Mar  8 17:36 .
	drwxr-xr-x. 4 vuser vusers   4096 Mar  8 16:52 ..
	drwxr-xr-x. 7 vuser vusers   4096 Mar  8 17:22 gnu-efi-3.0.17
	-rw-r--r--. 1 vuser vusers 165568 Mar  8 16:53 gnu-efi-3.0.17.tar.bz2
	-rw-r--r--. 1 vuser vusers   4167 Mar  8 16:57 listacpi.c
	-rwxr-xr-x. 1 vuser vusers  50380 Mar  8 17:36 listacpi.efi	<<===== .efi exe
	-rw-r--r--. 1 vuser vusers   1217 Mar  8 16:56 Makefile		<<===== mockup Makefile
	-rw-r--r--. 1 vuser vusers   2455 Mar  8 17:35 README.md

Please, do not forget to use sudo (to be root as following
these instructions).

Please, copy listacpi.elf to the /dev/sda2 (which supposed
to be mounted on /boot/efi/).

	.../gnu-efi/efi-example/gnu-efi-3.0.17$ sudo cp ./listacpi.elf /boot/efi/EFI

### Reboot the PC

You should have bootable EFI Shell USB Stick ready, and you
should boot from it:

* [EFI Shell USB Stick](https://www.thomas-krenn.com/de/wiki/EFI_Shell_USB_Stick)

Please,	reboot and boot to the bootable EFI Shell USB Stick.

Once you booted from the bootable EFI Shell USB Stick, you
should automatically stop in EFI Shell.

Use EFI Shell help to continue to the listacpi.elf execution.

![](EFI-Shell-v2.2.jpg)

	Shell> fs0:
	fs0:\>

	### Since you are at the root/mount point /boot/efi/
	fs0:\> cd EFI
	fs0:\EFI> ls
	directory of: fs0:\EFI
	...
	fs0:\EFI> listacpi.efi

For my HP EliteBook 840 notebook the output is the following:

![](acpi_tables.jpg)

(ACPI Tables listed, to be explored in The Future)

### HOMEWORK

To use the original Chapter [2.31] relative paths Makefile using
host /usr/include/efi/ headers for the creation of the [2.33]
relative paths Makefile using the minimal headers from GNU
EFI inc/ .

	~/projects/github/gnu-efi/efi-example$ ls -al
	total 244
	drwxr-xr-x. 3 vuser vusers   4096 Mar  8 17:36 .
	drwxr-xr-x. 4 vuser vusers   4096 Mar  8 16:52 ..
	drwxr-xr-x. 7 vuser vusers   4096 Mar  8 17:22 gnu-efi-3.0.17
	-rw-r--r--. 1 vuser vusers 165568 Mar  8 16:53 gnu-efi-3.0.17.tar.bz2
	-rw-r--r--. 1 vuser vusers   4167 Mar  8 16:57 listacpi.c
	-rwxr-xr-x. 1 vuser vusers  50380 Mar  8 17:36 listacpi.efi	<<===== .efi exe
=====>>	-rw-r--r--. 1 vuser vusers   1217 Mar  8 16:56 Makefile	<<===== [2.33] relative paths Makefile
	-rw-r--r--. 1 vuser vusers   2455 Mar  8 17:35 README.md

Happy efi shell-ing!

