#ifndef OBSLUGA_PAKIETU_H
#define OBSLUGA_PAKIETU_H

#include <stdint.h>


struct pakiet { 
	int rozmiar;
	int poprawny;	//pole uzyte tylko w funkcji pakiet_do_zainteresowania z pliku obsluga_pakietu.c
	void* dane;			//trzymam kolejnosc bajtow w sieciowej kolejnosci
};

struct zainteresowania { 
	char** interesy;
	int liczba_zainteresowan; //kolejnosc bajtow w zwyklej kolejnosci
};

struct zainteresowania* wlasne_zaint;

struct pakiet* wszystkie_do_pakietu();
struct pakiet* konkretne_zainteresowania_do_pakietu(char* zainteresowanie);
struct pakiet* dodaj_zainteresowanie(char* zainteresowanie);
struct zainteresowania* pakiet_do_zainteresowania(struct pakiet* wejscie);
struct pakiet* zainteresowania_do_pakiet(struct zainteresowania* wejscie);
struct zainteresowania* stringi_do_zainteresowania(char** wejscie);
struct zainteresowania* plik_do_zainteresowania(char* plik);
void wypisz_zainteresowania(struct zainteresowania* wejscie);
void odalokuj_zainteresowania(struct zainteresowania* usuwany);
#endif
