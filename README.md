# kernel
This is just a hobby project of mine experimenting with creating an operating system kernel. It doesn't do much more than boot at the moment, but it's a fun little project to hack on. I'll be adding more functionality as time permits.

The project requires GCC and GNU Assembler to build. The xorriso package is required to create a bootable iso for the kernel. The makefile also includes a run target that attempts to boot the generated iso file in a VirtualBox session. 
