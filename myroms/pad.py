# P2000 cartridge pad to 16k
# Maarten Pennings 2026 feb 14

import sys
newlen= 16*1024
print( "P2000 cartridge header padder")

if len(sys.argv) != 3 :
  print("SYNATX pad <infile> <outfile>")
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
if len(content)>16384 :
  print("ERROR file too long")
  sys.exit(22)

print(f"pad {newlen-len(content)} bytes")
paddedcontent= content +  b'\xff' * (newlen-len(content))

print(f"write '{outname}'")
with open(outname, 'bw') as file:
  file.write( paddedcontent )

sys.exit(0)
