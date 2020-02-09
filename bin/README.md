### 'ready to use' files
This folder contains some pre-built binaries, all of them are linked statically and it should be possible to execute them without any further precautions, after you've copied the file for your platform to the target system and made it executable (with ```chmod```). If you want to use a file on a regular base, you should create symbolic links for the wanted applets.

Please don't forget to check the detached GnuPG signature, provided together with each binary file. You can find my public key (```KeyID 0x30311D96```) on ```keys.gnupg.net``` or as file ```PeterPawn.asc``` in the root folder of this project.

The files provided and their target platforms are:

* decoder.armv7
  - built with ```libnettle``` (3.5.1) and ```glibc``` on a Raspberry Pi (4B) running Raspbian (Buster)
  - it's a little bit larger with glibc, but I had no usable ```uClibc``` for Raspbian and there's no other need to build one
* decoder.mips32r2
  - built for ```big endian``` machines with ```libnettle``` (3.5.1) and ```uClibc``` (1.0.14) using the ```Freetz``` toolchain (from my YourFreetz fork)
  - really small for a statically linked program, thanks to the used C library
  - should be usable on all FRITZ!Box models with VR9 and GRX5 chipset, works on Vx180 (7390), too
* decoder.x86
  - built with ```libnettle``` (3.5.1) and ```glibc```, targeting Intel 80386 compatible systems
  - somewhat larger due to the usage of ```glibc```
* decoder.x86_64
  - built with ```libnettle``` (3.5.1) and ```glibc```, targeting x64 compatible systems
  - should be usable from an Ubuntu installation in Windows 10
  - somewhat larger due to the usage of ```glibc```
