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

FRESULT scanDir(const char* path, char **t, uint8_t size, uint8_t page);

int openFile(char *fName, FIL* pFile);

FRESULT closeFile(FIL* pFile);

unsigned long setUp(FIL* pFile, I2S_HandleTypeDef *hi2s3, unsigned *ch);

FRESULT readData(uint32_t* data, FIL* pFile, unsigned size);
//FRESULT readData(uint16_t* data1, uint16_t *data2, FIL* pFile, unsigned size);

FRESULT fatInit(FATFS *FatFs);

//tworzy playliste w folderze playlist
void createPlaylist();

//to samo co scandir
void scanPlaylist(char *t[], uint8_t size, uint8_t page, FIL *pFile);

void howLinesFun();

void nextSong(int * nextsong);

void readPlaylist(int * nextsong);
