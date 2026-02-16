// cmdflash.cpp - command handler "flash" to view/control the size of flash
#include <Arduino.h>  // Serial
#include "cmd.h"      // v8.2.1 https://github.com/maarten-pennings/cmd
#include "drv.h"      // drv_id()
#include "cmdflash.h" // self


static       uint16_t cmdflash_chipsize   = 128; // in k bytes
static       uint16_t cmdflash_groupsize  = 4;   // in sectors
static const uint16_t cmdflash_sectorsize = 4;   // in k bytes 


uint16_t cmdflash_chipsize_k() {
  return cmdflash_chipsize;
}


uint16_t cmdflash_groupsize_sectors() {
  return cmdflash_groupsize;
}


uint16_t cmdflash_sectorsize_k() {
  return cmdflash_sectorsize;
}


static void cmdflash_chipshow() {
  Serial.print( F("chip  size: ") );
  Serial.print( cmdflash_chipsize );
  Serial.print( F(" k bytes (") );
  Serial.print( cmdflash_chipsize/cmdflash_sectorsize );
  Serial.print( F(" sectors) (") );
  Serial.print( cmdflash_chipsize/cmdflash_sectorsize/cmdflash_groupsize );
  Serial.println( F(" groups)") );
}


static void cmdflash_groupshow() {
  Serial.print( F("group size: ") );
  Serial.print( cmdflash_groupsize );
  Serial.print( F(" sectors (") );
  Serial.print( cmdflash_sectorsize*cmdflash_groupsize );
  Serial.println( F(" k bytes)") );
}


void cmdflash_patch_and_print() {
  if( cmdflash_sectorsize * cmdflash_groupsize > cmdflash_chipsize ) {
    Serial.println( F("WARNING group size reduced") );
    cmdflash_groupsize = cmdflash_chipsize / cmdflash_sectorsize;
  }
  cmdflash_chipshow();
  cmdflash_groupshow();
}


void cmdflash_auto() {
  uint8_t manid,devid;
  drv_id(&manid,&devid);

  // Print the two IDs
  Serial.print( F("manid ") );
  Serial.print(manid,HEX);
  if( manid==DRV_MANID_MICROCHIP ) Serial.print( F(": Microchip / SST") );
  Serial.println();
  Serial.print( F("devid ") );
  Serial.print(devid,HEX);
  if( devid==DRV_DEVID_39SF010 ) { Serial.print( F(": 39SF010 (1 Mbit = 128 kbyte)") ); cmdflash_chipsize=128; }
  if( devid==DRV_DEVID_39SF020 ) { Serial.print( F(": 39SF020 (2 Mbit = 256 kbyte)") ); cmdflash_chipsize=256; }
  if( devid==DRV_DEVID_39SF040 ) { Serial.print( F(": 39SF040 (4 Mbit = 512 kbyte)") ); cmdflash_chipsize=512; }
  Serial.println();
}


// The handler for the "flash" command
static void cmdflash_main(int argc, char * argv[] ) {
  if( argc==1 ) {
    cmdflash_chipshow();
    cmdflash_groupshow();
  } else if( cmd_isprefix(PSTR("chip"),argv[1]) ) { 
    if( argc==2 ) {
      cmdflash_chipshow();
    } else if( cmd_isprefix(PSTR("auto"),argv[2]) ) { 
      cmdflash_auto();
      cmdflash_patch_and_print();
    } else if( cmd_isprefix(PSTR("128"),argv[2]) ) { 
      cmdflash_chipsize=128; 
      cmdflash_patch_and_print();
    } else if( cmd_isprefix(PSTR("256"),argv[2]) ) { 
      cmdflash_chipsize=256; 
      cmdflash_patch_and_print();
    } else if( cmd_isprefix(PSTR("512"),argv[2]) ) { 
      cmdflash_chipsize=512; 
      cmdflash_patch_and_print();
    } else {
      Serial.println( F("'flash chip' has unknown size") );
    }
  } else if( cmd_isprefix(PSTR("group"),argv[1]) ) { 
    if( argc==2 ) {
      cmdflash_groupshow();
    } else {
      int size;
      bool ok= cmd_parse_dec(argv[2],&size) ;
      if( ok && size>0 ) {
        cmdflash_groupsize= size; 
        cmdflash_patch_and_print();
      } else {
        Serial.println( F("'flash group' has unknown/illegal size") );
      }
    }
  } else {
    Serial.println( F("'flash' needs subcommand 'chip' or 'group'") );
  }
}


static const char cmdflash_longhelp[] PROGMEM = 
  "SYNTAX: flash\n"
  "- shows chip size and group size\n"
  "SYNTAX: flash chip [ auto | 128 | 256 | 512 ]\n"
  "- without arguments shows configured chip size (default 128)\n"
  "- with arguments sets chip size (in k bytes)\n"
  "- with argument 'auto' queries flash chip for size\n"
  "SYNTAX: flash group [ <size> ]\n"
  "- without arguments shows configured group size (default 4)\n"
  "- with argument sets group size (in number of 4 k sectors)\n"
  "- group size is for convenience in 'read', 'write' commands\n"
  "- group size is intended to match ROM size in multi-ROM\n"
;


int cmdflash_register(void) {
  return cmd_register(cmdflash_main, PSTR("flash"), PSTR("view and control flash size"), cmdflash_longhelp);
}



