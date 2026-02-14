# P2000 cartridge pad to 16k
# Maarten Pennings 2026 feb 14

import sys
newlen= 16*1024
print( "P2000 cartridge header padder")
print()

if len(sys.argv) != 2 :
  print("ERROR: pass file.bin as argument")
  sys.exit(20)
inname= sys.argv[1]

print("READ")
print( f"  open  {inname}")
with open(inname, 'br') as file:
  content=file.read()
print( f"  size  {len(content)} bytes")
if len(content)<16 :
  print("ERROR file too short")
  sys.exit(21)
if len(content)>16384 :
  print("ERROR file too long")
  sys.exit(21)
print()

print("PAD")
print(f"  add   {newlen-len(content)} bytes")
paddedcontent= content +  b'\xff' * (newlen-len(content))
print()

outname= "padded."+inname
print("WRITE")
print( f"  open  {outname}")
with open(outname, 'bw') as file:
  file.write( paddedcontent )

sys.exit(0)
