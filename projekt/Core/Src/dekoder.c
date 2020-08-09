/*
 * Proces dekodowania wraz objasnieniami i propozycja implementacji
 * Oskar
 */

#include "spiritMP3Dec.h"
#include <stdio.h>

//Wielkosc dekodowanej ramki (podobno najlepiej wielokrotnosc 576)
#define frame_size_in_samples 576

//Bufor wychodzacy ze zdekodowanymi danymi
short PCMSamples [2*frame_size];

//Rozroznienie pierwszej ramki do odczytu informacji do odtwarzania muzyki
unsigned int first_frame = 0;

//Informacje potrzebne do poprawnego odtworzenia muzyki
TSpiritMP3Info MP3Info;
unsigned int FrequencyHz;
unsigned int BitrateKbps;
unsigned int Channels;

//Dekoder
TSpiritMP3Decoder MP3Decoder;

//Funkcja czytajaca dane MP3, wykorzystywana przez dekoder
unsigned int RetrieveMP3Data (void * MP3CompressedData, unsigned int MP3DataSizeinChars, void * token)
{
	return fread(MP3CompressedData, sizeof(char), MP3DataSizeInChars, (FILE*) token);
}

//zawartosc maina u nas bylaby zapewne w petli glownej, albo po "ifie"
//sprawdzajacym czy odtwarzany plik to .mp3
void main()
{
	unsigned int Samples;

	//u nas zastapiony przez zmienna z nazwa pliku, prawdopodobnie "ts"
	FILE * MP3file = fopen("plik.mp3","rb");

	//tymczasowy plik wyjsciowy, jezeli dekodujemy calosc przed odczytem
	//jezeli dekodujemy i odtwarzamy na biezaco mozna usunac
	FILE * PCMfile = fopen("plik.pcm", "wb");

	//inicjalizacja dekodera
	SpiritMP3DecoderInit(&MP3Decoder, RetrieveMP3Data, NULL, MP3file);

	//petla dekodujaca
	do{
		//dekodowanie do bufora PCMSamples ramki o podanej wielkosci
		Samples = SpiritMP3Decode(&MP3Decoder, PCMSamples, frame_size_in_samples, MP3Info);

		//pobranie informacji o pliku, tylko przy pierwszej ramce
		if (first_frame == 0)
		{
			FrequencyHz = MP3Info.nSampleRateHz;
			BitrateKbps = MP3Info.nBitrateKbps;
			Channels = MP3Info.nChannels;
			first_frame = 1;

			//Tutaj powinno nastapic przeslanie parametrow przed odczytaniem danych
		}
		//zapis ramki do pliku jesli dekodujemy calosc przed odczytem z drugiego pliku tymczasowego
		//jezeli robimy to na biezaco to usunac ta linijke
		//i zastapic funkcja odtwarzajaca zdekodowany bufor PCMSamples
		fwrite(PCMSamples, 2*sizeof(short), Samples, PCMfile);
	}
	//powtarzamy dopoki dekodowanie zwraca true, a wiec byly dalej dane do dekodowania
	while(Samples);

	//po przetworzeniu calego pliku, resetujemy status pierwszej ramki
	first_frame = 0;

	fclose(PCMfile);
	fclose(MP3file);

	//jezeli dekodujemy calosc przed odczytem, tutaj powinna nastapic zamiana wartosci "ts"
	//na nowy plik .pcm tak aby przy nastepnym przejsciu petli nie dekodowal jeszcze raz,
	//a odpalil odtwarzanie nowego pliku

	//ewentualnie w ramach testu ile bedzie trwalo dekodowanie calosci, tutaj ustawic ts=""
	//i kod odpalajacy diode sygnalizujaca koniec dekodowania
}
