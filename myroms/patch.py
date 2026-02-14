# P2000 cartridge header patching the initial checksum
# Maarten Pennings 2026 feb 14

import sys
print( "P2000 cartridge header patcher")
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
print()

print("PATCH")
csum=0
for c in content[5:] : csum+=c
print( f"  count needs to be {len(content)-5:04X} or {(len(content)-5)%0x100:02X} {(len(content)-5)//0x100:02X}")
insumb= 0x10000 - (csum%0x10000)
print( f"  insum needs to be {insumb:04X} or {insumb%0x100:02X} {insumb//0x100:02X}")
patchedcontent= content[:1] + bytes([(len(content)-5)%0x100,(len(content)-5)//0x100,insumb%0x100,insumb//0x100]) + content[5:]
print()

outname= "patched."+inname
print("WRITE")
print( f"  open  {outname}")
with open(outname, 'bw') as file:
  file.write( patchedcontent )

sys.exit(0)
