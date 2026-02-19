all: res/contents.adf

res/contents.ubin: src/contents.asm
	mkdir -p res
	z80asm   src/contents.asm   -o res/contents.ubin

res/contents.bin: res/contents.ubin patch.py
	python3 patch.py   res/contents.ubin   res/contents.bin

res/contents.adf: res/contents.bin adf.py 
	python3 adf.py   res/contents.bin   0   res/contents.adf

clean:
	rm -rf res
