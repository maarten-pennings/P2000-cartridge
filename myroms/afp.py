# P2000 cartridge: convert bin file (from assembler) to afp file for (proprietary) flasher
# Maarten Pennings 2026 feb 19

import sys
from datetime import datetime
print("P2000 cartridge bin-to-afp")

if len(sys.argv) != 4 :
  print("SYNATX patch <infile> <rom#> <outfile>")
  sys.exit(20)
inname= sys.argv[1]
rom= int(sys.argv[2])
outname= sys.argv[3]

if rom<0 or rom>31:
  print("<rom#> must be 0..31")
  edit(21)
  
print(f"read  '{inname}'")
with open(inname, 'br') as file:
  content=file.read()

print(f"write '{outname}'")
lines=[]
lines.append("@echo disable\n")
lines.append(f"// suggest line delay 10ms\n")
lines.append(f"// source {inname}\n")
lines.append(f"// target {outname}\n")
lines.append(f"// date   {datetime.now()}\n")
lines.append(f"\n")
lines.append(f"write stats clear\n")
lines.append(f"erase R{rom}\n")
lines.append(f"// wait for erase\n")
lines.append(f"\n") #  10ms
lines.append(f"\n") #  20ms
lines.append(f"\n") #  30ms
lines.append(f"\n") #  40ms
lines.append(f"\n") #  50ms
lines.append(f"\n") #  60ms
lines.append(f"\n") #  70ms
lines.append(f"\n") #  80ms
lines.append(f"\n") #  90ms
lines.append(f"\n") # 100ms
lines.append(f"write R{rom} *\n")
s=""
for ix,byte in enumerate(content) :
  s += f"{byte:02x} "
  if ix % 16 == 7 : 
    s += " "
  if ix % 16 == 15 : 
    s += "\n"
    lines.append( s )
    s=""
s += "*\n"
lines.append( s )
lines.append(f"write stats\n")
lines.append("echo faults\n")
lines.append("echo enable\n")
with open(outname, 'w') as file:
  file.writelines( lines)

sys.exit(0)
