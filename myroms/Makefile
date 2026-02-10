all: 000contents.bin

000contents.bin: 000contents/000contents.asm
	z80asm   000contents/000contents.asm   -o 000contents.bin

clean:
	rm -rf 000contents.bin
