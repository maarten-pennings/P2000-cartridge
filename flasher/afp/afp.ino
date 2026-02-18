// afp.ino - Arduino Flash Programmer (for the 39SF010, 39SF020, and 39SF040)
#include "drv.h"      // drv_init()
#include "cmd.h"      // https://github.com/maarten-pennings/cmd (written on v8.2.2)
#include "cmdinfo.h"  // cmdinfo_register()
#include "cmdflash.h" // cmdflash_register()
#include "cmdread.h"  // cmdread_register()
#include "cmderase.h" // cmderase_register()
#include "cmdwrite.h" // cmdwrite_register()
#include "afp.h"      // AFP_LONGNAME


static void cmds_register() {
  cmdecho_register();
  cmderase_register();
  cmdflash_register();
  cmdhelp_register();
  cmdinfo_register();
  cmdread_register();
  cmdwrite_register();
  Serial.println( F("cmds : init") );
}


void setup() {
  Serial.begin(115200, SERIAL_8N1);
  Serial.print( F(AFP_BANNER) );
  Serial.print( F(AFP_LONGNAME) ); Serial.print( F(" - version ") ); Serial.println( F(AFP_VERSION) );
  Serial.println( );

  drv_init();
  cmd_init();
  cmds_register();
  Serial.println( );

  Serial.println( F("Type 'help' for help") );
  cmd_prompt();
}


void loop() {
  cmd_pollserial();
}


// - have an info  command to print a table like below, maybe with EMPTY when all bytes are 0xFF

  // printid();

  // Serial.println("erase chip");
  // drv_erase_chip();
  // printrow(0x00000);
  // printrow(0x01000);
  // printrow(0x02000);
  
  // Serial.println("3:30, 1003:31, 2003:32");
  // drv_io_write(0x00003, 0x30);
  // drv_io_write(0x01003, 0x31);
  // drv_io_write(0x02003, 0x32);
  // printrow(0x00000);
  // printrow(0x01000);
  // printrow(0x02000);

  // Serial.println("erase sector 1000");
  // drv_erase_sector(0x1000);
  // printrow(0x00000);
  // printrow(0x01000);
  // printrow(0x02000);


  // drv_led_set(1);
  // delay(100);
  // drv_led_set(0);
  // delay(100);

/*
>> help
Available commands
 help - gives help (try 'help help')
 info - application info
 echo - echo a message (or en/disables echoing)
 read - read EEPROM memory
 write - write EEPROM memory
 verify - verify EEPROM memory
 program - write and verify EEPROM memory
 erase - erases EEPROM memory
 options - select options for the programmer



>> help read
SYNTAX: read [ <addr> [ <num> ] ]
- reads <num> bytes from EEPROM, starting at location <addr>
- when <num> is absent, it defaults to 1
- when <addr> and <num> are absent, reads entire EEPROM
NOTE:
- <addr> and <num> are in hex


>> help write
SYNTAX: write <addr> <data>...
- writes <data> byte to EEPROM location <addr>
- multiple <data> bytes allowed (auto increment of <addr>)
- <data> may be *, this toggles streaming mode
NOTE:
- <addr> and <data> are in hex
STREAMING:
- when streaming is active, the end-of-line does not terminate the command
- next lines having 0 or more <data> will also be written
- the prompt for next lines show the streaming mode (write, program, or verify)
- the prompt for next lines also show target address
- a line with * or a command will stop streaming mode


>> help verify
SYNTAX: verify <addr> <data>...
- reads byte from EEPROM location <addr> and compares to <data>
- prints <data> if equal, otherwise '<data>~<read>', where <read> is read data
- unequal values increment global error counter
- multiple <data> bytes allowed (auto increment of <addr>)
- <data> may be *, this toggles streaming mode (see 'write' command)
SYNTAX: verify print
- prints global error counter, uart overflow counter and stopwatch
SYNTAX: [@]verify clear
- sets global error counter, uart overflow counter, and stopwatch to 0
- with @ present, no feedback is printed
NOTE:
- <addr> and <data> are in hex


>> help program
SYNTAX: program <addr> <data>...
- performs write followed by verify
- see help for those commands for details
>> help erase

SYNTAX: erase [ <addr> [ <num> [ <data> [ <step> ] ] ] ]
- erase <num> bytes in EEPROM, starting at location <addr>, by writing <data>
- <data> it is stepped by one every <step> addresses.
- when <step> is absent, <data> is never stepped
- when <data> is absent, erase by writing 1's (<data>=FF)
- when <num> is absent, erase one page (<num>=100)
- when <addr> is absent, erase entire EEPROM
- erase is always verified
- command name 'erase' must be spelled in full to prevent accidental erase
NOTE:
- <addr>, <num>, <data>, and <step> are in hex


*/



// todo: commit cmd library

