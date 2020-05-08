/*
 * file_system.h
 *
 *  Created on: 5 kwi 2020
 *      Author: kaper
 */

#ifndef INC_FILE_SYSTEM_H_
#define INC_FILE_SYSTEM_H_

#include "ff.h"
#include "stm32f4xx_hal.h"

#endif /* INC_FILE_SYSTEM_H_ */

//funkcja czytająca pliki w danej lokalizacji

FRESULT scanDir(char* path);

FRESULT openFile(char *fName, FIL* pFile);

FRESULT closeFile(FIL* pFile);

unsigned long setUp(FIL* pFile, I2S_HandleTypeDef *hi2s3, unsigned *ch);

FRESULT readData(uint16_t* data, FIL* pFile, unsigned size);

FRESULT fatInit(FATFS *FatFs);
