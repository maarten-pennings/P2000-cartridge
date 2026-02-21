// cmdinfo.cpp - command handler "info" for application and cpu info
#include <Arduino.h>  // Serial
#include "cmd.h"      // https://github.com/maarten-pennings/cmd
#include "afp.h"      // AFP_NAME etc
#include "cmdinfo.h"  // self


// Get the VCC the Arduino Nano itself runs on (using built-in ref)
// Since is also powers the EEPROM, this should not be too low


static uint32_t cmdinfo_mega328_readVcc(void) {
  // From: https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
  uint32_t result; // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while( bit_is_set(ADCSRA,ADSC) );
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result;
  // Back-calculate AVcc in mV
  return result;
}


#define MEGA328_AVERAGE_COUNT 32
static uint32_t cmdinfo_mega328_Vcc(void) {
  uint32_t result=0;
  for( int i=0; i<MEGA328_AVERAGE_COUNT; i++ ) result+=cmdinfo_mega328_readVcc();
  return (result+MEGA328_AVERAGE_COUNT/2)/MEGA328_AVERAGE_COUNT;
}


// The handler for the "info" command
static void cmdinfo_main(int argc, char * argv[] ) {
  (void)argc; // unused
  if( argv[0][0]!='@' ) { Serial.print(F("name   : ")); Serial.print(F(AFP_LONGNAME)); Serial.print("\n"); }
  Serial.print(F("version: ")); Serial.print(F(AFP_VERSION)); Serial.print("\n");
  if( argv[0][0]!='@' ) { Serial.print(F("author : ")); Serial.print(F(AFP_AUTHOR)); Serial.print("\n"); }
  if( argv[0][0]!='@' ) { Serial.print(F("date   : ")); Serial.print(F(AFP_DATE)); Serial.print("\n"); }
  if( argv[0][0]!='@' ) { Serial.print(F("voltage: ")); Serial.print( cmdinfo_mega328_Vcc() ); Serial.print(F(" mV\n")); }
  if( argv[0][0]!='@' ) { Serial.print(F("cpufreq: ")); Serial.print( F_CPU ); Serial.print(F(" Hz\n")); }
  if( argv[0][0]!='@' ) { Serial.print(F("uartbuf: ")); Serial.print( SERIAL_RX_BUFFER_SIZE ); Serial.print(F(" bytes\n")); }
}


static const char cmdinfo_longhelp[] PROGMEM =
  "SYNTAX: info\n"
  "- shows application information (name, author, version, date)\n"
  "- shows cpu info (cpu voltage, cpu speed, uart rx buf size)\n"
;


int cmdinfo_register(void) {
  return cmd_register(cmdinfo_main, PSTR("info"), PSTR("application and cpu info"), cmdinfo_longhelp);
}
