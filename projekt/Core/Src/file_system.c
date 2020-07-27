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


/* do playlist */

int kol_piosenka = 1; /* jako zmienna globalna potem przekazywana do funkcji readPlaylist (zaczynamy liczyć od 1) */
int liczbaLinii = 0; /* zmienna przechowujaca ile jest linii w pliku z playlista */
/* char nazw_play[255] = "p1.txt"; // przykładowa zmienna w której może być zapisana nazwa playlisty */
/* readPlaylist(nazw_play); // przykładowe wywołanie funkcji do odczytania kolejnej piosenki z playlisty */
/* kolPiosenka(); // wywolanie funkcji od playlist */


/* opis:
 * najpierw wywolywana jest funkcja do sprawdzenia ile jest linii w pliku,
 * potem jest odczytywana nazwa piosenki do zagrania (przy okazji jest sprawdzane czy
 * playlista dobiegla do konca jesli tak to jest wlaczana od nowa)
 *
 * (1)
 * Myslalam nad tym, ze playlista by byla grana tak dlugo dopoki nie zostanie naduszony jakis przycisk,
 * i ze po nacisnieciu tego przycisku nie wlaczy sie kolejna piosenka a ta co byla zostanie odtworzona do konca
 * (no albo, ze ta co byla tez zostanie przerwana ale to chyba wiecej do roboty by bylo).
 * Na razie mam stale zapetlenei funkcji zalezne od i, ale mozna od jakiegos przycisku.
 *
 * (2)
 * W tamtym miejscu trzeba dodac funkcje, która odtworzy ta piosenke, ale ja nie wiem ktora to funkcja. */


void liczbaLiniiFun(char nazw_play[255]) {
  FILE * wsk_plik;
  char buffer[255];
  if ((wsk_plik = fopen(nazw_play, "r")) != NULL) {
    while (fgets(buffer, 255, wsk_plik) != NULL) {
      liczbaLinii = liczbaLinii + 1;
    }
  }
  fclose(wsk_plik);
}

void kolPiosenka() {
  int i = 0;
  liczbaLiniiFun(nazw_play);

  while (i < 9) {						// (1)
    if (kol_piosenka > liczbaLinii) {
      kol_piosenka = 1;
      readPlaylist(nazw_play);
    } else {
      readPlaylist(nazw_play);
    }
    i = i + 1;							// (1)
  }
}

void readPlaylist(char nazw_play[255]) {
  FILE * wsk_plik;
  char buffer[255];
  int ile_lini = 1;

  if ((wsk_plik = fopen(nazw_play, "r")) != NULL) {
    //printf("Plik otwarto.\n");

    while (fgets(buffer, 255, wsk_plik) != NULL) {

      if (ile_lini == kol_piosenka) {
        //printf("%s\n", buffer); // do testów czy poprawnie odczytuje nazwe piosenki
        kol_piosenka = kol_piosenka + 1;
        ile_lini = 1;

        // teraz buffer trzeba przekazać do innej funkcji, żeby odtworzyła nazwę kolejnej do zagrania piosenki
        int i = 255;
        while (i--) ts[i] = buffer[i];

        // (2)

        break;
      }
      ile_lini = ile_lini + 1;
    }
  }

  fclose(wsk_plik);
}

/* koniec */


FRESULT scanDir(char* path, char *t[], uint8_t size, uint8_t page)
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
			if(res != FR_OK || fno.fname[0] == 0){
				t[i++]="";
			}
			else if(fno.fattrib & AM_SYS) continue;
			else{
				t[i] = strdup(fno.fname);
				i++;
				if(page > 0 && i == 5)
				{
					page--;
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

FRESULT openFile(char *fName, FIL* pFile)
{
	f_close (pFile);
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
		else
		{
			return FR_INVALID_PARAMETER;
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

//FRESULT readData(uint16_t *data1, uint16_t *data2, FIL* pFile, unsigned size)
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
