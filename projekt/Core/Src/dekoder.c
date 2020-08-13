/*
 * Dekodowanie MP3
 * Oskar
 */

#include "spiritMP3Dec.h"
#include <stdio.h>

//Wielkosc dekodowanej ramki (podobno najlepiej wielokrotnosc 576)
#define frame_size_in_samples 576

//Bufor wychodzacy ze zdekodowanymi danymi
short PCMSamples [2*frame_size_in_samples];

//Informacje potrzebne do poprawnego odtworzenia muzyki
TSpiritMP3Info MP3Info;
unsigned int FrequencyHz;
unsigned int BitrateKbps;
unsigned int Channels;

//Dekoder
TSpiritMP3Decoder MP3Decoder;

//zmienna dla plikow MP3
FILE * MP3file;

//Funkcja czytajaca dane MP3, wykorzystywana przez dekoder
unsigned int RetrieveMP3Data (void * MP3CompressedData, unsigned int MP3DataSizeInChars, void * token)
{
	return f_read(MP3CompressedData, sizeof(char), MP3DataSizeInChars, (FILE*) token);
}

//Laczymy dekoder z podanym plikiem - uruchamiane raz przed pierwszym dekodowaniem
void MP3Init(char* filename)
{
	MP3file = fopen(*filename,"rb");
	SpiritMP3DecoderInit(&MP3Decoder, RetrieveMP3Data, NULL, MP3file);

}

//Funkcja dekodujaca ramke z zainicjalizowanego wczesniej pliku
//Przy wywolaniu sprawdzac wynik dzialania
//0 - udalo sie zdekodowac fragment, nie zmieniac pliku wejsciowego
//1 - nie udalo sie zdekodowac fragmentu, zmienic plik wejsciowy
int MP3Decode()
{
	//dekodowanie do bufora PCMSamples ramki o podanej wielkosci
	if (SpiritMP3Decode(&MP3Decoder, PCMSamples, frame_size_in_samples, MP3Info))
	{
		FrequencyHz = MP3Info.nSampleRateHz;
		BitrateKbps = MP3Info.nBitrateKbps;
		Channels = MP3Info.nChannels;
		//tutaj nastepuje przeslanie parametrow
		//tutaj odczytanie danych z PCMSamples

		return 0;
	}
	else
	{
		fclose(MP3file);
		return 1;
	}
}
