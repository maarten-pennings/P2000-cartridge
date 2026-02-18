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
