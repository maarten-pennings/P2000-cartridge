// cmdwrite.cpp - command handler "write" to write to flash
#include <Arduino.h>  // Serial
#include "cmd.h"      // https://github.com/maarten-pennings/cmd
#include "drv.h"      // drv_io_read()
#include "cmdflash.h" // cmdflash_sectorsize_k()
#include "cmdwrite.h" // self


static uint32_t cmdwrite_addr;
static uint32_t cmdwrite_stats_writecount;
static uint32_t cmdwrite_stats_errorcount;
static uint32_t cmdwrite_stats_timems;


void cmdwrite_stream(int argc, char * argv[] ) {
  for( int i=0; i<argc; i++) {
    if( strcmp(argv[i],"*")==0 ) { // toggle streaming mode
      if( cmd_get_streamfunc()==0 ) cmd_set_streamfunc(cmdwrite_stream); else cmd_set_streamfunc(0);
      continue;
    }
    // Process <data> byte
    uint16_t data1;
    if( !cmd_parse_hex(argv[i],&data1) || data1>=0x100) { Serial.print(F("ERROR: <data> must be hex 00..FF (use * to exit)\n")); goto exit; }
    if( cmdwrite_addr >= cmdflash_chipsize() ) { Serial.print( F("ERROR: <addr> greater than chip size\n") ); cmd_set_streamfunc(0); goto exit; }
    bool ok= drv_io_write(cmdwrite_addr, data1 );
    if( ok ) {
      uint8_t data2;
      drv_io_read(cmdwrite_addr, &data2, 1);
      if( data1 != data2 ) cmdwrite_stats_errorcount++; // written data does not match read data
    } else {
      cmdwrite_stats_errorcount++; // write signalled fail (timeout of write)
    }
    cmdwrite_stats_writecount++;
    cmdwrite_addr++;
  }
exit:
  // Set the streaming prompt (will only be shown in streaming mode)
  cmd_set_streampromptf("w%05lx>> ",cmdwrite_addr);
}


void cmdwrite_main(int argc, char * argv[] ) {
  if( argc==1 ) {
    Serial.print(F("ERROR: expected 'stat' or <addr>\n"));
    return;
  }

  if( cmd_isprefix(PSTR("stats"),argv[1]) ) {
    if( argc==2 )  {
      cmd_printf("bytes written %lu\n",cmdwrite_stats_writecount);
      cmd_printf("write errors  %lu\n",cmdwrite_stats_errorcount);
      cmd_printf("elapsed time  %lu ms\n",millis()-cmdwrite_stats_timems);
      return;
    }
    if( argc>3 ) { Serial.print(F("ERROR: too many args\n")); return; }
    if( ! cmd_isprefix(PSTR("clear"),argv[2]) ) { Serial.print(F("ERROR: expected 'clear'\n")); return; }
    cmdwrite_stats_writecount= 0;
    cmdwrite_stats_errorcount= 0;
    cmdwrite_stats_timems= millis();
    if( argv[0][0]!='@' ) Serial.print("cleared\n");
    return;
  }

  // Parse addr
  char *s= argv[1];
  uint32_t factor=1;
  if(      *s=='S' || *s=='s' ) { s++; factor= cmdflash_sectorsize(); }
  else if( *s=='R' || *s=='r' ) { s++; factor= cmdflash_romsize(); }

  if( argc==2 ) { Serial.print(F("ERROR: expected <data> or '*'\n")); return; }
  uint32_t addr;
  if( ! cmd_parse_hex32(s,&addr) ) { Serial.print( F("ERROR: <addr> must be hex\n") ); return; }
  addr *= factor;

  // Write data('s)
  cmdwrite_addr= addr;
  cmdwrite_stream(argc-2, argv+2);
}


static const char cmdwrite_longhelp[] PROGMEM =
  "SYNTAX: write stats [clear]\n"
  "- with 'clear' reset stats\n"
  "- otherwise prints stats\n"
  "SYNTAX: write <addr> <data>...\n"
  "- writes <data> byte to flash at location <addr>\n"
  "- multiple <data> bytes allowed (auto increment of <addr>)\n"
  "- <data> may be *, this toggles streaming mode\n"
  "- when <addr> has form <hex> the address is <hex>\n"
  "- when <addr> has form s<hex> (sector) the address is <hex>*0x1000\n"
  "- when <addr> has form r<hex> (rom) the address is <hex>*<group>*0x1000\n"
  "NOTE:\n"
  "- <addr>, <data>, and <hex> are in hex\n"
  "- a sector (16 pages, 4096 bytes) is the unit of erase of the flash chip\n"
  "- a rom consists of <rom> sectors, the unit of use (eg ROM cartridge)\n"
  "- <rom> is set with the 'flash' command\n"
  "STREAMING:\n"
  "- when streaming is active, the end-of-line does not terminate the command\n"
  "- next lines having 0 or more <data> will also be written\n"
  "- the prompt for next lines also show target address\n"
  "- a line with * will stop streaming mode\n"
;


int cmdwrite_register(void) {
  return cmd_register(cmdwrite_main, PSTR("write"), PSTR("write bytes to flash"), cmdwrite_longhelp);
}
