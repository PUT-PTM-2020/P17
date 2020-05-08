/*
 * file_system.c
 *
 *  Created on: 5 kwi 2020
 *      Author: kaper
 */
#include "ff.h"
#include "file_system.h"
#include "stm32f4xx_hal.h"

int pOpen = 0;
int mOpen = 0;

FRESULT scanDir(char* path)
{
	FRESULT result;
	DIR directory;

}

FRESULT openFile(char *fName, FIL* pFile)
{
	unsigned snl = strlen(fName);
	FRESULT fResult;
	UINT r;
	if(snl>5)
	{
		char tmp[5];
		//memcpy(tmp,&fName[sizeof(fName)-5],4);
		memcpy(tmp,&fName[snl-4],4);
		tmp[4]='\0';
		if(strcmp(tmp,".wav")==0)
		{
			mOpen = 1;
		}
		else if(strcmp(tmp,".mp3")==0)
		{
			mOpen = 1;
		}
		else if(strcmp(tmp,".txt")==0)
		{
			pOpen = 1;
		}
		fResult = f_open(pFile, fName, FA_READ );
	}
	else fResult = FR_INVALID_PARAMETER;
	return fResult;
}

FRESULT closeFile(FIL* pFile)
{
	FRESULT fResult = f_close (&pFile);
	return fResult;
}

unsigned long setUp(FIL* pFile, I2S_HandleTypeDef *hi2s3, unsigned *ch)
{
	unsigned br;
	FRESULT f;
	uint8_t data1[1];
	uint16_t data2[1];
	uint32_t data4[1];
	for (int i = 0; i < 4; i++)
	{
		f_read(pFile,data1,1,&br);
	}
	f_read(pFile,data4,4,&br);
	f_read(pFile,data4,4,&br);
	f_read(pFile,data4,4,&br);
	f_read(pFile,data4,4,&br);
	f_read(pFile,data2,2,&br);
	f_lseek(pFile,22);

	f_read(pFile,data2,2,&br);

	*ch = (unsigned)data2[0];

	f_read(pFile,data4,4,&br);
	switch(data4[0])
	{
	case 44100:
		hi2s3->Init.AudioFreq = I2S_AUDIOFREQ_44K;
		break;
	case 32000:
		hi2s3->Init.AudioFreq = I2S_AUDIOFREQ_32K;
		break;
	case 22050:
		hi2s3->Init.AudioFreq = I2S_AUDIOFREQ_22K;
		break;
	case 16000:
		hi2s3->Init.AudioFreq = I2S_AUDIOFREQ_16K;
		break;
	case 11025:
		hi2s3->Init.AudioFreq = I2S_AUDIOFREQ_11K;
		break;
	case 8000:
		hi2s3->Init.AudioFreq = I2S_AUDIOFREQ_8K;
		break;
	default:
		break;
	}
	f_lseek(pFile, 34);
	// 8 | 16
	f_read(pFile,data2,2,&br);
	f_lseek(pFile, 40);
	f_read(pFile,data4,4,&br);
	unsigned long sdata = data4[0];
	return sdata;
}

FRESULT readData(uint16_t *data, FIL* pFile, unsigned size)
{
	UINT br;
	FRESULT fResult = f_read(pFile,data,size,&br);
	return fResult;
}

FRESULT fatInit(FATFS *fat)
{
	FRESULT fResult = f_mount(fat, "", 0);
	return fResult;
}
