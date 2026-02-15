// Arduino Flash Programmer (for the 39SF010, 39SF020, and 39SF040)
#include "drv.h"


void printid() {
  uint8_t manid,devid;
  drv_id(&manid,&devid);

  // Print the two IDs
  Serial.print("manid ");
  Serial.print(manid,HEX);
  if( manid==DRV_MANID_MICROCHIP ) Serial.print(" Microchip");
  Serial.println();
  Serial.print("devid ");
  Serial.print(devid,HEX);
  if( devid==DRV_DEVID_39SF010 ) Serial.print(" 39SF010");
  if( devid==DRV_DEVID_39SF020 ) Serial.print(" 39SF020");
  if( devid==DRV_DEVID_39SF040 ) Serial.print(" 39SF040");
  Serial.println();
}


void printrow(uint32_t addr) {
  // Read
  uint8_t io[16];
  drv_io_read(addr, io, sizeof io);
  // Print
  if( addr<0x10 ) Serial.print("0");
  if( addr<0x100 ) Serial.print("0");
  if( addr<0x1000 ) Serial.print("0");
  Serial.print(addr,HEX);
  Serial.print(":");
  for( uint8_t i=0; i<sizeof io; i++ ) {
    if( io[i]<10 ) Serial.print("0");
    Serial.print(io[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200, SERIAL_8N1);
  Serial.println(); Serial.println();
  Serial.println("Welcome to AFP - Android Flash programmer");
  drv_init();

  printid();

  Serial.println("erase chip");
  drv_erase_chip();
  printrow(0x00000);
  printrow(0x01000);
  printrow(0x02000);
  
  Serial.println("3:30, 1003:31, 2003:32");
  drv_io_write(0x00003, 0x30);
  drv_io_write(0x01003, 0x31);
  drv_io_write(0x02003, 0x32);
  printrow(0x00000);
  printrow(0x01000);
  printrow(0x02000);

  Serial.println("erase sector 1000");
  drv_erase_sector(0x1000);
  printrow(0x00000);
  printrow(0x01000);
  printrow(0x02000);
}


void loop() {
  drv_led_set(1);
  delay(100);
  drv_led_set(0);
  delay(100);
}


// - as address also allow Snn where nn is hex sector number
// - have opt commmand to set the number of clusters in a group (default 4 matches our ROM size)
// - as address also allow Gnn where nn is hex rom number
// - have a type command to set type, allow autodetect
// - have an info  command to print a table like below, maybe with EMPTY when all bytes are 0xFF

