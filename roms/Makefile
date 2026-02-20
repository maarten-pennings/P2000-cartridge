roms: myroms stockroms

myroms: res/contents.afp

stockroms: res/BASICNL1.1.afp  res/Forth.afp  res/UCSD-Pascal.afp  res/assembler-5.9.afp  res/JWSBasic.afp  res/familiegeheugen-4.afp  res/Maintenance-2.afp 



res/contents.ubin: myroms/contents.asm
	mkdir -p res
	z80asm   myroms/contents.asm   -o res/contents.ubin

res/contents.bin: res/contents.ubin patch.py  patch.py
	python3 patch.py   res/contents.ubin   res/contents.bin

res/contents.afp: res/contents.bin afp.py  afp.py
	python3 afp.py   res/contents.bin                  0   res/contents.afp



res/BASICNL1.1.afp: stockroms/BASICNL1.1.bin  afp.py
	mkdir -p res
	python3 afp.py   stockroms/BASICNL1.1.bin          1   res/BASICNL1.1.afp
  
res/Forth.afp: stockroms/Forth.bin  afp.py
	mkdir -p res
	python3 afp.py   stockroms/Forth.bin               2   res/Forth.afp
  
res/UCSD-Pascal.afp: stockroms/UCSD-Pascal.bin  afp.py
	mkdir -p res
	python3 afp.py   stockroms/UCSD-Pascal.bin         3   res/UCSD-Pascal.afp

res/assembler-5.9.afp: stockroms/assembler-5.9.bin  afp.py
	mkdir -p res
	python3 afp.py   stockroms/assembler-5.9.bin       4   res/assembler-5.9.afp

res/JWSBasic.afp: stockroms/JWSBasic.bin  afp.py
	mkdir -p res
	python3 afp.py   stockroms/JWSBasic.bin            5   res/JWSBasic.afp
  
res/familiegeheugen-4.afp: stockroms/familiegeheugen-4.bin  afp.py
	mkdir -p res
	python3 afp.py   stockroms/familiegeheugen-4.bin   6   res/familiegeheugen-4.afp

res/Maintenance-2.afp: stockroms/Maintenance-2.bin  afp.py
	mkdir -p res
	python3 afp.py   stockroms/Maintenance-2.bin       7   res/Maintenance-2.afp



clean:
	rm -rf res
