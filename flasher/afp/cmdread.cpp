// cmdread.cpp - command handler "read" to read flash contents
#include <Arduino.h>  // Serial
#include "cmd.h"      // v8.2.1 https://github.com/maarten-pennings/cmd
#include "drv.h"      // drv_io_read()
#include "cmdflash.h" // cmdflash_sectorsize_k()
#include "cmdread.h"  // self


static uint32_t cmdread_addr= 0x00000;


#define BYTES_PER_LINE 16
static void cmdread_show(uint32_t num) {
  // clip current address to flash size
  uint32_t flash_size= cmdflash_chipsize_k()*1024L;
  if( cmdread_addr >= flash_size ) cmdread_addr= 0; 
  // clip num bytes to flash size
  if( cmdread_addr+num >= flash_size ) num= flash_size - cmdread_addr;
  
  while( num>0 ) {
    uint32_t sector_addr= cmdread_addr / (1024*cmdflash_sectorsize_k());
    char sector_star= cmdread_addr % (1024*cmdflash_sectorsize_k()) ? ' ' : '*';
    uint32_t group_addr = sector_addr  / cmdflash_groupsize_sectors();
    char group_star= ' ';
    if( sector_star=='*' ) group_star= sector_addr  % cmdflash_groupsize_sectors() ? ' ' : '*';
    cmd_printf( "G%02lx%cS%02lx%c%05lx:", group_addr, group_star, sector_addr, sector_star, cmdread_addr );

    uint8_t size= BYTES_PER_LINE;
    uint8_t skip= 8;
    if( cmdread_addr % BYTES_PER_LINE != 0 ) {
      // start addr is not 00 of page
      size -= (cmdread_addr % BYTES_PER_LINE);
      if( num>size ) {
        for( uint8_t i=0; i<BYTES_PER_LINE-size; i++ ) {
          if( i==skip ) cmd_printf(" ");
          cmd_printf("   ");
        }
        skip -= BYTES_PER_LINE-size;
      }
    }
    if( size>num ) size= num;

    uint8_t io[BYTES_PER_LINE];
    drv_io_read(cmdread_addr, io, size);
    for( uint8_t i=0; i<size; i++ ) {
      if( i==skip ) cmd_printf(" ");
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
  if( *s=='S' || *s=='s' ) { s++; factor= cmdflash_sectorsize_k()*1024L; }
  else if( *s=='G' || *s=='g' ) {s++; factor= cmdflash_groupsize_sectors()*cmdflash_sectorsize_k()*1024L; }
  uint32_t addr;
  if( ! cmd_parse_hex32(s,&addr) ) {
    Serial.println( F("'read' has error in <addr>") );
    return;
  }
  addr *= factor;
  if( addr >= cmdflash_chipsize_k()*1024L ) {
    Serial.println( F("'read' has <addr> greater than chip size") );
    return;
  }

  uint32_t num;
  if( argc==2 ) {
    num = 0x100;
  } else {
    if( ! cmd_parse_hex32(argv[2],&num) ) {
      Serial.println( F("'read' has error in <num>") );
      return;
    }
  }
  
  cmdread_addr= addr;
  cmdread_show(num);
}


static const char cmdread_longhelp[] PROGMEM = 
  "SYNTAX: read [ <addr> [ <num> ]]\n"
  "- reads <num> bytes from flash, starting at location <addr>\n"
  "- when <num> is absent, reads 256 bytes\n"
  "- when <addr> has form <hex> the address is <hex>\n"
  "- when <addr> has form s<hex> (sector) the address is <hex>*0x1000\n"
  "- when <addr> has form g<hex> (group) the address is <hex>*<group>*0x1000\n"
  "- when addr is absent continues from previous read\n"
  "NOTE:\n"
  "- <addr>, <num>, and <hex> are in hex\n"
  "- a sector (16 pages, 4096 bytes) is the unit of erase of the flash chip\n"
  "- a group consists of <group> sectors, the unit of use (eg ROM cartridge)\n"
  "- <group> is set with the 'flash' command\n"
;


int cmdread_register(void) {
  return cmd_register(cmdread_main, PSTR("read"), PSTR("read flash contents"), cmdread_longhelp);
}

