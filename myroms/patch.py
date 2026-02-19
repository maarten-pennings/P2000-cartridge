# P2000 cartridge: patching the initial checksum in the header
# Maarten Pennings 2026 feb 14

import sys
print("P2000 cartridge header patcher")

if len(sys.argv) != 3 :
  print("SYNATX patch <infile> <outfile>")
  sys.exit(20)
inname= sys.argv[1]
outname= sys.argv[2]

print(f"read '{inname}'",end="")
with open(inname, 'br') as file:
  content=file.read()
print(f" {len(content)} bytes")
if len(content)<16 :
  print("ERROR file too short")
  sys.exit(21)

csum=0
for c in content[5:] : csum+=c
print(f"patch count {(len(content)-5)%0x100:02X} {(len(content)-5)//0x100:02X}")
insumb= 0x10000 - (csum%0x10000)
print(f"patch insum {insumb%0x100:02X} {insumb//0x100:02X}")
patchedcontent= content[:1] + bytes([(len(content)-5)%0x100,(len(content)-5)//0x100,insumb%0x100,insumb//0x100]) + content[5:]

print(f"write '{outname}'")
with open(outname, 'bw') as file:
  file.write( patchedcontent )

sys.exit(0)
