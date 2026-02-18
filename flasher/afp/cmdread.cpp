// cmdread.cpp - command handler "read" to read flash contents
#include <Arduino.h>  // Serial
#include "cmd.h"      // https://github.com/maarten-pennings/cmd
#include "drv.h"      // drv_io_read()
#include "cmdflash.h" // cmdflash_sectorsize_k()
#include "cmdread.h"  // self


static uint32_t cmdread_addr;
static bool     cmdread_verbose;


#define BYTES_PER_LINE 16
static void cmdread_show(uint32_t num) {
  // clip current address to flash size
  if( cmdread_addr >= cmdflash_chipsize() ) {
    Serial.println(F("ERROR: at end of flash"));
    return;
  }
  // clip num bytes to flash size
  if( cmdread_addr+num >= cmdflash_chipsize() ) num= cmdflash_chipsize() - cmdread_addr;
  
  bool next=0;
  while( num>0 ) {
    uint32_t sector_addr= cmdread_addr / cmdflash_sectorsize();
    uint32_t rom_addr = sector_addr  / cmdflash_romsize();

    if( cmdread_verbose ) {
      // print divider
      char sep=0;
      if( cmdread_addr % 0x100 == 0 ) sep=' ';
      if( cmdread_addr % cmdflash_sectorsize() == 0 ) sep='-';
      if( cmdread_addr % cmdflash_romsize() == 0 ) sep='=';
      if( next && sep ) { for(byte i=0; i<63; i++) Serial.print(sep); Serial.println(); }
      next= 1;
    }

    // print address(es)
    cmd_printf( "R%02lx S%02lx %05lx:", rom_addr, sector_addr, cmdread_addr );

    // print data
    uint8_t size= BYTES_PER_LINE;
    uint8_t skip= BYTES_PER_LINE/2;
    if( cmdread_addr % BYTES_PER_LINE != 0 ) {
      // start addr is not 00 of page
      size -= (cmdread_addr % BYTES_PER_LINE);
      if( num>size ) {
        for( uint8_t i=0; i<BYTES_PER_LINE-size; i++ ) {
          if( i==skip ) Serial.print(" ");
          Serial.print("   ");
        }
        skip -= BYTES_PER_LINE-size;
      }
    }
    if( size>num ) size= num;

    uint8_t io[BYTES_PER_LINE];
    drv_io_read(cmdread_addr, io, size);
    for( uint8_t i=0; i<size; i++ ) {
      if( i==skip ) Serial.print(" ");
      cmd_printf(" %02x",io[i]);
    }
    Serial.println();

    num-= size;
    cmdread_addr+= size;
  }
}


// The handler for the "read" command
static void cmdread_main(int argc, char * argv[] ) {
  if( argc==1 ) {
    cmdread_show(0x100);
    return;
  } 
  
  char *s= argv[1];
  uint32_t factor=1;
  if(      *s=='S' || *s=='s' ) { s++; factor= cmdflash_sectorsize(); }
  else if( *s=='R' || *s=='r' ) { s++; factor= cmdflash_romsize(); }
  uint32_t addr;
  if( ! cmd_parse_hex32(s,&addr) ) { Serial.println( F("ERROR: <addr> must be hex") ); return; }
  addr *= factor;
  if( addr >= cmdflash_chipsize() ) { Serial.println( F("ERROR: <addr> greater than chip size") ); return; }

  uint32_t num;
  if( argc==2 ) {
    num = 0x100;
  } else {
    if( ! cmd_parse_hex32(argv[2],&num) ) {
      Serial.println( F("ERROR: <num> must be hex") );
      return;
    }
  }
  
  cmdread_addr= addr;
  cmdread_verbose= argv[0][0]!='@';
  cmdread_show(num);
}


static const char cmdread_longhelp[] PROGMEM = 
  "SYNTAX: read [ <addr> [ <num> ]]\n"
  "- reads <num> bytes from flash, starting at location <addr>\n"
  "- when <num> is absent, reads 256 bytes\n"
  "- when <addr> has form <hex> the address is <hex>\n"
  "- when <addr> has form s<hex> (sector) the address is <hex>*0x1000\n"
  "- when <addr> has form r<hex> (rom) the address is <hex>*<group>*0x1000\n"
  "- when addr is absent continues from previous read\n"
  "NOTE:\n"
  "- <addr>, <num>, and <hex> are in hex\n"
  "- a sector (16 pages, 4096 bytes) is the unit of erase of the flash chip\n"
  "- a rom consists of <rom> sectors, the unit of use (eg ROM cartridge)\n"
  "- <rom> is set with the 'flash' command\n"
;


int cmdread_register(void) {
  return cmd_register(cmdread_main, PSTR("read"), PSTR("read flash contents"), cmdread_longhelp);
}

