# AFP - Arduino Flash Programmer

Making a tool to program ("burn") a flash memory.
We use an Arduino Nano with two shift registers.


## Prototype

The concept

![AFP schematics](images/flasher.pcb.drawio.png)

testing on a breadboard

![AFP on breadboard](images/breadboard.jpg)

It appears we need a tool to get the flasg chip out of its socket.

![PLCC IC pull tool](images/plcc-pull-tool.png)


## AFP PCB 

All files are stored in the [pcb](pcb) directory.


### Schematics

I designed the PCB schematics in [EasyEda](https://easyeda.com).
They are available in [pdf](pcb/AFP-schem.pdf) and 
as EasyEda [json](pcb/AFP-schem.json) source.


### Layout

Also the PCB layout I did in [EasyEda](https://easyeda.com).
The layout is available as two pdfs [front](pcb/AFP-pcb-front.pdf) and [back](pcb/AFP-pcb-back.pdf),
and also as EasyEda [json](pcb/AFP-pcb.json) source.


### Gerber

For ordering at [jlcpcb](https://jlcpcb.com) I created a gerber file.
The final [gerber](pcb/AFP-gerber.zip) is also available in this repo.

The render of the PCB.

![PCB render](pcb/AFP-render.png)


### Bare PCBs

I ordered at 2026-02-22 and 2026-02-xx the PCBs arrived at my home.

... todo ...



## AFP firmware

The [AFP firmware](afp) shall be compiled with the 
[Arduino IDE](https://www.arduino.cc/en/software)
and flashed to the Nano.


(end)

