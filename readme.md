# Worksheet Two - Brainhurt OS

## Task One - 0xCAFEBABE

To build this task i followed chapter 2 of the Little Book of OS development and the provided resources in the Worksheet Two Part One pdf.

### Overview

The goal of the task is to set up the smallest operating system possible that can write 0xCAFEBABE to the EAX register. To do this i set up the bootloaders and loader.s files. The main bulk of interesting code is found in the loader.s file, shown below. 


*Setting up the magic number*
![Loader code setting magic number](readmeImages/LoaderMagicNumbers.png)
*Moving 0xCAFEBABE to EAX*
![Loader code moving CAFEBABE to EAX](./readmeImages/LoaderCAFEBABE.png)

The above code is vital in the setup of the operating system. Below i have explained some of the above codes use. 

#### Code Explanation

- ```global loader``` makes the loader function accessible outside of the loader.s file. So other files can call it. 
- ```MAGIC_NUMBER equ 0x1BAD002``` equates the hex 0x1BAD002 to the variable MAGIC_NUMBER. This works similar to #define in c, so every time MAGIC_NUMBER is used it will be replaced with 0x1BAD002.
- ```loader:``` This is the start point on the loader function. Everyting inside this function is ran when loader is called. 
- ```mov eax, 0xCAFEBABE``` This moves (copies) the hex value 0xCAFEBABE to the eax register.

### Booting the OS

In order to run and test that the OS is bootable and successfully moving 0xCAFEBABE to the eax register we need to setup the makefile to assemble, link and build the OS. To do this we need to:
- Assemble the ```loader.s``` file using nasm.
- Link the ```loader.o``` to the emulator using the ```link.ld``` file.
- Generate an iso image using stage2_eltorito
- Run the QEMU emulator in curses mode with debug logQ.txt set. 

Below is an image of my makefile. Note this is the complete Makefile for the whole OS so i will identify the bits needed for task one.

*Makefile for booting OS*

![Makefile code for task1](readmeImages/MakefileTask1.png)

### Code Explanation 
- ```nasm -f elf loader.s -o loader.o``` This assembles the ```loader.s``` file into a ```loader.o``` object file
- ```ld -T ./source/link.ld -melf_i386 loader.o -o ./iso/boot/kernel.elf``` This links the ```loader.o``` object file into a ```kernel.elf``` binary using the ```link.ld``` script.

- ```genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o ./iso/os.iso iso```.
This generates an iso image using the grub bootloader ```stage2_eltorito``` creating an ```os.iso``` image.

- ```qemu-system-i386 -display curses -monitor telnet::41451,server, -boot d -cdrom  ./iso/os.iso -m 32 -d cpu -D logQ.txt``` This runs the QEMU emulator and boots the OS, saving a log file ```logQ.txt```

### Testing and proof of 0xCAFEBABE

To test and prove that 0xCAFEBABE has been successfully moved into EAX, we simply have to run the OS and check within the logQ.txt that is generated. I used `crtl + f` to find all occurances of 0xCAFEBABE.

*Evidence of 0xCAFEBABE in logQ.txt*
![Evidence of 0xCAFEBABE in LogQ.txt](readmeImages/LogQCafebabe.png)

The above image shows evidence of CAFEBABE in the debug log of the OS. I can find 6 instances of CAFEBABE in the logs.

### Testing and proof of makefile that builds and runs the OS

To test that the makefile runs correctly i will present an image of the makefile commands below and show them working in the VIVO

*Make all example from makefile*

![[Example make file]](readmeImages/ExampleMakefile.png)

## Task Two - Chapter Three 

### Overview
Task Two instructs us to follow chapter three of the Little Book of OS Development implementing all the books features and at least two other C functions of our choosing. 

### Setting up a Stack
To get C programs working in our OS we need to set up the C stack. 
To do this we need modify the ```loader.s``` to reserve bytes to be used for the stack, then set the stack pointer the ```esp`` register to the end of the reserved memory. Below is the C code that does this.

*Reserving 4096 bytes for the stack and setting stack pointer*