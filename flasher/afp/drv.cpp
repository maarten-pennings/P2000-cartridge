// drv.cpp - driver for the 39SF010, 39SF020, and 39SF040 flash chips
#include <Arduino.h>
#include <stdint.h>
#include "drv.h"


// This driver maintains two invariants
// This hold between function calls, but are maybe "violated" within a function
// + Flash-invariant (port B bits 0,1)
//   - WEN=1 (write disabled)
//   - OEN=1 (output disabled)
// + Shift-invariant (port B bits 3,4)
//   - CLK=0
//   - LAT=0


// We have a slightly confusing terminology:
// - The flash chip (39SF0x0) has up to 19 address lines (A0..A18)
// - The flash chip (39SF0x0) has precisly 8 io (data) lines (IO0..IO7).
// We use IO3 instead of D3, because D3 is a GPIO name of an Arduino pin.
void drv_init() {
  // This driver uses PORTX, DDRX, and PINX instead of digitalWrite() etc because of the speed.

  //  7   6   5   4   3   2   1   0  (Arduino pin nums, Dxx)
  // PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0 (ATMEGA328 port/pin)
  // IO7 IO6 IO5 IO4 IO3  -   -   -  (PCB: flash data pins)
  PORTD = 0b00000000; 
  DDRD  = 0b00000000;

  //  -   -   13  12  11  10  9   8  (Arduino pin nums, Dxx)
  // PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0 (ATMEGA328 port/pin)
  //  -   -  LED LAT CLK DAT WEN OEN (PCB: control)
  PORTB = 0b00000011; // WEN=HIGH, OEN=HIGH
  DDRB  = 0b00111111; // bits to outputs (except unused ones)
  // flash and shift invariant established (WEN=1, OEN=1, CLK=0, LAT=0)

  //  21  20  19  18  17  16  15  14  (Arduino pin nums, Dxx)
  //  -   -  PC5 PC4 PC3 PC2 PC1 PC0 (ATMEGA328 port/pin)
  //  -   -  A18 A17 A16 IO2 IO1 IO0  (PCB: flash address and data pins)
  PORTC = 0b00000000; 
  DDRC  = 0b00111000;  // address lines output

  // flash and shift invariant
}


void drv_led_set(uint8_t state) {
  bitWrite(PORTB, 5, !state);
}


// Shortcuts for the control lines (in port B)
#define SET_OEN(state)    bitWrite(PORTB, 0, state)   // output enable pin flash chip (high for write, low for read)
#define SET_WEN(state)    bitWrite(PORTB, 1, state)   // write  enable pin flash chip (low for write, high for read)
#define SET_DAT(state)    bitWrite(PORTB, 2, state)   // Data  line of 74HC595 shift register: DAT (aka SER) 
#define SET_CLK(state)    bitWrite(PORTB, 3, state)   // Clock line of 74HC595 shift register: CLK (aka SRCLK or SH_CLK) 
#define SET_LAT(state)    bitWrite(PORTB, 4, state)   // Latch line of 74HC595 shift register: LAT (aka RCLK or ST_CLK)


// Post condition of these macros is flash and shift invariant (assuming pre condition is also both)
#define PULSE_WEN()       do { SET_WEN(LOW); SET_WEN(HIGH); } while(0)
#define PULSE_CLK()       do { SET_CLK(LOW); SET_CLK(HIGH); } while(0)
#define PULSE_LAT()       do { SET_LAT(HIGH); SET_LAT(LOW); } while(0)


// Before a read or write operation, the address pins of the flash chip must be set.
// These pins are spread over 3 pins in port C (A18..A16) and 16 via external shift registers (A15..A0).
// This function sets all 19 address lines (512k range).
// The external shift register is wired as LSB first (normally MSB first is used on comms lines).
void drv_addr_set(uint32_t addr) { 
  for( uint8_t i=0; i<16; i++) {
    bitWrite(PORTB, 2, addr & 1); 
    PULSE_CLK(); 
    addr = addr>>1;
  }
  PULSE_LAT(); 
  PORTC = (PORTC & 0b11000111) | ((addr<<3) & 0b111000); // Set three MSB of addr
  // flash and shift invariant (CLK and LAT pulsed)
}


// The data or IO pins (IO7..IO0) are spread of port C (3 LSBs) and port D (5 MSBs).
// When reading from the flash, we need to configure these pins as tristate input.
void drv_io_cfgread() {
  // Note the &=
  DDRC  &= 0b11111000; // Three LSBs IO2/IO1/IO0 to input (0)
  PORTC &= 0b11111000; // When input, PORTC controls tristate (0) or pullup (1); switch inputs to tristate
  DDRD  &= 0b00000111; // Five MSBs IO7/IO6/IO5/IO4/IOD3 to input (0)
  PORTD &= 0b00000111; // When input, PORTC controls tristate (0) or pullup (1); switch inputs to tristate
  // flash and shift invariant (port B not touched)
}


