## ARM dts vs INTEL acpi

* [arm dts vs intel acpi](https://unix.stackexchange.com/questions/399619/why-do-embedded-systems-need-device-tree-while-pcs-dont)

Advanced Configuration and Power Interface (ACPI) is an open
standard that operating systems can use to discover and
configure computer hardware components, to perform power
management (e.g. putting unused hardware components to sleep),
auto configuration (e.g. Plug and Play and hot swapping), and
status monitoring.

### ARM dts vs INTEL acpi [the extended answer]

Peripherals are connected to the main processor via a bus. Some
bus protocols support enumeration (also called discovery), i.e.
the main processor can ask “what devices are connected to this
bus?” and the devices reply with some information about their
type, manufacturer, model and configuration in a standardized
format. With that information, the operating system can report
the list of available devices and decide which device driver
to use for each of them. Some bus protocols don't support
enumeration, and then the main processor has no way to find
out what devices are connected other than guessing.

All modern PC buses support enumeration, in particular PCI (the
original as well as its extensions and successors such as AGP
and PCIe), over which most internal peripherals are connected,
USB (all versions), over which most external peripherals are
connected, as well as Firewire, SCSI, all modern versions of
ATA/SATA, etc. Modern monitor connections also support discovery
of the connected monitor (HDMI, DisplayPort, DVI, VGA with
EDID). So on a PC, the operating system can discover the
connected peripherals by enumerating the PCI bus, and
enumerating the USB bus when it finds a USB controller on the
PCI bus, etc. Note that the OS has to assume the existence of
the PCI bus and the way to probe it; this is standardized on
the PC architecture (“PC architecture” doesn't just mean an x86
processor: to be a (modern) PC, a computer also has to have a
PCI bus and has to boot in a certain way).

Many embedded systems use less fancy buses that don't support
enumeration. This was true on PC up to the mid-1990s, before PCI
overtook ISA. Most ARM systems, in particular, have buses that
don't support enumeration. This is also the case with some
embedded x86 systems that don't follow the PC architecture.
Without enumeration, the operating system has to be told what
devices are present and how to access them. The device tree is
a standard format to represent this information.

The main reason PC buses support discovery is that they're
designed to allow a modular architecture where devices can be
added and removed, e.g. adding an extension card into a PC or
connecting a cable on an external port. Embedded systems
typically have a fixed set of devices¹, and an operating system
that's pre-loaded by the manufacturer and doesn't get replaced,
so enumeration is not necessary.

### ACPICA Overview

* [ACPICA Overview](https://www.intel.com/content/www/us/en/developer/topic-technology/open/acpica/overview.html)

#### ABSTRACT

With ACPI, power management moves from the BIOS to the operating system.

	- An application can tell the operating system that
	  the display is in use, and change its power policy
	  accordingly.
	- The operating system does not have the size limitation
	  of the BIOS.
	- The ACPI system firmware describes the system's
	  characteristics by placing data, organized into
	  tables, into the main memory.

ACPICA code is fairly mature and implements the following:

	- An AML (ACPI machine language) interpreter
	- A table manager
	- A namespace manager
	- A resource manager
	- Fixed and general purpose event support
	- ACPI hardware support
	- Support for the ACPI 5.0 specification

### ACPI Tables

* [ACPI Tables](https://www.kernel.org/doc/html/next/arm64/acpi_object_usage.html)

#### ACPI commands

Example ACPI commands:

	### Fedora 39
	$ sudo dnf install acpidump

	### Ubuntu
	$ sudo apt install acpidump

	### Man page
	$ man acpidump

	### General help
	$ acpidump -h

	Usage: acpidump [options]
	Options:
	  -b                  Dump tables to binary files
	  -h -?               This help message
	  -o <File>           Redirect output to file
	  -r <Address>        Dump tables from specified RSDP
	  -s                  Print table summaries only
	  -v                  Display version information
	  -vd                 Display build date and time
	  -z                  Verbose mode

	Table Options:
	  -a <Address>        Get table via a physical address
	  -c <on|off>         Turning on/off customized table dumping
	  -f <BinaryFile>     Get table via a binary file
	  -n <Signature>      Get table via a name/signature
	  -x                  Use RSDT instead of XSDT

	Invocation without parameters dumps all available tables
	Multiple mixed instances of -a, -f, and -n are supported

	### Options help
	$ sudo acpidump -r --help

### Possible ACPI code handling in the Linux kernel

	### Tables' handling code
	.../linux-stable/arch/x86/mm

	### Tables themselves
	.../linux-stable/arch/x86/kernel/acpi/
	.../linux-stable/arch/x86/kernel/acpi/boot.c

