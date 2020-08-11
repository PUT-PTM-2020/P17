/*
 * display_control.c
 *
 *  Created on: May 28, 2020
 *      Author: kaper
 */

/* EDITED - funkcja do dodawania piosenek do playlisty, funkcja do usuwania playlisty */
// linijki: 214-238 

#include "ssd1306.h"
#include "file_system.h"
#include "ff.h"
#include <string.h>

uint8_t page = 0;
//aktualne 5 plikow
char *titles[5];
char *tracks[5];
//sciezka
char *dir = "/";
char directory[256] = "/";
char currentTitle[50];

char currentPlaylist[50];
extern uint8_t mode;
uint8_t viewMode;

void displayInit()
{
	ssd1306_Init();
	ssd1306_Fill(White);

	ssd1306_SetCursor(4, 2);
	ssd1306_WriteString("F:/",Font_6x8,Black);

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



int open(const uint8_t *pos, FIL pFile[2])
{
	FRESULT fResult;
	unsigned snl = strlen(titles[*pos]);
	int res = 0;

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
			fResult = f_open(&pFile[0], temp, FA_READ );
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
	{
		if(strcmp(titles[0],"")==0) return 0;

		int st = strlen(dir);
		char *temp1 = strdup(titles[*pos]);
		char *temp2 = strdup(dir);
		char *temp3 = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
		strcpy(temp3,temp2);
		if(st>1) strcat(temp3,"/");
		strcat(temp3,temp1);

		if(pos==0x5)
		{
			scanDir(dir, titles, 5, page);
		}
		else if(res=openFile(temp3,pFile))
		{
			//HAL_Delay(1);
		}
		else if (scanDir(temp3, titles, 5, page)==FR_OK)
		{
		//
			int st = strlen(dir);
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
			}
		//
			dir = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
			strcpy(dir,temp2);
			if(st>1)strcat(dir,"/");
			strcat(dir,temp1);

			//*pos = 0;
			res = 4;
		}
		else
		{
			//scanDir("/FOLDER2/FOLDER3", titles, 5, page);
			//*pos = 0;
			res = 0;
		}
	}
	else
	{
		if(pos==0x5)
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

			if(res=openFile(temp3,pFile))
			{

			}
			else if (scanDir(temp3, titles, 5, page)==FR_OK)
			{
				res = 4;
			}
		}
	}


	return res;*/
}

void updateDir()
{
	scanDir(directory,titles,5,page);
	//scanDir(dir,titles,5,page);
}

/// NEW 
void deletePlaylist(const uint8_t *pos)
{
	FRESULT fResult;
	char temp[256]="";
	strcpy(temp,directory);
	strcat(temp,"/");
	strcat(temp,titles[*pos]);

	int test = 0;

	char tmp[5];
	int snl = strlen(titles[*pos]);
	memcpy(tmp,&titles[*pos][snl-4],4);
	tmp[4]='\0';
	if(test = strcmp(tmp,".txt\0")==0)
	{
		fResult = f_unlink(temp); 		// usunięcie playlisty
		updateDir();
		update(pos);
	}
}

/// NEW 
void addSong(const uint8_t *pos, FIL *pFile)
{	
	UINT bw;
	UINT bww;
	FRESULT fResult;
	char temp[256]="";
	strcpy(temp,directory);
	strcat(temp,titles[*pos]);
	
	int snl = strlen(titles[*pos]);

	char tmp[5];
	memcpy(tmp,&titles[*pos][snl-4],4);
	tmp[4]='\0';
	if(strcmp(tmp,".wav") || strcmp(tmp,".mp3"))
	{
		fResult = f_write(pFile,temp,strlen(temp),&bw);
		fResult = f_write(pFile,"\n",1,bw);
	}
	//char playDir[256];
	//stpcpy(playDir, "/playlists/plist\0");
	//strcat(playDir, playName);
	//FILE *plik;
	//plik = fopen(playDir, temp);	// wpisanie do playlisty piosenki
	//f_write(pFile,titles[*pos],strlen(titles[*pos]),bw);

}

void goBack(uint8_t *pos)
{
	*pos = 0;
	//char *temp = strdup(dir);
	//int s = strlen(dir);
	if(viewMode==0)
	{
		int s = strlen(directory);
		for(int i=s;i>=0;i--)
		{
			if(directory[i]=='/')
			{
				//char *str;
				if(i>0) directory[i]='\0';
				else directory[i+1] = '\0';
				//memmove(dir,dir,s);
				scanDir(directory, titles, 5, page);
				//scanDir(dir, titles, 5, page);
				HAL_Delay(1);
				break;
			}
		}
	}
	else
	{
		viewMode = 0;
		updateDir();
	}

	/*while(s>0)
	{

	}*/
}

void update(uint8_t *pos)
{
	char *temp = "F:";
	//temp = (char*)malloc(strlen(temp)+strlen(dir)+1);
	temp = (char*)malloc(strlen(temp)+strlen(directory)+1);
	strcpy(temp,"F:");
	strcat(temp,directory);
	ssd1306_Fill(White);
	ssd1306_SetCursor(4, 2);
	if(mode==0)
	{
		ssd1306_WriteString(temp,Font_6x8,Black);
	}
	else
	{
		ssd1306_WriteString(currentTitle,Font_6x8,Black);
	}
	//HAL_Delay(1);
	if(viewMode==0)
	{
		if(strcmp(titles[0],"")==0)
		{
			ssd1306_SetCursor(10, 15);
			ssd1306_WriteString("--Empty--",Font_6x8,Black);
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
	}
	else
	{
		if(strcmp(tracks[0],"")==0)
		{
			ssd1306_SetCursor(10, 15);
			ssd1306_WriteString("--Empty List--",Font_6x8,Black);
		}
		else
		{
			char temp2[50]="";
			char temp3[100]="";
			for(int i = 15,j=0;i<=55;i+=10,j++)
			{
				strcpy(temp2,"");
				ssd1306_SetCursor(10, i);

				int s = strlen(tracks[j]);
				strcpy(temp3,tracks[j]);

				for(int k=s;k>=0;k--)
				{
					if(temp3[k]=='/')
					{
						//char *str;
						//if(k>0)
						//{
							memcpy(temp2,&temp3[k+1],s-1-k);
						//}
						//else directory[i+1] = '\0';
						//memmove(dir,dir,s);
						//scanDir(directory, titles, 5, page);
						//scanDir(dir, titles, 5, page);
						//HAL_Delay(1);
						break;
					}
				}

				ssd1306_WriteString(temp2,Font_6x8,Black);
				//ssd1306_WriteString(tracks[j],Font_6x8,Black);
				//if(strcmp(titles[(i/10)-1],"")!=0) *pos = (i/10)-1;
			}
			ssd1306_DrawArrowR(3,16+(*pos * 10) ,Black);
		}
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
	else if(viewMode==0)
	{
		if(strcmp(titles[*pos+1],"")!=0)
		{
			*pos = *pos + 1;
		}
	}
	else if(viewMode==1)
	{
		if(strcmp(tracks[*pos+1],"")!=0)
		{
			*pos = *pos + 1;
		}
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
int forceFile(int *pos, FIL* pFile)
{
	FRESULT fResult;
	int res = 0;

	fResult = f_open(pFile,tracks[*pos],FA_READ);
	if(fResult == FR_OK)
	{
		int snl = strlen(tracks[*pos]);
		char tmp[5];
		memcpy(tmp,&tracks[*pos][snl-4],4);
		tmp[4]='\0';
		if(strcmp(tmp,".wav")==1)
		{
			res = 1;
		}
		else if(strcmp(tmp,".mp3")==1)
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
