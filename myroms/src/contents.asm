; "contents.asm"
; -----------------------------------------------------------
    org 0x1000


; -----------------------------------------------------------
header:
    ; signature
    DB 0x5E

    ;  byte count and checksum (count=0 results in checksum=0)
    DB 0x00,0x00, 0x00,0x00

    ; name of the cartridge (8 bytes)
    DB "MPEN2026"
    
    ; revision
    DB 0x00
    
    ; Reserved
    DB 0x00,0x00


; -----------------------------------------------------------
main:
    ; copy rom image (two screens) to frame buffer
    LD BC,2*40*25
    LD HL,screens  ; rom image
    LD DE,0x5000   ; frame buffer
    LDIR
    
loop:
    ; switch to left-most screen
    LD A,0
    OUT (48),A
    CALL waitforkey
    
    ; switch to right-most screen
    LD A,40
    OUT (48),A
    CALL waitforkey
    
    JP loop

  
; -----------------------------------------------------------
; Wait a bit 
; Destroys: B, C
delay:
    LD B,80
delay1:
    LD C,0
delay2:
    DEC C
    JP NZ,delay2
    DEC B
    JP NZ,delay1
    RET
    

; -----------------------------------------------------------
; Spins until a key is pressed, then returns
; Destroys: A, B, C
waitforkey:
    CALL checkkbd
    JP Z,waitforkey
    CALL delay ; debounce
    RET

    
; -----------------------------------------------------------
; Scans P2000 keyboard ports 0x00-0x09
; Returns: NZ iff key press, Z if no press
; Destroys: A, B, C
checkkbd:
    LD B, 10            ; number of ports to scan
    LD C, 0             ; first port number

kbdloop:
    IN A,(C)            ; read the keyboard row
    CP 0xFF             ; is it 0xFF? (0 bit indicates pressed key)
    JP NZ, kbdret       ; goto kbdret with NZ (key press)
    
    INC C
    DEC B
    JP NZ, kbdloop
    ; goto kbdret with Z (no press)
    
kbdret:
    RET    

    
; -----------------------------------------------------------
; data for 2 screens of 25 rows of 40 columns
screens:
    ;  0 000 ABK alpha-black
    ;  1 001 ANR alpha-red
    ;  2 002 ANG alpha-green
    ;  3 003 ANY alpha-yelloW
    ;  4 004 ANB alpha-blue
    ;  5 005 ANM alpha-magenta
    ;  6 006 ANC alpha-cyan
    ;  7 007 ANW alpha-white
    ; 13 015 DBH double height
    ; 19 023 MSY mosaic yellow
    ;   01234567890123456789012345678901234567890123456789012345678901234567890123456789
    DB "                                                                                " ;  0
    DB "        \023,,,,,,,,,,,,,,,,,,,,,,                                                 " ;  1
    DB "        \003\015Erix Collection                                                       " ;  2
    DB "                                                                                " ;  3
    DB "        \023,,,,,,,,,,,,,,,,,,,,,,                                                 " ;  4
    DB "        \006\015000\007Contents                  \003Shop                                   " ;  5
    DB "                                        \006https://www.erixcollectables.nl/       " ;  6
    DB "        \006\015001\007Basic NL                  \007Near Eindhoven, NL                     " ;  7
    DB "                                                                                " ;  8
    DB "        \006\015010\007Forth                                                             " ;  9
    DB "                                                                                " ; 10
    DB "        \006\015011\007UCSD Pascal               \003Repository with sources                " ; 11
    DB "                                        \006https://github.com/maarten-pennings    " ; 12
    DB "        \006\015100\007Assembler                 \006  /P2000-cartridge                     " ; 13
    DB "                                                                                " ; 14
    DB "        \006\015101\007JWS Disk BASIC                                                    " ; 15
    DB "                                                                                " ; 16
    DB "        \006\015110\007Familiegeheugen           \003Repository with roms                   " ; 17
    DB "                                        \006https://github.com/p2000t/software     " ; 18
    DB "        \006\015111\007Maintenance               \006  /tree/main/cartridges                " ; 19
    DB "                                                                                " ; 20
    DB "        \023,,,,,,,,,,,,,,,,,,,,,,                                                 " ; 21
    DB "        \005 off;set dipswitch;on                                                  " ; 22
    DB "                                                                                " ; 23
    DB "                                                                                " ; 24

