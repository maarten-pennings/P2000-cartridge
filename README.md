# P2000-cartridge

Making a cartridge for the Philips P2000T.

![PCB render](images/cartridge.jpg)


## Introduction

Since I retrieved an old [Philips P2000](https://github.com/maarten-pennings/P2000) from my attic I wanted to make a cartridge myself. 
I help repairing retro computers at [Erix Collectables](https://www.erixcollectables.nl/), and also there we planned 
making a cartridge to facilitate testing and bringing up P2000 mods.


## Analysis

I did some [analysis](analysis.md) coming to the following conclusions:

- Ivo Filot ([Github](https://github.com/ifilot) or [blog](https://philips-p2000t.nl/)) uses
  the [SST39SF0x0](SST39SF010A-SST39SF020A-SST39SF040-Data-Sheet-DS20005022.pdf), 
  it is one of the cheaper solutions, and we have a programmer for it. 
  We will also pick this memory.
- Let's make a modest multi-rom: SST39SF010 - 1 Mbit or 128 kbyte or 8 ROMs. 
  But let's prepare the PCB that they work for the bigger chips too.
- Use PLCC socket; it is smaller than DIL/DIP.
- No reset switch - we don't need it.


## Design

With the analysis phase done, let's make the PCB.
Design it, manufacture it, test track connections, assemble it, and test functionality.
 
![PCB assembled](images/pcb-assembled.jpg)
 
Find all design files (schematics, layout, gerber) in the [pcb](pcb) directory.


## Which ROMs?

Which ROMs to put on the multi-ROM cartridge?
The chosen flash chip 39SF010 has room for 8.


### Stock cartridges

Likely cartridge images are:

- A self-made table of contents.
- [BASIC NL 1.1](https://github.com/p2000t/software/blob/main/cartridges/BASICNL1.1.bin) because I only have an UK 1.0 cartridge.
- [Forth](https://github.com/p2000t/software/blob/main/cartridges/Forth.bin)
- [UCSD Pascal](https://github.com/p2000t/software/blob/main/cartridges/UCSD%20Pascal.bin)
- [Assembler](https://github.com/p2000t/software/blob/main/cartridges/assembler%205.9.bin)
- [JWS Disk BASIC](https://github.com/p2000t/software/blob/main/cartridges/JWSBasic.bin) because we found a P2000 with built-in JWS drive.
  It seems this was developed by Jan-Willem Sparnaaij to enable floppy disks instead of the cassette recorder in BASIC.
- [ECOL](https://github.com/p2000t/software/blob/main/cartridges/ECOL3.0.bin), the Educative COmputer Language.
- [Maintenance 2](https://github.com/p2000t/software/blob/main/cartridges/Maintenance%202.bin), a test cartridge.

List of candidate cartridges can be found on the 
[P2000T Preservation Project](https://github.com/p2000t/software/tree/main/cartridges).

The ones listed here are ["backed up"](roms/stockroms) in this repo.


### Own cartridge: Contents

The proposal is that the first ROM is a sort of table of contents.
I made the [Contents](roms) ROM myself. 
It is written in assembler, but rather simple: just an LDIR to copy a screen image to the frame buffer.

![Screenshot Collection](images/collection.png)


### Makefile

The [roms](roms) directory contains a sub directory with the [stockroms](roms/stockroms) and a sub directory
with [myroms](roms/myroms) (the Contents rom). It also contains a [Makefile](roms/Makefile) to compile 
myroms to a `.bin` and convert all `.bin` files (from myroms and from stockroms) to flash scripts (`.afp`).


## Cost 

Overview of the [cost](cost.md).


(end)

