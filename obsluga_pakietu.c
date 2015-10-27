#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "obsluga_pakietu.h"
#include "baza_zainteresowan.h"

#define MAX_DLUG_ZAINT 128

struct pakiet* wszystkie_do_pakietu() {
	struct pakiet* wynik;
	wynik = malloc(sizeof (struct pakiet));
	wynik -> rozmiar = 4;

	if (lista_komputerow.poczatek == NULL) {
		printf(":(\n");
		wynik -> dane = NULL;
	}
	struct wezel* temp = lista_komputerow.poczatek;
	int liczba_spakowanych = 0;

	while (temp != NULL) {
		liczba_spakowanych++;
		wynik -> rozmiar = wynik -> rozmiar + 12; //na wlasciciela, czas i zainteresowania
		for (int i = 0; i < temp -> zainteresowania_wezel ->  liczba_zainteresowan; i++) 
			wynik -> rozmiar =  wynik -> rozmiar + strlen(temp -> zainteresowania_wezel ->interesy[i]) + 1;
		temp = temp -> nast;
	}
	wynik -> dane = malloc(wynik -> rozmiar);
	temp = lista_komputerow.poczatek;
	void* dane_pakietu = wynik -> dane;
	*((int*)dane_pakietu) = lista_komputerow.ilosc_komputerow;
	dane_pakietu = dane_pakietu + 4;
	for (int i = 0; i < lista_komputerow.ilosc_komputerow; i++) {
		*((int*)dane_pakietu) = temp -> wlasciciel;
		dane_pakietu = dane_pakietu + 4;

		*((int*)dane_pakietu) = difftime(time(NULL), temp -> data_ostatnich_wiesci);
		dane_pakietu= dane_pakietu + 4;
		
		*((int*)dane_pakietu) = temp -> zainteresowania_wezel -> liczba_zainteresowan;
		dane_pakietu = dane_pakietu + 4;
		for (int i = 0; i < temp -> zainteresowania_wezel -> liczba_zainteresowan; i++) {
			strcpy((char*)dane_pakietu, temp -> zainteresowania_wezel -> interesy[i]);
			dane_pakietu = dane_pakietu + strlen( temp -> zainteresowania_wezel -> interesy[i]) + 1;
		
		} 
		temp = temp -> nast;
	
	}
	return wynik;
}

struct pakiet* konkretne_zainteresowania_do_pakietu(char* zainteresowanie) {
	struct pakiet* wynik = malloc(sizeof(struct pakiet));
	struct wezel* temp = lista_komputerow.poczatek;
	wynik -> rozmiar = 0;
	int liczba_zainteresowanych = 0;
	while (temp != NULL) {
		for (int i = 0; i < temp -> zainteresowania_wezel -> liczba_zainteresowan; i++) {
			if (strcmp(zainteresowanie, temp -> zainteresowania_wezel -> interesy[i]) == 0) {
				wynik -> rozmiar = wynik -> rozmiar + 4;
				liczba_zainteresowanych++;
				break;
			}
		}
		 temp = temp -> nast;
	}
	wynik -> rozmiar = wynik -> rozmiar + 4; // na liczbe zainteresowanych
	wynik -> dane = malloc(wynik -> rozmiar);
	temp =  lista_komputerow.poczatek;
	void* dane_pakietu = wynik -> dane;
	*((int*)dane_pakietu) = liczba_zainteresowanych;
	dane_pakietu = dane_pakietu + 4;
	while (temp != NULL) {
		for (int i = 0; i < temp -> zainteresowania_wezel -> liczba_zainteresowan; i++) {
			if (strcmp(zainteresowanie, temp -> zainteresowania_wezel -> interesy[i]) == 0) {
				*((int*)dane_pakietu) = temp -> wlasciciel;
				dane_pakietu = dane_pakietu + 4;
				break;
			}
		}
		 temp = temp -> nast;
	}
	return wynik;
}

struct pakiet* dodaj_zainteresowanie(char* zainteresowanie) {
	struct zainteresowania* nowe = malloc(sizeof(struct zainteresowania));
	nowe -> interesy = malloc((wlasne_zaint -> liczba_zainteresowan + 1) * sizeof(char*));
	nowe -> liczba_zainteresowan = wlasne_zaint -> liczba_zainteresowan + 1;
	for (int i = 0; i < nowe -> liczba_zainteresowan - 1; i++) {
		nowe -> interesy[i] = strdup(wlasne_zaint -> interesy[i]);
	}
	nowe -> interesy[nowe -> liczba_zainteresowan - 1] = strdup(zainteresowanie);

	struct pakiet* wynik = zainteresowania_do_pakiet(nowe);
	wlasne_zaint -> liczba_zainteresowan = nowe -> liczba_zainteresowan;
	for (int i = 0; i < wlasne_zaint -> liczba_zainteresowan - 1; i++) {
		free(wlasne_zaint -> interesy[i]);
	}
	free(wlasne_zaint -> interesy);
	wlasne_zaint -> interesy = malloc(wlasne_zaint -> liczba_zainteresowan * sizeof(char*));
	for (int i = 0; i < nowe -> liczba_zainteresowan; i++) {
		wlasne_zaint -> interesy[i] = strdup(nowe -> interesy[i]);
	}
	odalokuj_zainteresowania(nowe);
	return wynik;
}


