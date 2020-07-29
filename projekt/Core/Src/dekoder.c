/*
 * Plik c zawierajacy funkcje zwiazane z dekoderem MP3 do
 * pozniejszego zastosowania w projekcie wraz z opisami
 * Oskar
 */

/*#include "mp3dec.h"

//inicjalizacja dekodera
HMP3Decoder mp3decoder;
mp3decoder = MP3InitDecoder();

//bufor odczytujacy
#define READ_BUFFER_SIZE 2 * MAINBUF_SIZE + 216 //ponad dwa razy wiekszy od bufora dekodera
unsigned char read_buffer[READ_BUFFER_SIZE];

//bufor zapisujacy
#define DECODED_MP3_FRAME_SIZE MAX_NGRAN * MAX_NCHAN * MAX_NSAMP
#define OUT_BUFFER_SIZE 2 * DECODED_MP3_FRAME_SIZE //rozmiar dwa razy wiekszy od zdekodowanej ramki
short out_buffer[OUT_BUFFER_SIZE];

int offset; //offset do nastepnego bajtu synchronizujacego
int result; //wynik dekodowania, 0 = udane, <0 nieudane

offset = MP3FindSyncWord(read_pointer, bytes_left); //znajdz poczatek ramki
	while(offset < 0) //jesli blad, szukaj ponownie
	{
		if(refill_inbuffer(mp3_file) != 0)
		{
			return END_OF_FILE;
		}
		if(bytes_left > 0)
		{
			bytes_left -= 1;
			read_pointer += 1;
		}
		offset = MP3FindSyncWord(read_pointer, bytes_left);
	}
	read_pointer += offset; //przesun wskaznik odczytu
	bytes_left -= offset;
	bytes_left_before_decoding = bytes_left;

	if (bytes_left < MAINBUF_SIZE) { //jesli trzeba uzupelniamy bufor wejsciowy
		if(refill_inbuffer(mp3_file) != 0)
			return END_OF_FILE;
	}

	result = MP3Decode(mp3decoder, &read_pointer,	&bytes_left, out_buffer, 0); //dekodowanie ramki
*/
