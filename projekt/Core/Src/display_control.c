/*
 * display_control.c
 *
 *  Created on: May 28, 2020
 *      Author: kaper
 */

#include "ssd1306.h"
#include "file_system.h"
#include "ff.h"
#include <string.h>
#include <stdio.h>

uint8_t page = 0;
//aktualne 5 plikow
char *titles[5];
//sciezka
char *dir = "/";
char currentTitle[50];

void displayInit()
{
	ssd1306_Init();
	ssd1306_Fill(White);

	ssd1306_SetCursor(4, 2);
	ssd1306_WriteString("0:/",Font_6x8,Black);

	scanDir(dir, titles, 5, page);

	for(int i = 15,j=0;i<=55;i+=10,j++)
	{
		ssd1306_SetCursor(10, i);
		ssd1306_WriteString(titles[j],Font_6x8,Black);
	}

	ssd1306_DrawArrowR(3,16,Black);	//16+10pos
	ssd1306_DrawStrLine(0, 11, 130, 11, Black);
	ssd1306_UpdateScreen();
}

int open(uint8_t *pos, FIL* pFile)
{
	int res = 0;
<<<<<<< Updated upstream
	if(page==0)
=======

	char temp[256];
	strcpy(temp,directory);
	if(strlen(temp)>1) strcat(temp,"/");
	strcat(temp,titles[*pos]);
	/*int st = strlen(dir);
	char *temp = NULL;
	if(st>1) *temp = (char*)malloc(strlen(dir)+strlen(titles[*pos])+2);

	strcpy(temp,dir);
	if(st>1) strcat(temp,"/");
	strcat(temp,titles[*pos]);
	strcat(temp,'\0');*/

	/*char *temp1 = strdup(titles[*pos]);
	char *temp2 = strdup(dir);
	char *temp3 = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
	strcpy(temp3,temp2);
	if(st>1) strcat(temp3,"/");
	strcat(temp3,temp1);*/

	if(snl>5)
	{
		char tmp[5];
		memcpy(tmp,&titles[*pos][snl-4],4);
		tmp[4]='\0';
		if(strcmp(tmp,".wav")==0)
		{
			//wav
			fResult = f_close(&pFile[0]);
			fResult = f_open(&pFile[0], temp, FA_READ );
			strcpy(currentTitle,titles[*pos]);
			strcpy(currentPlaylist,"");
			res = 1;
		}
		else if(strcmp(tmp,".mp3")==0)
		{
			//mp3
			fResult = f_close(&pFile[0]);
			//closemp3();
			//MP3Init(temp);
			//*mFile = fopen(temp,"rb");
			//fResult = f_open(&pFile[0], temp, FA_READ );
			strcpy(currentTitle,titles[*pos]);
			strcpy(currentPlaylist,"");
			res = 2;
		}
		else if(strcmp(tmp,".txt")==0)
		{
			//playlista
			fResult = f_close(&pFile[1]);
			fResult = f_open(&pFile[1], temp, FA_READ );
			strcpy(currentPlaylist,"/playlists/");
			strcat(currentPlaylist,titles[*pos]);
			scanPlaylist(tracks, 5, page, &pFile[1]);
			//*pos=0;
			res = 3;
		}
		else
		{
			if(strcmp(titles[*pos],"")!=0)
			{
				strcpy(directory,temp);
			}
			page = 0;
			//folder
			/*dir = (char*)malloc(strlen(temp));
			strcpy(dir,temp);
			int ddd = strlen(dir);*/
			//dir[strlen(dir)] = '\0';
			res = 4;
		}
	}
	return res;


	/*if(page==0)
>>>>>>> Stashed changes
	{
		if(strcmp(titles[0],"")==0) return 0;

		int st = strlen(dir);
		char *temp1 = strdup(titles[*pos]);
		char *temp2 = strdup(dir);
		char *temp3 = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
		strcpy(temp3,temp2);
		if(st>1) strcat(temp3,"/");
		strcat(temp3,temp1);

		if(openFile(temp3,pFile)==FR_OK)
		{
			HAL_Delay(1);
		}
		else if (scanDir(temp3, titles, 5, page)==FR_OK)
		{
			/*int st = strlen(dir);
			if(st==1)
			{
				dir = (char*)malloc(strlen(dir)+strlen(temp)+1);
				strcpy(dir,"/");
				strcat(dir,temp);
			}
			else if(st>1)
			{
				dir = (char*)malloc(strlen(dir)+strlen(temp)+2);
				strcat(dir,"/");
				strcat(dir,temp);
			}*/

			dir = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
			strcpy(dir,temp2);
			if(st>1)strcat(dir,"/");
			strcat(dir,temp1);

			//*pos = 0;
			res = 1;
		}
		else
		{
			//scanDir("/FOLDER2/FOLDER3", titles, 5, page);
			//*pos = 0;
			res = 1;
		}
	}
	else
	{
		if(*pos=='T')
		{
			scanDir(dir, titles, 5, page);
		}
		else
		{
			int st = strlen(dir);
			char *temp1 = strdup(titles[*pos]);
			char *temp2 = strdup(dir);
			char *temp3 = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
			strcpy(temp3,temp2);
			if(st>1) strcat(temp3,"/");
			strcat(temp3,temp1);

			if(openFile(temp3,pFile)==FR_OK)
			{

			}
			else if (scanDir(temp3, titles, 5, page)==FR_OK)
			{

			}
		}

			//else if (scanDir(temp3, titles, 5, page)==FR_OK)
			//{
				/*int st = strlen(dir);
				if(st==1)
				{
					dir = (char*)malloc(strlen(dir)+strlen(temp)+1);
					strcpy(dir,"/");
					strcat(dir,temp);
				}
				else if(st>1)
				{
					dir = (char*)malloc(strlen(dir)+strlen(temp)+2);
					strcat(dir,"/");
					strcat(dir,temp);
				}*/

				/*dir = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
				strcpy(dir,temp2);
				if(st>1)strcat(dir,"/");
				strcat(dir,temp1);

				*pos = 0;
				res = 1;*/
			//	res = 1;
			//}
			/*else
			{
				//scanDir("/FOLDER2/FOLDER3", titles, 5, page);
				*pos = 0;
				res = 1;
			}*/
	}


	return res;
}

