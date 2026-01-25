# P2000-cartridge

Making a cartridge for the Philips P2000T.


## Introduction

Since I [retrieved]((https://github.com/maarten-pennings/P2000)) an old Philips P2000 from my attic
I wanted to make a cartridge myself. I help repairing retro computers, and also there we planned 
making a cartridge to facilitate testing and bringing up P2000 mods.


## Which ROMs?

Likely cartridge images 
- [JWS Disk BASIC](https://github.com/p2000t/software/blob/main/cartridges/JWSBasic.bin) because we found a P2000 with built-in JWS drive.
  It seems this was developed by Jan-Willem Sparnaaij to enable floppy disks instead of the cassette recorder in BASIC.
- A test cartridge like [Maintenance](https://github.com/p2000t/software/blob/main/cartridges/Maintenance%202.bin).
- [BASIC NL 1.1](https://github.com/p2000t/software/blob/main/cartridges/BASICNL1.1.bin) because I only have an UK 1.0 cartridge.

**todo**: make a list of candidate cartridges; URL needed, eg from [P2000T Preservation Project](https://github.com/p2000t/software/tree/main/cartridges)


## Design

I see these design choices.

### Make a single or multi-rom cartridge?

A cartridge either occupies 8 kB or 16 kB in the memory map of the P2000 (see map below).
Modern EEPROMs are typically much bigger, for example an 512 kB EEPROM could store 32 cartridge images of each 16 kB.
The alternative is using a smaller ROM or simply not using the extra memory in the EEPROM.

Technically a multi-rom sounds interesting. Down-sides include: not very retro-like,
not-so robust dip switches needed, need sticker with e.g. 32 entries, don't switch while powered,
do we have enough ROMs to make sense in one EEPROM.

There is a great [GitHiub](https://github.com/ifilot) account from Ivo Filot showing both 
options.

- ["simple" ie single](https://github.com/ifilot/p2000t-simple-cartridge)

- [Several multi](https://github.com/ifilot/p2000t-cartridges)

![P2000T Memory map](images/memorymap.drawio.png)


### Which EEPROM chip to use?

Again, looking at Ivo's GitHub, there are two likely candidates. 

- The SST39SFxxx series.  

  | Type       |  bit |     byte      | ROMs | dip switches | link    |
  |:-----------|-----:|:-------------:|-----:|:------------:|:--------|
  | SST39SF010 | 1 Mb | ⅛ MB = 128 kB |    8 |      3       |[🔗](https://ww1.microchip.com/downloads/aemDocuments/documents/MPD/ProductDocuments/DataSheets/SST39SF010A-SST39SF020A-SST39SF040-Data-Sheet-DS20005022.pdf) |
  | SST39SF020 | 2 Mb | ¼ MB = 256 kB |   16 |      4       | idem    |
  | SST39SF040 | 4 Mb | ½ MB = 512 kB |   32 |      5       | idem    |
  
  
- The W27Cxxx or W28Cxxx series.

  The W27Cxxx seems to be for EPROMs (one E), they are erased by exposure to a UV (ultraviolet) light. Programming typically requires a higher voltage supply (e.g., 12.5V or 13V Vpp).
  The W28Cxxx seems to be for EEPROMs (two Es), they are electrically erasable and programmable.
  However I have an 27C256 without a "window", so I'm not sure about this.
  
  I have experience with the W28C64, Ivo [mentions](https://github.com/ifilot/p2000t-cartridges?tab=readme-ov-file#4-x-16kib-tht-w27c512-64-kib-rom) the w27c512.

  | Type       |    bit |    byte | ROMs | dip switches | link    |
  |:-----------|-------:|:-------:|-----:|:------------:|:--------|
  | W28C64     |  64 kb |    8 kB |    ½ |    -1   😊   | [🔗](https://ww1.microchip.com/downloads/en/devicedoc/11003L.pdf) |
  | W28C128    | 128 kb |   16 kB |    1 |     0        | [🔗](https://ww1.microchip.com/downloads/aemDocuments/documents/MPD/ProductDocuments/DataSheets/AT28C256-Industrial-Grade-256-Kbit-Paged-Parallel-EEPROM-Data-Sheet-DS20006386.pdf) |
  | W27C256    | 256 kb |   32 kB |    2 |     1        | [🔗](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/11001N.pdf) |
  | W27C512    | 512 kb |   64 kB |    4 |     2        | [🔗](https://ww1.microchip.com/downloads/en/DeviceDoc/doc0015.pdf) |

  
### Which EEPROM package?

Both chips come in two packages.

- PLCC stands for Plastic Leaded Chip Carrier.

  ![PLCC](images/PLCC-27C256.jpg)

- DIL or DIP stands for Dual In-line (or Dual In-line Package/Packaging).

  ![DIL](images/DIL-28C64.jpg)
  
There is a slight height difference of 0.6mm when socketed:
the PLCC is 7.8mm and the DIL is 8.4mmm.

![Socketed height](images/socketed-height.jpg)


### With or without socket?

With socket allows us to reflash - e.g. when a first flash somehow failed.
Of course we could have one cartridge with socket ("for development") and solder 
all the rest (for "use").

Why is this even a question?
Not the extra money for the socket, but the increased height: 
the P2000 cartridges do not leave room for sockets:
"the PLCC is 7.8mm and the DIL is 8.4mmm", both more than 7.2.

![Cartridge dimensions](images/cartridge-dimensions.drawio.png)

We could put the socketed chip in the top of the cartridge.
See here some photos of a mockup.

![Mockup PCB ](images/mockup-pcb-1.jpg)

![Mockup PCB ](images/mockup-pcb-2.jpg)

![Mockup PCB ](images/mockup-pcb-5.jpg)

![Mockup PCB ](images/mockup-pcb-3.jpg)

![Mockup PCB ](images/mockup-pcb-4.jpg)


### How to program the EEPROM?

I do have a self-made [programmer](https://github.com/maarten-pennings/6502/tree/master/3eeprom#3a2-the-programmer).
Unfortunately, it only supports 28C16 and 28C64; we would now need at least one notch up: 28C128.

I do not yet have an overview of good programmers, there might be one available with a colleague.

**todo**: check availability EEPROM programmer.


### Reset switch?

Do we want a reset switch? Quite complicated schematics.


### Schematics

When we look at Ivo's [27C512](https://github.com/ifilot/p2000t-cartridges?tab=readme-ov-file#4-x-16kib-tht-w27c512-64-kib-rom) solution
it is very similar to the SST39SF040  solution.

![Schematics](images/schematic-ivo-27C512.png)

Source: [click sch file in right pane](https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2Fifilot%2Fp2000t-cartridges%2Ftree%2Fmaster%2Fmulticartridge-w27c512%2Fpcb)

Contents of this design:

- Flash chip with a capacitor.
- Cartridge edge connector.
- Dip switch controlling A14 and A15.
- Address decoding with one quad NAND (with cap).

Explanation of the design:

- Flash chip routes D0..D7 to edge connector.
- Flash chip routes A0..A11 to edge connector.
- Flash chip routes A12 and A13 to address decoding.
- Flash chip routes A14 and A15 to dip switches.

The hardest part is the address decoding.
But I happen to have investigated that [earlier](https://github.com/maarten-pennings/P2000/blob/main/readme.md#addressing).


### Proposal

- I have a slight preference for a single ROM (easy), but a modest multi-rom (4 or 8 ROMs) is also fine.
- Have a slight preference for the 28C512 because I see myself expanding my programmer. 
  If we find/have a programmer for SST39SF010, that is also fine.
- Have a slight preference for the DIL, just because it is familiar. 
  It is taller than the PLCC solution though.
- I want to go for with-socket; maybe move to socket to the top.
- Still need to find the programmer.
- No reset switch - don't need it.


### Cost 

First quick cost estimate (we can save by buying fewer pieces, but the shipment kicks in).
- 28F512 come at [€18](https://nl.aliexpress.com/item/1005009302628487.html) for 10 (with shipping).
  Not sure about the `F` in the type name yet.
- 28C256 is more expensive [€49](https://nl.aliexpress.com/item/1005005830753898.html) for 10 (with shipping).
- The SST39SF010 is a better deal [€13](https://nl.aliexpress.com/item/1005008535405486.html) for 20 pieces (with shipping).

- PLCC sockets seem to be available at [€12](https://nl.aliexpress.com/item/1005006065584831.html) for 50 (with shipping).
- DIP sockets seem a slightly cheaper [€12](https://nl.aliexpress.com/item/1005007090221911.html) for 60 (with shipping).

- Quad NAND is [€12](https://nl.aliexpress.com/item/1005002328900896.html) for 50 pieces (with shipping).
- Dip switches is [€10](https://nl.aliexpress.com/item/4001205849246.html) for 50 pieces (with shipping).

- 100 pieces [10k resistors ](https://nl.aliexpress.com/item/32952657927.html)
- 100 pieces [0.1 uF capacitors](https://nl.aliexpress.com/item/1005010462657465.html)

- PCBs at [JLCPCB](jlcpcb.com) cost €2 euros for 5 pieces (if we stay below 10×10cm²)
  going up to €12 for 10 pieces (15×7cm²) plus €10 for shipping.

Here a quick shopping cart, now going for 10 pieces, still need to add €25 for PCBs, 
and we need to solve the EEPROM burner.

![Shopping cart](images/shopping-cart.png)


(end)

