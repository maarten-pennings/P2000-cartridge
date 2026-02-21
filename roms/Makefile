roms: myroms stockroms

myroms: generated/contents.afp

stockroms: generated/BASICNL1.1.afp  generated/Forth.afp  generated/UCSD-Pascal.afp  generated/assembler-5.9.afp  generated/JWSBasic.afp  generated/ECOL3.0.afp  generated/Maintenance-2.afp 



generated/contents.ubin: myroms/contents.asm
	mkdir -p generated
	z80asm   myroms/contents.asm   -o generated/contents.ubin

generated/contents.bin: generated/contents.ubin patch.py  patch.py
	python3 patch.py   generated/contents.ubin   generated/contents.bin

generated/contents.afp: generated/contents.bin afp.py  afp.py
	python3 afp.py   generated/contents.bin            0   generated/0-contents.afp



generated/BASICNL1.1.afp: stockroms/BASICNL1.1.bin  afp.py
	mkdir -p generated
	python3 afp.py   stockroms/BASICNL1.1.bin          1   generated/1-BASICNL1.1.afp
  
generated/Forth.afp: stockroms/Forth.bin  afp.py
	mkdir -p generated
	python3 afp.py   stockroms/Forth.bin               2   generated/2-Forth.afp
  
generated/UCSD-Pascal.afp: stockroms/UCSD-Pascal.bin  afp.py
	mkdir -p generated
	python3 afp.py   stockroms/UCSD-Pascal.bin         3   generated/3-UCSD-Pascal.afp

generated/assembler-5.9.afp: stockroms/assembler-5.9.bin  afp.py
	mkdir -p generated
	python3 afp.py   stockroms/assembler-5.9.bin       4   generated/4-assembler-5.9.afp

generated/JWSBasic.afp: stockroms/JWSBasic.bin  afp.py
	mkdir -p generated
	python3 afp.py   stockroms/JWSBasic.bin            5   generated/5-JWSBasic.afp
  
generated/ECOL3.0.afp: stockroms/ECOL3.0.bin  afp.py
	mkdir -p generated
	python3 afp.py   stockroms/ECOL3.0.bin             6   generated/6-ECOL3.0.afp

generated/Maintenance-2.afp: stockroms/Maintenance-2.bin  afp.py
	mkdir -p generated
	python3 afp.py   stockroms/Maintenance-2.bin       7   generated/7-Maintenance-2.afp



clean:
	rm -rf generated
