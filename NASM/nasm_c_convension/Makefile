ASMBIN=nasm
ASM_SOURCE=write
CPP_SOURCE=main
all: assemble link

assemble: $(ASM_SOURCE).asm $(CPP_SOURCE).cpp
	$(ASMBIN) -g -O0 -f elf32 $(ASM_SOURCE).asm 
	gcc -g -m32 -O0 -mpreferred-stack-boundary=2 -c $(CPP_SOURCE).cpp

link:
	gcc -g -m32 $(CPP_SOURCE).o $(ASM_SOURCE).o -O0 -lm
run:
	./a.out

clean:
	rm *.o
	rm a.out
