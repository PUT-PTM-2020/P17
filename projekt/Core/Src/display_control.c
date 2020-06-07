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

uint8_t page = 0;
char *titles[5];
char *dir = "/";

void displayInit()
{
	ssd1306_Init();
	ssd1306_Fill(White);

	ssd1306_SetCursor(4, 2);
	ssd1306_WriteString("E:/",Font_6x8,Black);

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
	if(page==0)
	{
		if(strcmp(titles[0],"")==0) return 0;

			int st = strlen(dir);
			char *temp1 = strdup(titles[*pos]);
			char *temp2 = strdup(dir);
			char *temp3 = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
			strcpy(temp3,temp2);
			if(st>1)strcat(temp3,"/");
			strcat(temp3,temp1);
			/*dir = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
			strcpy(dir,temp2);
			if(st>1)strcat(dir,"/");
			strcat(dir,temp1);*/

			if(openFile(temp3,pFile)==FR_OK)
				{

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
		if(openFile(dir,pFile)==FR_OK)
			{

			}
			else if (scanDir(dir, titles, 5, page)==FR_OK)
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

				/*dir = (char*)malloc(strlen(temp2)+strlen(temp1)+2);
				strcpy(dir,temp2);
				if(st>1)strcat(dir,"/");
				strcat(dir,temp1);

				*pos = 0;
				res = 1;*/
				res = 1;
			}
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

void update(const uint8_t *pos)
{
	char *temp = "E:";
	temp = (char*)malloc(strlen(temp)+strlen(dir)+1);
	strcpy(temp,"E:");
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
		}
		ssd1306_DrawArrowR(3,16+(*pos * 10) ,Black);
	}
		//16+10pos
	ssd1306_DrawStrLine(0, 11, 130, 11, Black);
	ssd1306_UpdateScreen();

}

void scrollDown(uint8_t *pos)
{
	*pos = *pos + 1;
	if(*pos==5)
	{
		page++;
		*pos = 0;
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