// The IO pins (IO7..IO0) are spread of port C (3 LSBs) and port D (5 MSBs).
// When writing to the flash, we need to configure these pins as output.
void drv_io_cfgwrite() {
  // Note the |=
  DDRC |= 0b00000111; // Three LSBs IO2/IO1/IO0 to output (1)
  DDRD |= 0b11111000; // Five MSBs IO7/IO6/IO5/IO4/IOD3 to input (0)
  // flash and shift invariant (port B not touched)
}


// Once the data aka IO pins are configured for input - with drv_io_cfgread()
// this function reads the values on the pins and assembles them into a byte.
// Note, OEN must be pulled low before the flash chip puts the data on the pins.
#define drv_io_get()         ( (PIND & 0b11111000) | (PINC & 0b00000111) )


// Once the data aka IO pins are configured for output - with drv_io_cfgwrite()
// this function writes the bits from io to the various pins.
// Note, this puts signals on the data aka IO pins of the flash, it does not necesarily write to flash.
// For this flash needs to be prepared ("byte-program sequence"), see drv_io_write() for that.
void drv_io_set(uint8_t io) { 
  PORTC = (PORTC & 0b11111000) | (io & 0b00000111); // Three LSBs IO2/IO1/IO0 written
  PORTD = (PORTD & 0b00000111) | (io & 0b11111000); // Five MSBs IO7/IO6/IO5/IO4/IOD3 written
  // flash and shift invariant (port B not touched)
}


void drv_io_read(uint32_t addr, uint8_t * io, uint8_t size) {
  drv_io_cfgread();
  SET_OEN(LOW);  
    for( uint32_t a=addr; a<addr+size; a++ ) {
      drv_addr_set(a);
      *io= drv_io_get();
      io++;
    }
  SET_OEN(HIGH);  
  // flash and shift invariant (OEN restored)
}


#define MSB(x) ((x)&128)
// This functions writes data `io` to address `addr` in the flash.
// It prepares the flash for the write operation ("byte-program sequence"), then writes, then waits till write completes.
// Returns true iff write completes in time (~20us).
// Writing `mem[addr]=io` actually means "and with `mem[addr]&=io`, so only 0 bits are created.
// Before writing typically erase the sector of entire chip.
bool drv_io_write(uint32_t addr, uint8_t io) {
  // Write the byte-program sequence
  drv_io_cfgwrite();
  drv_addr_set(0x5555); drv_io_set(0xaa); PULSE_WEN();
  drv_addr_set(0x2aaa); drv_io_set(0x55); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0xa0); PULSE_WEN();
  // Write the byte
  drv_addr_set(addr);   drv_io_set(io);   PULSE_WEN();
  // Poll for completion
  drv_io_cfgread();
  SET_OEN(LOW);
    uint8_t time=0; while( (MSB(drv_io_get())!=MSB(io)) && (time<100) ){ time++; delayMicroseconds(1); } // time ~20us
  SET_OEN(HIGH);
  //Serial.print("byte write "); Serial.print(time); Serial.println("us");
  return time<50;
  // flash and shift invariant (WEN pulsed, OEN restored)
}


// This functions erases one sector in the flash. A sector is the smallest unit of erasing; it is 4k byte. Erase means fill with 0xFF.
// Returns true iff erase completes in time (~200us).
// `addr` is a random address in the (4k) sector, typically use the base address (ie sectornum*0x1000)
bool drv_erase_sector(uint32_t addr) {
  // Write the chip-erase sequence
  drv_io_cfgwrite();
  drv_addr_set(0x5555); drv_io_set(0xaa); PULSE_WEN(); 
  drv_addr_set(0x2aaa); drv_io_set(0x55); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0x80); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0xaa); PULSE_WEN();
  drv_addr_set(0x2aaa); drv_io_set(0x55); PULSE_WEN();
  drv_addr_set(addr);   drv_io_set(0x30); PULSE_WEN();
  // Poll for completion
  drv_io_cfgread();
  SET_OEN(LOW);
    uint16_t time=0; while( (MSB(drv_io_get())!=128) && (time<2000) ) { time++; delayMicroseconds(100); } // time ~200us
  SET_OEN(HIGH);
  //Serial.print("sector erase "); Serial.print(time); Serial.println("00us");
  return time<1000;
  // flash and shift invariant (WEN pulsed, OEN restored)
}


