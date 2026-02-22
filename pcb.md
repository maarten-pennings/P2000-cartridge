# P2000-cartridge - PCB design

With the analysis phase done, let's make the PCB.
Design it, manufacture it, assemble it, and test it.

 
## Schematics

I designed the PCB schematics in [EasyEda](https://easyeda.com).
They are available in [pdf](pcb/P2000Cartridge-schem.pdf) and 
as EasyEda [json](pcb/P2000Cartridge-schem.json) source.


## Layout

Also the PCB layout I did in [EasyEda](https://easyeda.com).
The layout is available as two pdfs [front](pcb/P2000Cartridge-pcb-front.pdf) and [back](pcb/P2000Cartridge-pcb-back.pdf),
and also as EasyEda [json](pcb/P2000Cartridge-pcb.json) source.


I double checked the socket since I don't have it yet physically.
Warning, the socket diagram is the pin-out from the front side of the PCB ("bottom view").

![socket](pcb/PLCC32-pinout.png) 


## Gerber

For ordering at [jlcpcb](https://jlcpcb.com) I created a gerber file.
The final [gerber](pcb/P2000Cartridge-gerber.zip) is also available in this repo.

The render of the PCB.

![PCB render](pcb/P2000Cartridge-render.png)


## Bare PCBs

I ordered at 2026-02-09 and 2026-02-19 the PCBs arrived at my home.
In 10 days, that is not bad.

![PCB bare](images/pcb-bare.jpg)

I did a quick "continuity" test; are all pads going where I think they need to go.

For a moment I believed I mirrored the flash socket.
That was due to the diagram from when I checked the [layout](#layout).
That diagram is mirrored, since I checked it in EasyEda with the fron side visible.
Now I was measuring at the backside.


## Assembly

Next step is assembly. That was easy. Just one socket, one DIL IC, 
2 capacitors and 5 resistors. I do not yet have the 5-way dipswitch.
This means the flash chip is configured for "000".


![PCB assembled](images/pcb-assembled.jpg)


## Possible improvements


I probably should
- have put a pin-one marking on the PCB (for the flash socket);
- placed the flash socket even higher to make it fit in a standard P2000 cartridge (the socket is the thickest, up in the cartrdige there is more depth).

I am still bothered that the dip-switch footprint has bit 1 in the right-most 
position instead of the left-most: I want my bits big-endian.


(end)
