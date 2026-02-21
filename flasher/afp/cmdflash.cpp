// cmdflash.cpp - command handler "flash" to view/control the size of flash
#include <Arduino.h>  // Serial
#include "cmd.h"      // https://github.com/maarten-pennings/cmd
#include "drv.h"      // drv_id()
#include "cmdflash.h" // self


static       uint16_t cmdflash_chipsize_k   = 128; // can also be 256 or 512
static       uint16_t cmdflash_romsize_k    =  16; // may be changed by user
static const uint16_t cmdflash_sectorsize_k =   4; // fixed


uint32_t cmdflash_chipsize() {
  return cmdflash_chipsize_k * 1024L;
}


uint32_t cmdflash_romsize() {
  return cmdflash_romsize_k * 1024L;
}


uint32_t cmdflash_sectorsize() {
  return cmdflash_sectorsize_k * 1024L;
}


static void cmdflash_show_chip(bool verbose) {
  Serial.print( F("chip size: ") );
  Serial.print( cmdflash_chipsize_k );
  Serial.print( F(" kbyte") );
  if( verbose ) Serial.print( F(" (") );
  if( verbose ) Serial.print( cmdflash_chipsize_k/cmdflash_sectorsize_k );
  if( verbose ) Serial.print( F(" sectors) (") );
  if( verbose ) Serial.print( cmdflash_chipsize_k/cmdflash_romsize_k );
  if( verbose ) Serial.print( F(" roms)") );
  Serial.print("\n");
}


static void cmdflash_show_rom(bool verbose) {
  Serial.print( F("rom  size: ") );
  Serial.print( cmdflash_romsize_k );
  Serial.print( F(" kbyte") );
  if( verbose ) Serial.print( F(" (") );
  if( verbose ) Serial.print(  cmdflash_romsize_k/cmdflash_sectorsize_k);
  if( verbose ) Serial.print( F(" sectors)") );
  Serial.print("\n");
}


static void cmdflash_rom_patch() {
  if( cmdflash_romsize_k > cmdflash_chipsize_k ) {
    Serial.print( F("WARNING: rom size reduced\n") );
    cmdflash_romsize_k = cmdflash_chipsize_k;
  }
}


void cmdflash_auto( bool verbose ) {
  uint8_t manid,devid;
  drv_id(&manid,&devid);

  // Print the two IDs
  if( verbose ) { Serial.print( F("manid ") ); Serial.print(manid,HEX); Serial.print( F(": ") ); }
  if( manid==DRV_MANID_MICROCHIP ) Serial.print( F("Microchip/SST") );
  if( verbose ) { Serial.print("\n"); Serial.print( F("devid ") ); Serial.print(devid,HEX); Serial.print( F(": ") ); }
  if( devid==DRV_DEVID_39SF010 ) { Serial.print( F("39SF010 (1 Mbit = 128 kbyte)") ); cmdflash_chipsize_k=128; }
  if( devid==DRV_DEVID_39SF020 ) { Serial.print( F("39SF020 (2 Mbit = 256 kbyte)") ); cmdflash_chipsize_k=256; }
  if( devid==DRV_DEVID_39SF040 ) { Serial.print( F("39SF040 (4 Mbit = 512 kbyte)") ); cmdflash_chipsize_k=512; }
  Serial.print("\n");
}


// The handler for the "flash" command
static void cmdflash_main(int argc, char * argv[] ) {
  if( argc==1 ) {
    cmdflash_show_chip(argv[0][0]!='@');
    cmdflash_show_rom(argv[0][0]!='@');
  } else if( cmd_isprefix(PSTR("chip"),argv[1]) ) {
    if( argc==2 ) {
      cmdflash_show_chip(argv[0][0]!='@');
    } else if( argc>3 ) {
      Serial.print( F("ERROR: too many args\n") );
      return;
    } else {
      if( cmd_isprefix(PSTR("auto"),argv[2]) ) {
        cmdflash_auto( argv[0][0]!='@' );
      } else if( cmd_isprefix(PSTR("128"),argv[2]) ) {
        cmdflash_chipsize_k=128;
      } else if( cmd_isprefix(PSTR("256"),argv[2]) ) {
        cmdflash_chipsize_k=256;
      } else if( cmd_isprefix(PSTR("512"),argv[2]) ) {
        cmdflash_chipsize_k=512;
      } else {
        Serial.print( F("ERROR: unknown size (128, 256, 512)\n") );
        return;
      }
      cmdflash_rom_patch();
      if( argv[0][0]!='@' ) cmdflash_show_chip( argv[0][0]!='@' );
    }
  } else if( cmd_isprefix(PSTR("rom"),argv[1]) ) {
    if( argc==2 ) {
      cmdflash_show_rom(argv[0][0]!='@');
    } else if( argc>3 ) {
      Serial.print( F("ERROR: too many args\n") );
      return;
    } else {
      int size;
      bool ok= cmd_parse_dec(argv[2],&size) ;
      if( !ok || size==0 ) {
        Serial.print( F("ERROR: illegal size\n") );
        return;
      }
      if( size%cmdflash_sectorsize_k!=0 ) {
        Serial.print( F("ERROR: rom size must be multiple of sector size (") );
        Serial.print(cmdflash_sectorsize_k);
        Serial.print( F(" kbyte)\n") );
        return;
      }
      if( cmdflash_chipsize_k%size!=0 ) {
        Serial.print( F("ERROR: rom size must be divider of flash size (") );
        Serial.print(cmdflash_chipsize_k);
        Serial.print( F(" kbyte)\n") );
        return;
      }
      cmdflash_romsize_k= size;
      if( argv[0][0]!='@' ) cmdflash_show_rom(argv[0][0]!='@');
    }
  } else {
    Serial.print( F("ERROR: need subcommand 'chip' or 'rom'\n") );
  }
}


static const char cmdflash_longhelp[] PROGMEM =
  "SYNTAX: flash\n"
  "- shows configured chip size and rom size\n"
  "SYNTAX: flash chip [ auto | 128 | 256 | 512 ]\n"
  "- without arguments shows configured chip size (default 128)\n"
  "- with arguments sets chip size (in k byte)\n"
  "- with argument 'auto' queries flash chip for size and configures that\n"
  "SYNTAX: flash rom [ <size> ]\n"
  "- without arguments shows configured rom size (default 16)\n"
  "- with argument sets rom size (in k byte)\n"
  "- rom size is a convenience feature for other commands (erase rom 3)\n"
  "- rom size is intended to match ROM size in multi-ROM\n"
;


int cmdflash_register(void) {
  return cmd_register(cmdflash_main, PSTR("flash"), PSTR("view and control flash size"), cmdflash_longhelp);
}
