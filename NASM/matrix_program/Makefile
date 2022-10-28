ASMBIN=nasm
ASM_SOURCE=lr4
all: assemble link

assemble: $(ASM_SOURCE).asm
	$(ASMBIN) -g -f elf $(ASM_SOURCE).asm -l $(ASM_SOURCE).lst

link:
	ld -m elf_i386 $(ASM_SOURCE).o -o $(ASM_SOURCE)

clean:
	rm *.o
	rm $(ASM_SOURCE).lst
	rm $(ASM_SOURCE)
