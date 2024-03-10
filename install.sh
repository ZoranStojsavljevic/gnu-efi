######################################################################
### Please, find instructions in ./gnu-efi/efi-example/README.md
### Install GNU EFI environment in 5 easy steps
###
### Summary of the steps:
###
### After cloning the gnu-efi git, the following steps are required!
###
###	[1] ./gnu-efi$ tar -xjvf gnu-efi-3.0.17.tar.bz2
###	[2] ./gnu-efi$ cd gnu-efi-3.0.17
###	### Make the UEFI compatible/linkable environment
###	[3] ./gnu-efi/gnu-efi-3.0.17$ make
###	### Then get back to gnu-efi/ directory
###	[4] ./gnu-efi/gnu-efi-3.0.17$ cd ..
###	### Make .efi executable
###     [5] ./gnu-efi$ make
###
###	THAT'S ALL, FOLKS!
###
###	All this is outlined in the script: .../gnu-efi/install.sh
######################################################################

#!/bin/bash

tar -xjvf gnu-efi-3.0.17.tar.bz2
cd gnu-efi-3.0.17

### Make the UEFI compatible/linkable environment
make

### Then get back to gnu-efi/efi-example directory
cd ..

### Make .efi executable
make

