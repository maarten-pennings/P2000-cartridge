// cmdflash.h - command handler "flash" to view/control the size of flash
#ifndef _CMDFLASH_H_
#define _CMDFLASH_H_


uint32_t cmdflash_chipsize();
uint32_t cmdflash_romsize();
uint32_t cmdflash_sectorsize();


int cmdflash_register(void);


#endif