void goBack(uint8_t *pos)
{
	*pos = 0;
	char *temp = strdup(dir);
	int s = strlen(dir);
	for(int i=s;i>=0;i--)
	{
		if(dir[i]=='/')
		{
			//char *str;
			if(i>0) dir[i]='\0';
			else dir[i+1] = '\0';
			//memmove(dir,dir,s);
			scanDir(dir, titles, 5, page);
			HAL_Delay(1);
			break;
		}
	}

	/*while(s>0)
	{

	}*/
}

void update( uint8_t *pos)
{
	char *temp = "0:";
	temp = (char*)malloc(strlen(temp)+strlen(dir)+1);
	strcpy(temp,"0:");
	strcat(temp,dir);
	ssd1306_Fill(White);
	ssd1306_SetCursor(4, 2);
	ssd1306_WriteString(temp,Font_6x8,Black);
	HAL_Delay(1);
	if(strcmp(titles[0],"")==0)
	{
		ssd1306_SetCursor(10, 15);
		ssd1306_WriteString("--Pusto--",Font_6x8,Black);
	}
	else
	{
		for(int i = 15,j=0;i<=55;i+=10,j++)
		{
			ssd1306_SetCursor(10, i);
			ssd1306_WriteString(titles[j],Font_6x8,Black);
			//if(strcmp(titles[(i/10)-1],"")!=0) *pos = (i/10)-1;
		}
		ssd1306_DrawArrowR(3,16+(*pos * 10) ,Black);
	}
		//16+10pos
	ssd1306_DrawStrLine(0, 11, 130, 11, Black);
	ssd1306_UpdateScreen();

}

void scrollDown(uint8_t *pos)
{

	if(*pos==4)
	{
		page++;
		*pos = 0;
	}
	else if(strcmp(titles[*pos+1],"")!=0)
	{
		*pos = *pos + 1;
	}
}
void scrollUp(uint8_t *pos)
{
	if(*pos>0 && *pos <255)
	{
		*pos=*pos -1;
	}
	else
	{
		if(page>0)
		{
			*pos=4;
			page--;
			scanDir(dir, titles, 5, page);
		}
	}
}
<<<<<<< Updated upstream
=======

void changeMode(int *mode)
{
	*mode=(*mode+1)%2;
}
void changeView(int x)
{
	page = 0;
	viewMode = x;
}

//otwarcie dowolnego pliku w wyznaczonej ścieżce
int forceFile(uint8_t *pos, FIL* pFile)
{
	FRESULT fResult;
	int res = 0;
	fResult = f_close(pFile);
	fResult = f_open(pFile,tracks[*pos],FA_READ);
	if(fResult == FR_OK)
	{
		int snl = strlen(tracks[*pos]);
		char tmp[5];
		memcpy(tmp,&tracks[*pos][snl-4],4);
		tmp[4]='\0';
		if(strcmp(tmp,".wav")==0)
		{
			res = 1;
		}
		else if(strcmp(tmp,".mp3")==0)
		{
			res = 2;
		}
	}
	return res;
}

//skok do wyznaczonej lokalizacji
void forceDir(char* fDir)
{
	strcpy(directory,fDir);
}
void resetPL(uint8_t *pos)
{
	page = 0;
	*pos = 0;
}
>>>>>>> Stashed changes
