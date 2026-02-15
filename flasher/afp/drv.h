// drv.cpp - driver for the 39SF010, 39SF020, and 39SF040 flash chips
#ifndef _DRV_H_
#define _DRV_H_


#include <stdint.h>


#define DRV_MANID_MICROCHIP 0xBF
#define DRV_DEVID_39SF010   0xB5
#define DRV_DEVID_39SF020   0xB6
#define DRV_DEVID_39SF040   0xB7
void drv_id(uint8_t * manid, uint8_t * devid);

void drv_init();

void drv_io_read(uint32_t addr, uint8_t * io, uint8_t size);
bool drv_io_write(uint32_t addr, uint8_t io);
bool drv_erase_sector(uint32_t addr);
bool drv_erase_chip();

void drv_led_set(uint8_t state);


#endif


// grp  sectors  addresses    flash chip types
// ---  -------  -----------  ------------------------
//  00  000-003  00000-03FFF  39SF010
//  01  004-007  04000-07FFF  39SF010
//  02  008-011  08000-0BFFF  39SF010
//  03  012-015  0C000-0FFFF  39SF010
//  04  016-019  10000-13FFF  39SF010
//  05  020-023  14000-17FFF  39SF010
//  06  024-027  18000-1BFFF  39SF010
//  07  028-031  1C000-1FFFF  39SF010
// ---  -------  -----------  ------------------------
//  08  032-035  20000-23FFF  39SF010,39SF020
//  09  036-039  24000-27FFF  39SF010,39SF020
//  10  040-043  28000-2BFFF  39SF010,39SF020
//  11  044-047  2C000-2FFFF  39SF010,39SF020
//  12  048-051  30000-33FFF  39SF010,39SF020
//  13  052-055  34000-37FFF  39SF010,39SF020
//  14  056-059  38000-3BFFF  39SF010,39SF020
//  15  060-063  3C000-3FFFF  39SF010,39SF020
// ---  -------  -----------  ------------------------
//  16  064-067  40000-43FFF  39SF010,39SF020,39SF040
//  17  068-071  44000-47FFF  39SF010,39SF020,39SF040
//  18  072-075  48000-4BFFF  39SF010,39SF020,39SF040
//  19  076-079  4C000-4FFFF  39SF010,39SF020,39SF040
//  20  080-083  50000-53FFF  39SF010,39SF020,39SF040
//  21  084-087  54000-57FFF  39SF010,39SF020,39SF040
//  22  088-091  58000-5BFFF  39SF010,39SF020,39SF040
//  23  092-095  5C000-5FFFF  39SF010,39SF020,39SF040
// ---  -------  -----------  ------------------------
//  24  096-099  60000-63FFF  39SF010,39SF020,39SF040
//  25  100-103  64000-67FFF  39SF010,39SF020,39SF040
//  26  104-107  68000-6BFFF  39SF010,39SF020,39SF040
//  27  108-111  6C000-6FFFF  39SF010,39SF020,39SF040
//  28  112-115  70000-73FFF  39SF010,39SF020,39SF040
//  29  116-119  74000-77FFF  39SF010,39SF020,39SF040
//  30  120-123  78000-7BFFF  39SF010,39SF020,39SF040
//  31  124-127  7C000-7FFFF  39SF010,39SF020,39SF040
// ---  -------  -----------  ------------------------

