# P2000 cartridge header analysis
# Maarten Pennings 2026 feb 12

import sys
print( "P2000 cartridge header analysis")
print()

if len(sys.argv) != 2 :
  print("ERROR: pass file.bin as argument")
  sys.exit(20)
inname= sys.argv[1]

print("FILE")
print( f"  open  {inname}")
with open(inname, 'br') as file:
  content=file.read()
print( f"  size  {len(content)} bytes")
if len(content)<15 :
    print("ERROR file to short")
    sys.exit(21)
head=""
for i,c in enumerate(content[0:16]) :
  head+=f"{c:02X} "
  if i in [0,2,4,12,13] : head+="| "
print( f"  head  {head}")
print()

print("HEADER")
sig= content[0]
print( f"  sig   {sig:02X}")
count= content[1]+256*content[2]
print( f"  count {count:04X} ({count} bytes)")
insum= content[3]+256*content[4]
print( f"  insum {insum:04X}")
cname=""
for c in content[5:5+8]:
  cname+= chr(c) if 32<=c and c<=127 else "."
print( f"  cname '{cname}'")
rel= content[13]
print( f"  rel   {rel:02X}")
resvd= content[14]+256*content[15]
print( f"  resvd {resvd:04X}")
print()

error = 0
print("CHECKSUM")
if 0x0005+count > len(content) :
  print("ERROR: checksum count in header exceeds file length")
  sys.exit(22)
missing= len(content)-(0x0005+count)
csum= insum
for c in content[5:5+count] : csum+= c
csum16a = csum % 0x10000
print( f"  csum  {0x1005:04X}-{0x1005+count-1:04X} = {csum16a:04X}")
if csum16a==0 :
  print( f"  csum  ok (i.e. insum ok)")
else :    
  insuma= 0x10000 - ((csum-insum)%0x10000)
  print( f"  csum  ERROR")
  error=1
  print( f"  insum needs to be {insuma:04X} or {insuma%0x100:02X} {insuma//0x100:02X}")

if missing>0 :
  print()
  print( f"CHECKSUM" )
  print( f"  WARNING checksum skips {missing} bytes" )
  print( f"  WARNING use these settings for full-file checksum" )
  for c in content[5+count:len(content)] : csum+= c
  csum16b = csum % 0x10000
  print( f"  csum  {0x1005:04X}-{0x1000+len(content):04X} = {csum16b:04X}")
  print( f"  count needs to be {len(content)-5:04X} or {(len(content)-5)%0x100:02X} {(len(content)-5)//0x100:02X}")
  insumb= 0x10000 - ((csum-insum)%0x10000)
  print( f"  insum needs to be {insumb:04X} or {insumb%0x100:02X} {insumb//0x100:02X}")

print()
print( f"REPORT" )
if error!=0 :
  print("  MUST FIX `insum` needs update (leads to checksum error)")
if missing>0 :
  print("  MAY  FIX checksum does not cover complete file (enlarge `count`)")
if error==0 and missing==0 :
  print("  ok")

sys.exit(error)

