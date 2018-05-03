all: patch

patch:  start.o lab4_util.o  task1.o 
	ld -m elf_x86_64 start.o task1.o lab4_util.o -o patch
	
lab4_util.o: lab4_util.c
	gcc -m64 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o lab4_util.o
	
task1.o: task1.c
	gcc -m64 -Wall -ansi -c -nostdlib -fno-stack-protector task1.c -o task1.o

start.o: start.s
	nasm -f elf64 start.s -o start.o

.PHONY: clean


clean: 
	rm -f *.o patch 
