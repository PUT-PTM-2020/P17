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

//obiekt pliku playlisty
FIL playlist;

//tutaj se odczytuje daną lokalizacje/zawartość folderu
//*t[] tablica tablic czyli tablica stringów z tytułami/nazwami
FRESULT scanDir(const char* path, char *t[], uint8_t size, uint8_t page)
{
	int i = 0;
	FRESULT res;
	DIR dir;
	static FILINFO fno;
	volatile int ile = 0;

	res = f_opendir(&dir,path);
	if(res == FR_OK)
	{
		dir.dptr = 0;
		while(i<size)
		{
			res = f_readdir(&dir,&fno);
			if(res != FR_OK || fno.fname[0] == 0)
			{
				t[i++]="";
			}
			else if(fno.fattrib & AM_SYS) continue;
			else
			{
				t[i] = strdup(fno.fname);
				i++;
				if(page > 0 && i == 5)
				{
					page=page-1;
					i=0;
				}
				continue;
			}
		}
		if(i<size)
		{

		}
	}
	f_closedir(&dir);
	return res;
}

int openMFile(FIL* mFile)
{

}

int openPFile(FIL* pFile)
{
	FRESULT fResult;

}

//otwiera plik
int openFile(char *fName, FIL* pFile)
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
		if(strcmp(tmp,".WAV")==0)
		{
			mOpen = 1;
			f_close (pFile);
			fResult = f_open(pFile, fName, FA_READ );
			return 1;
		}
		else if(strcmp(tmp,".MP3")==0)
		{
			mOpen = 1;
			f_close (pFile);
			fResult = f_open(pFile, fName, FA_READ );
			return 2;
		}
		else if(strcmp(tmp,".TXT")==0)
		{
			pOpen = 1;
			return 3;
		}
		else
		{
			return 0;
		}

		HAL_Delay(1);
	}
	else return 0;
}

FRESULT closeFile(FIL* pFile)
{
	FRESULT fResult = f_close (&pFile);
	return fResult;
}

//to odczytuje nagłówek .wav i tam ustawienia robi
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

//FRESULT readData(uint16_t *data1, uint16_t *data2, FIL* pFile, unsigned size)
FRESULT readData(uint32_t *data, FIL* pFile, unsigned size)
{
	UINT br;
	FRESULT fResult = f_read(pFile,data,size,&br);



	return fResult;
}

//fat init
FRESULT fatInit(FATFS *fat)
{
	FRESULT fResult = f_mount(fat, "", 0);
	return fResult;
}


void createPlaylist()
{
	FRESULT fResult;
	FILINFO fIleinfo;

	int i = 1;
	char temp1[50] = "/playlists/plist\0";
	char temp2[50] = "";
	char temp3[2];

	while(1)
	{
		sprintf(temp3,"%i",i);
		strcpy(temp2,temp1);
		strcat(temp2,temp3);
		strcat(temp2,".txt");
		fResult = f_stat(temp2,&fIleinfo);
		if(fResult==FR_NO_FILE)
		{
			FIL fpl;
			fResult = f_open(&fpl,temp2,FA_CREATE_NEW);
			break;
		}
		i++;
	}
}

void scanPlaylist(const char* path, char *t[], uint8_t size, uint8_t page, FIL *pFile)
{
	int i = 0;


	char buffer[256]="";

	pFile->fptr = 0;
	while(i<size)
	{
		f_gets(buffer,256,pFile);
		t[i] = strdup(buffer);
		i++;
		if(page>0 && i==5)
		{
			page=page-1;
			i=0;
		}
	}


}


