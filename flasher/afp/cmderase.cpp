// cmderase.cpp - command handler "erase" to erase (sector of) the flash
#include <Arduino.h>  // Serial
#include "cmd.h"      // https://github.com/maarten-pennings/cmd
#include "drv.h"      // drv_io_erase()
#include "cmdflash.h" // cmdflash_sectorsize_k()
#include "cmderase.h" // self


static bool cmderase_isempty(uint8_t sector) {
  uint32_t addr= sector*cmdflash_sectorsize();
  uint16_t num= cmdflash_sectorsize();

  #define BYTES_PER_LINE 16
  while( num>0 ) {
    uint8_t io[BYTES_PER_LINE];
    uint8_t size= min(num,BYTES_PER_LINE);
    drv_io_read(addr, io, size);
    for( uint8_t i=0; i<size; i++ ) {
      if( io[i]!=0xFF ) return false;
    }
    addr+=size;
    num-=size;
  }

  return true;
}


static void cmderase_show() {
  uint8_t rom2 = cmdflash_chipsize()/cmdflash_romsize();
  uint8_t empty=0;
  uint8_t inuse=0;
  Serial.println( F("rom sectors addresses    state-of-sectors:empty/inuse") );
  for( uint8_t rom1=0; rom1<rom2; rom1++ ) {
    uint8_t sector1=rom1*cmdflash_romsize()/cmdflash_sectorsize();
    uint8_t sector2=(rom1+1)*cmdflash_romsize()/cmdflash_sectorsize();
    cmd_printf( "R%02x S%02x-S%02x %05lx-%05lx:", rom1, sector1, sector2-1,sector1*cmdflash_sectorsize(),sector2*cmdflash_sectorsize()-1 );
    for( uint8_t sector=sector1; sector<sector2; sector++ ) {
      if( cmderase_isempty(sector) ) { empty++; Serial.print(F("    -  ")); } else { inuse++; Serial.print(F("  inuse")); }
    }
    Serial.println();
  }
  Serial.print(F("clusters: empty ")); Serial.print(empty); Serial.print(F(" inuse ")); Serial.println(inuse);
}


// The handler for the "erase" command
static void cmderase_main(int argc, char * argv[] ) {
  if( argc==1 ) {
    cmderase_show();
    return;
  }

  if( argc>2 ) {
    Serial.println( F("ERROR: too many args") );
    return;
  }

  if( strcmp(argv[0],"erase")!=0 ) {
    Serial.println( F("ERROR: 'erase' command must be spelled in full to prevent accidental erase"));
    return;
  }

  if( cmd_isprefix(PSTR("all"),argv[1]) ) {
    bool ok= drv_erase_chip();
    if( !ok ) { Serial.println(f("chip erase failed")); return; }
    if( argv[0][0]!='@') Serial.println( F("chip erased") );
    return;
  }

  // Establish S/R mode
  char *s= argv[1];
  uint8_t numsectors=0;
  if(      *s=='S' || *s=='s' ) { s++; numsectors=1; }
  else if( *s=='R' || *s=='r' ) { s++; numsectors=cmdflash_romsize()/cmdflash_sectorsize(); }
  if( numsectors==0 ) { Serial.println( F("ERROR: <addr> must start with S or R") ); return; }

  // Compute begin and end sector number
  uint32_t sector1;
  if( ! cmd_parse_hex32(s,&sector1) ) {
    Serial.println( F("ERROR: <addr> must be hex") );
    return;
  }
  sector1 *= numsectors;
  uint32_t sector2= sector1+numsectors;
  if( sector2 > cmdflash_chipsize()/cmdflash_sectorsize() ) {
    Serial.println( F("ERROR: <addr> greater than chip size") );
    return;
  }

  // Erase the sectors
  for( uint8_t sector=sector1; sector<sector2; sector++ ) {
    uint32_t sector_addr= sector * cmdflash_sectorsize();
    uint32_t rom_addr= sector_addr  / cmdflash_romsize();
    // print address(es)
    cmd_printf( "R%02lx S%02x:", rom_addr, sector );
    bool ok= drv_erase_sector(sector_addr);
    if( !ok ) { Serial.println(f("fail")); return; }
    if( argv[0][0]!='@') Serial.println( F("erased") );
  }
}


static const char cmderase_longhelp[] PROGMEM =
  "SYNTAX: erase\n"
  "- shows which sectors are empty or in use\n"
  "SYNTAX: erase <addr>\n"
  "- erases (ie fill with 0xFF) the sector indicated ny <addr>\n"
  "- when <addr> has form 'all', the whole chip (all sectors) is erased\n"
  "- when <addr> has form 's<hex>' (sector) that sector is erased\n"
  "- when <addr> has form 'r<hex>' (rom) the all its sectors are erased\n"
  "NOTE:\n"
  "- <hex> is in hex\n"
  "- a sector (4096 bytes) is the unit of erase of the flash chip\n"
  "- a rom consists of <rom> sectors, the unit of use (eg ROM cartridge)\n"
  "- <rom> is set with the 'flash' command\n"
  "- for added safety 'erase' command can not be abbreviated\n"
;


int cmderase_register(void) {
  return cmd_register(cmderase_main, PSTR("erase"), PSTR("erase (sector of) the flash"), cmderase_longhelp);
}