struct zainteresowania* pakiet_do_zainteresowania(struct pakiet* wejscie) { 
	wejscie -> poprawny = 1;
	struct zainteresowania* wynik = malloc(sizeof(struct zainteresowania));
	if (wejscie -> rozmiar < 4) wejscie -> poprawny = -1;
	wynik -> liczba_zainteresowan = ntohl( ( (int*)wejscie -> dane)[0]);
	
	int wskaznik = 4;
	for(int i = 0; i < wynik -> liczba_zainteresowan; i++) {
		while(wskaznik < wejscie -> rozmiar && ((char*)wejscie -> dane)[wskaznik] != '\0') wskaznik++;
		if(wskaznik == (wejscie -> rozmiar) -1 && ((char*)wejscie -> dane)[wskaznik] != '\0') wejscie -> poprawny = -1;
		if(wskaznik < wejscie -> rozmiar) wskaznik++;
	}
	if (wejscie -> poprawny == -1) { 
		printf("Pakiet niepoprawny\n");
		return NULL;
	}
	wynik -> interesy = malloc(wynik -> liczba_zainteresowan * sizeof (char*));
	char* poczatek_nast_interesu = (char*)wejscie -> dane + 4;
	 
	for (int i = 0; i < wynik -> liczba_zainteresowan; i++) {
		wynik -> interesy[i] = strdup(poczatek_nast_interesu);
		poczatek_nast_interesu = poczatek_nast_interesu + strlen(poczatek_nast_interesu) + 1;
	}
	return wynik;
}

struct pakiet* zainteresowania_do_pakiet(struct zainteresowania* wejscie) {
	struct pakiet* wynik = malloc(sizeof(struct pakiet));
	wynik -> rozmiar = 4; //liczba na poczatku ma 4 bajty
	for (int i = 0; i < wejscie -> liczba_zainteresowan ; i++) {
		wynik -> rozmiar = wynik -> rozmiar + strlen(wejscie->interesy[i]) + 1;
	}
	wynik -> dane = malloc(wynik -> rozmiar * sizeof (char));
	((int*)wynik -> dane)[0] = htonl(wejscie -> liczba_zainteresowan);
	char* koniec_obecnych_danych = (char*)wynik-> dane + 4; 
	for (int i = 0; i < wejscie -> liczba_zainteresowan; i++) {
		strcpy(koniec_obecnych_danych, wejscie -> interesy[i]); 
		koniec_obecnych_danych = koniec_obecnych_danych + strlen(wejscie -> interesy[i]) + 1;
	}	
	return wynik;
}

struct zainteresowania* stringi_do_zainteresowania(char** wejscie) {
	struct zainteresowania* wynik = malloc(sizeof (struct zainteresowania));
	wynik -> liczba_zainteresowan = atoi(wejscie[0]);
	wynik -> interesy = malloc(wynik -> liczba_zainteresowan * sizeof(char*));
	for (int i = 0; i < wynik -> liczba_zainteresowan; i++) {
		wynik -> interesy[i] = strdup(wejscie[i+1]);
	}
	return wynik;
}

struct zainteresowania* plik_do_zainteresowania(char* plik) {
	FILE *fp;
   	fp = fopen(plik, "r");
   	struct zainteresowania* wynik = malloc(sizeof (struct zainteresowania));
	fscanf(fp, "%d ", &wynik->liczba_zainteresowan);
	wynik -> interesy = malloc(wynik -> liczba_zainteresowan * sizeof(char*));
	for (int i = 0; i < wynik -> liczba_zainteresowan; i++) {
		char* temp = malloc (MAX_DLUG_ZAINT * sizeof (char)); 
		if (temp = fgets(temp, MAX_DLUG_ZAINT, fp)) {
			wynik -> interesy[i] = temp; 
			for (int k = 0; k < MAX_DLUG_ZAINT; k++) {
				if (wynik -> interesy[i][k] == '\n') { 
					wynik -> interesy[i][k] = '\0'; 	
					break;
				}
			}
		}
		else printf("Zainteresowania za dlugie");
	}
	return wynik;

}

void odalokuj_zainteresowania(struct zainteresowania* usuwany) {
	for (int i = 0; i < usuwany -> liczba_zainteresowan; i++) {
		free(usuwany -> interesy[i]);
	}
	free(usuwany -> interesy);
	free(usuwany);
}

void wypisz_zainteresowania(struct zainteresowania* wejscie) {
	printf("%d\n", wejscie -> liczba_zainteresowan);
	for (int i = 0; i < wejscie -> liczba_zainteresowan; i++) {
		printf("\t%s\n", wejscie -> interesy[i]); 
	}
}

