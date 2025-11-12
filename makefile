all: clean build run clean

build:
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c kmain.c -o kmain.o
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c ./drivers/io.c -o ./drivers/io.o
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c ./gdt/gdt.c -o ./gdt/gdt.o
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c ./idt/idt.c -o ./idt/idt.o
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c ./idt/pic.c -o ./idt/pic.o
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c ./drivers/keyboard.c -o ./drivers/keyboard.o

	nasm -f elf loader.s -o loader.o
	nasm -f elf ./drivers/io.s -o ./drivers/io_asm.o
	nasm -f elf ./gdt/gdt_asm.s -o ./gdt/gdt_asm.o
	nasm -f elf ./idt/idt_asm.s -o ./idt/idt_asm.o

	ld -T ./source/link.ld -melf_i386 loader.o kmain.o ./drivers/io.o ./drivers/io_asm.o ./gdt/gdt.o ./gdt/gdt_asm.o ./idt/idt.o ./idt/idt_asm.o ./drivers/keyboard.o ./idt/pic.o -o ./iso/boot/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o ./iso/os.iso iso

run:
	qemu-system-i386 -display curses -monitor telnet::45455,server,nowait -serial file:seriallog.txt -boot d -cdrom ./iso/os.iso -m 32 -d cpu -D logQ.txt -device isa-debug-exit,iobase=0xf4 || true
	clear
clean:
	rm -f *.o
	rm -f ./iso/os.iso
	rm -f iso/boot/kernel.elf
	rm -f logQ.txt
	rm -f seriallog.txt
	rm -f ./drivers/*.o
	rm -f ./gdt/*.o
	rm -f ./idt/*.o
	clear
