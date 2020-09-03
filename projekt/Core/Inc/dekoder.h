/*
 * dekoder.h
 *
 *  Created on: 17 sie 2020
 *      Author: kaper
 */

#ifndef INC_DEKODER_H_
#define INC_DEKODER_H_

unsigned int RetrieveMP3Data (void * MP3CompressedData, unsigned int MP3DataSizeInChars, void * token);
void MP3Init(char* filename);
int MP3D();
void closemp3();

#endif /* INC_DEKODER_H_ */


