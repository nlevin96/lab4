all: search

search:  start.o lab4_util.o  task2c.o 
	ld -m elf_x86_64 start.o task2c.o lab4_util.o -o search
	
lab4_util.o: lab4_util.c
	gcc -m64 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o lab4_util.o
	
task2c.o: task2c.c
	gcc -m64 -Wall -ansi -c -nostdlib -fno-stack-protector task2c.c -o task2c.o

start.o: start.s
	nasm -f elf64 start.s -o start.o

.PHONY: clean


clean: 
	rm -f *.o search 