// This functions erases the entire flash.  Erase means fill with 0xFF.
// Returns true iff erase completes in time (~700us).
bool drv_erase_chip() {
  // Write the chip-erase sequence
  drv_io_cfgwrite();
  drv_addr_set(0x5555); drv_io_set(0xaa); PULSE_WEN(); 
  drv_addr_set(0x2aaa); drv_io_set(0x55); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0x80); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0xaa); PULSE_WEN();
  drv_addr_set(0x2aaa); drv_io_set(0x55); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0x10); PULSE_WEN();
  // Poll for completion
  drv_io_cfgread();
  SET_OEN(LOW);
    uint16_t time=0; while( (MSB(drv_io_get())!=128) && (time<2000) ) { time++; delayMicroseconds(100); } // time ~700us
  SET_OEN(HIGH);
  //Serial.print("chip erase "); Serial.print(time); Serial.println("00us");
  return time<2000;
  // flash and shift invariant (WEN pulsed, OEN restored)
}


#define DRV_MANID_MICROCHIP 0xBF
#define DRV_DEVID_39SF010   0xB5
#define DRV_DEVID_39SF020   0xB6
#define DRV_DEVID_39SF040   0xB7


void drv_id(uint8_t * manid, uint8_t * devid) {
  // Write the id-entry sequence
  drv_io_cfgwrite();
  drv_addr_set(0x5555); drv_io_set(0xaa); PULSE_WEN(); 
  drv_addr_set(0x2aaa); drv_io_set(0x55); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0x90); PULSE_WEN();
  // Read the two IDs
  drv_io_cfgread();
  SET_OEN(LOW);
    drv_addr_set(0x00000);
    *manid= drv_io_get();
    drv_addr_set(0x00001);
    *devid= drv_io_get();
  SET_OEN(HIGH);
  // Write the id-exit sequence
  drv_io_cfgwrite();
  drv_addr_set(0x5555); drv_io_set(0xaa); PULSE_WEN(); 
  drv_addr_set(0x2aaa); drv_io_set(0x55); PULSE_WEN();
  drv_addr_set(0x5555); drv_io_set(0xF0); PULSE_WEN();
}


// grp  sectors  addresses    flash chip types
//  00  000-003  00000-03FFF  39SF010
//  01  004-007  04000-07FFF  39SF010
//  02  008-011  08000-0BFFF  39SF010
//  03  012-015  0C000-0FFFF  39SF010
//  04  016-019  10000-13FFF  39SF010
//  05  020-023  14000-17FFF  39SF010
//  06  024-027  18000-1BFFF  39SF010
//  07  028-031  1C000-1FFFF  39SF010
//  08  032-035  20000-23FFF  39SF010,39SF020
//  09  036-039  24000-27FFF  39SF010,39SF020
//  10  040-043  28000-2BFFF  39SF010,39SF020
//  11  044-047  2C000-2FFFF  39SF010,39SF020
//  12  048-051  30000-33FFF  39SF010,39SF020
//  13  052-055  34000-37FFF  39SF010,39SF020
//  14  056-059  38000-3BFFF  39SF010,39SF020
//  15  060-063  3C000-3FFFF  39SF010,39SF020
//  16  064-067  40000-43FFF  39SF010,39SF020,39SF040
//  17  068-071  44000-47FFF  39SF010,39SF020,39SF040
//  18  072-075  48000-4BFFF  39SF010,39SF020,39SF040
//  19  076-079  4C000-4FFFF  39SF010,39SF020,39SF040
//  20  080-083  50000-53FFF  39SF010,39SF020,39SF040
//  21  084-087  54000-57FFF  39SF010,39SF020,39SF040
//  22  088-091  58000-5BFFF  39SF010,39SF020,39SF040
//  23  092-095  5C000-5FFFF  39SF010,39SF020,39SF040
//  24  096-099  60000-63FFF  39SF010,39SF020,39SF040
//  25  100-103  64000-67FFF  39SF010,39SF020,39SF040
//  26  104-107  68000-6BFFF  39SF010,39SF020,39SF040
//  27  108-111  6C000-6FFFF  39SF010,39SF020,39SF040
//  28  112-115  70000-73FFF  39SF010,39SF020,39SF040
//  29  116-119  74000-77FFF  39SF010,39SF020,39SF040
//  30  120-123  78000-7BFFF  39SF010,39SF020,39SF040
//  31  124-127  7C000-7FFFF  39SF010,39SF020,39SF040


