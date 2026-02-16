// cmdflash.h - command handler "flash" to view/control the size of flash
#ifndef _CMDFLASH_H_
#define _CMDFLASH_H_


uint16_t cmdflash_chipsize_k();
uint16_t cmdflash_groupsize_sectors();
uint16_t cmdflash_sectorsize_k();


int cmdflash_register(void);


#endif