all: res/contents.afp

res/contents.ubin: src/contents.asm
	mkdir -p res
	z80asm   src/contents.asm   -o res/contents.ubin

res/contents.bin: res/contents.ubin patch.py
	python3 patch.py   res/contents.ubin   res/contents.bin

res/contents.afp: res/contents.bin afp.py 
	python3 afp.py   res/contents.bin   0   res/contents.afp

clean:
	rm -rf res
