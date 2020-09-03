/*
 * Plik c zawierajacy funkcje zwiazane z dekoderem MP3 do
 * pozniejszego zastosowania w projekcie wraz z opisami
 * Oskar
 */

<<<<<<< Updated upstream
#include "mp3dec.h"

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
=======
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
static unsigned int RetrieveMP3Data(void * MP3CompressedData, unsigned int MP3DataSizeInChars, void * token)
{
	return fread(MP3CompressedData, sizeof(char), MP3DataSizeInChars, (FILE*) token);
}

//Laczymy dekoder z podanym plikiem - uruchamiane raz przed pierwszym dekodowaniem
void MP3Init(char* filename)
{
	MP3file = fopen(filename,"rb");
	//SpiritMP3DecoderInit(&MP3Decoder, RetrieveMP3Data, NULL, NULL);

}


//Funkcja dekodujaca ramke z zainicjalizowanego wczesniej pliku
//Przy wywolaniu sprawdzac wynik dzialania
//1 - udalo sie zdekodowac fragment, nie zmieniac pliku wejsciowego
//0 - nie udalo sie zdekodowac fragmentu, zmienic plik wejsciowy
int MP3D()
{

	//dekodowanie do bufora PCMSamples ramki o podanej wielkosci
	if (SpiritMP3Decode(&MP3Decoder, PCMSamples, frame_size_in_samples, &MP3Info))
	{
		FrequencyHz = MP3Info.nSampleRateHz;
		BitrateKbps = MP3Info.nBitrateKbps;
		Channels = MP3Info.nChannels;
		//tutaj nastepuje przeslanie parametrow
		//tutaj odczytanie danych z PCMSamples

		return 1;
	}
	else
	{
		fclose(MP3file);
		return 0;
	}
}

void closemp3()
{
	fclose(MP3file);
}
>>>>>>> Stashed changes
