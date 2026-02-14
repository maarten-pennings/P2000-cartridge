all: res/contents.bin

res/contents.bin: res/contents.sbin
	python3 pad.py   res/contents.sbin   res/contents.bin

res/contents.sbin: res/contents.subin
	python3 patch.py   res/contents.subin   res/contents.sbin

res/contents.subin: src/contents.asm
	mkdir -p res
	z80asm   src/contents.asm   -o res/contents.subin

clean:
	rm -rf res
