all: build

build:
	 nasm -f elf64 kernel.asm -o kernel.o
	 ld -o start kernel.o

clean:
	rm *.o
	rm start