#!/bin/bash

cd efi-example
tar -xjvf gnu-efi-3.0.17.tar.bz2
cd gnu-efi-3.0.17

### Make the UEFI compatible/linkable environment
make

### Then get back to gnu-efi/efi-example directory
cd ..

### Make .efi executable
make
