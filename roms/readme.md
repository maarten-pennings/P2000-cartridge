# P2000-cartridge - Contents ROM

Partly as a proof of concept, partly for branding and partly for documentation,
I decided to make my own _Contents_ ROM. 

![GitHub actions dashboard](../images/collection.png)


## Introduction

The idea is that the _Contents_ app can operate as a real cartridge.
A ROM chip filled with this app's binary image should make the P2000 boot-loader accept it.
This means the image should start with the correct header, which includes a matching checksum.
Then the boot-loader will start our app.

The _Contents_ ROM would be the first ROM in our multi ROM cartridge.
We keep this app simple. It should list which other ROMs are stored on the multi ROM.
It is written in assembler, but doesn't do much: it is basically an LDIR instruction to 
copy a prerecorded screen (which is part of the ROM) to the P2000 frame buffer.

It should be noted that the P2000 has a frame buffer that holds 80×25 characters.
This is twice as wide as the physical screen, which is 40×25 characters.
With an `OUT` instruction we can instruct the video chip which column starts on the physical display.

It should also be noted that the P2000 only has a "character buffer", not a "color buffer".
Different colors are created by "switch color" characters that display as a space on the screen.
It does mean that just copying the 2000 bytes is enough.

We use the second screen to shows some URLs, e.g. to this repo or to Erix Collectibles.
Pressing any button will toggle the screens.


## Implementation

For the source file, see the [src directory](src/contents.asm).

Notes

- A cartridge is mapped to 0x1000 in the P2000 memory map. The first line is therefore `org 0x1000`.

- Next comes the 16-byte header. 
  See [P2000 System T&M Reference Manual](https://github.com/p2000t/documentation/blob/main/hardware/P2000%20System%20T%20%26%20M%20Reference%20Manual.pdf).
  Section 4.4.2 on the 51st page describes the header.

  | Address | Field     |  Description     |
  |:--------|:----------|:----------------:|
  | 0x1000  | Signature |        5E        |
  | 0x1001  | ROM size  | lo-byte, hi-byte |
  | 0x1003  | Checksum  | lo-byte, hi-byte |
  | 0x1005  | ROM name  |      8 bytes     |
  | 0x100D  | Version   |      1 byte      |
  | 0x100E  | Reserved  |      2 bytes     |
  
- Next comes `main`, the entry point of our app.
  It  copies two screens of data from the ROM image (label `screens`) to the frame buffer at 0x5000.
  
- Next comes the main `loop`.
  It configures the video chip to show the first screen, and after a key-press the second.
  After another key-press back to the first screen.
  Note that the screen switching is done with `OUT (48),0` (respectively `OUT (48),40`). 
  
- Following the main loop are some rudimentary keyboard scan routines.
  
- The assembler file ends with 2000 bytes of string data, representing the two screens.


## Python helpers

There are some Python scripts to process bin files.


### Patch

To compute the checksum for any P2000 ROM image, we have written a python program [`patch`](patch.py).
This reads a `.bin` file - for example ours as produced by the assembler - computes the checksum, and 
overwrites the associated bytes in the header.

The `patch` script assumes the whole file should be part of the checksum. We have noticed that 
commercial ROMs in the field sometimes specify a shorter ROM size (bytes at 0x1001/0x1002).
It is even possible to use a size of 0 (and set the checksum at 0).


### Pad

The generated bin file should not exceed 16 k (the P2000 doesn't allow more).
Typically they are shorter.

Since we want to make a multi ROM, we need to concatenate 8 ROMs into one big one 
and offer that to the flash programmer.

The Python script [`pad`](pad.py) pads a `.bin` file with 0xFF bytes to be exactly 16 k bytes.
It is your choice whether to first run `patch` and then `pad` or the other way around.
The first one boots faster, in the second one, all padding bytes are 
included in the checksum.

> The [ROM](https://ww1.microchip.com/downloads/aemDocuments/documents/MPD/ProductDocuments/DataSheets/SST39SF010A-SST39SF020A-SST39SF040-Data-Sheet-DS20005022.pdf)
> we have chosen can be erased per sector of 4 k bytes. One P2000 ROM exactly matches 4 sectors.
> Furthermore, our [flasher](../flasher) supports erasing individual sectors and it suppoprts
> writing individual sectors (bytes actually).
> As a result, when using our proprietary flasher, there is no need to use `pad`.


### AFP

We have written a proprietary flasher, called the AFP or [Arduino Flash Programmer](../flasher).
AFP is a tool made from an Arduino sketch and a Nano; this tool burns flash memories.
AFP has a user interface via the virtual COM port (over USB).
This interface is textual, there is a `read` command, an `erase` command and a `write` command.

The Python [`afp`](afp.py) script converts a `.bin` file to a `.afp` file, a text file with 
commands for the AFP tool. The generated `.afp` file contains one `erase` command for the area 
the binary is mapped to, and one `write` command with (up to) 16 k bytes of data for that area.

The `.afp` file can be sent via the virtual COM port in the AFP tool.


### Burn

For each ROM an `.afp` file is generated. This is a text file, which can be sent 
via the virtual COM port to the AFP tool. The problem when sending such an `.afp` file 
to the AFP tool is speed difference: the sender (the PC) is much faster than the micro controller 
in the AFP tool (Nano). The result is that the AFP tool can't keep up, commands get scrambled or lost.

One solution is to use a terminal tool that not only can sent text files, but which can also be
throttled by setting a line delay (of say 10ms per line).

Another solution is to only send the next line when the previous is executed.
The AFP tool signals this by sending a prompt (`>>`) back.

The Python [`burn`](burn.py) script does just that. It sends `.afp` files to the AFP tool, line by line, 
each time waiting for the prompt.


## Makefile

There is a [Makefile](Makefile) that compiles the [assembly source](src/contents.asm) to an
unpatched bin file (`.ubin`). Next the Python `patch` script is run. It patches the 
checksum and generates a `.bin` file. You can run this in the [emulator](https://github.com/p2000t/M2000).

The makefile also generates the `.afp` files that can be sent to the 
[Arduino Flash programmer](../flasher). Not only for the `contents.bin`, but also
for the other seven [stock roms](stockroms).

I develop on Windows, but use WSL to install the Z80 assembler.
This repo also has a workflow to do this every Git push, see the 
[workflow](../.github/workflows/build_contents.yml).

On the [GitHub actions dashboard](https://github.com/maarten-pennings/P2000-cartridge/actions) 
you see every run. Click one, and on the next screen at the bottom, download the build results called "roms.zip".

![GitHub actions dashboard](../images/github-actions.png)

It contains the compiled contents (.bin). It also contains the .afp files of all rom; from contents.bin,
but also from all stock roms.

(end)

