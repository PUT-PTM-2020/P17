/*
 * display_control.h
 *
 *  Created on: May 28, 2020
 *      Author: kaper
 */

#ifndef INC_DISPLAY_CONTROL_H_
#define INC_DISPLAY_CONTROL_H_

#include <stdio.h>

#endif /* INC_DISPLAY_CONTROL_H_ */

void displayInit();
void update(uint8_t *pos);
void scrollDown(uint8_t *pos);
void scrollUp(uint8_t *pos);
int open(uint8_t *pos, FIL* pFile);
void goBack(uint8_t *pos);
<<<<<<< Updated upstream
=======
void updateDir();
void changeMode(int *mode);
void changeView(int x);
void addSong(const uint8_t *pos, FIL *pFile);
void deletePlaylist(const uint8_t *pos);
int forceFile(uint8_t *pos, FIL* pFile);
void resetPL(uint8_t *pos);
>>>>>>> Stashed changes
