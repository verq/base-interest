#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "baza_zainteresowan.h"
#include "obsluga_pakietu.h"

#define HELLO_INTERVAL 180.00

struct lista lista_komputerow;

struct wezel* inicjalizuj() {
	lista_komputerow.ilosc_komputerow = 0;
	lista_komputerow.hello_interval = HELLO_INTERVAL;	
	lista_komputerow.poczatek = NULL;
	return lista_komputerow.poczatek;
}

struct wezel* wstaw_komputer(uint32_t wlas, uint32_t ostatnie_wiesci, struct zainteresowania* inter, int czy_wypisywac) { //rowniez aktualizuje!
	if (lista_komputerow.ilosc_komputerow == 0) {
		lista_komputerow.poczatek = malloc(sizeof(struct wezel));
		lista_komputerow.poczatek -> wlasciciel = wlas;
		lista_komputerow.poczatek -> data_ostatnich_wiesci = ostatnie_wiesci;
		lista_komputerow.poczatek -> zainteresowania_wezel = inter;
		lista_komputerow.ilosc_komputerow++;
		if (czy_wypisywac == 1) printf("Dodano wpis do bazy \n");
	} else if (sprawdz_czy_na_liscie(wlas)) {
		aktualizuj(wlas, ostatnie_wiesci, inter);
		if (czy_wypisywac == 1) printf("Zaktualizowano wpis");
	}
	else {
		struct wezel* temp = lista_komputerow.poczatek; 
		while (temp -> nast != NULL) {
			temp = temp -> nast;
		}
		temp -> nast = malloc(sizeof (struct wezel));
		temp -> nast -> wlasciciel = wlas;
		temp -> nast -> data_ostatnich_wiesci = ostatnie_wiesci;
		temp -> nast -> zainteresowania_wezel = inter; 
		temp -> nast -> nast = NULL;
		lista_komputerow.ilosc_komputerow++;
	}
	return lista_komputerow.poczatek;
}

int sprawdz_czy_na_liscie(uint32_t wlas) {
	struct wezel* temp = lista_komputerow.poczatek;
	while (temp != NULL) {
		if (temp -> wlasciciel == wlas) return 1;
		temp = temp -> nast;
	}
	return 0;
}

struct wezel* aktualizuj(uint32_t wlas, uint32_t ostatnie_wiesci, struct zainteresowania* inter) {
	struct wezel* temp = lista_komputerow.poczatek;
	while (temp -> wlasciciel != wlas) { //wiadomo ze jest, bo przed wywolaniem to sprawdzamy
		if (temp == NULL) printf("Blad aktualizacji\n");
		else temp =  temp -> nast;
	}
	temp -> data_ostatnich_wiesci = ostatnie_wiesci;
	odalokuj_zainteresowania(temp -> zainteresowania_wezel);
	temp -> zainteresowania_wezel = inter; 
	return lista_komputerow.poczatek;	
}

struct wezel* sprawdz_liczniki(int czy_wypisywac) {
	time_t biezaca_data;
	biezaca_data = time(NULL);
	struct wezel* temp = lista_komputerow.poczatek;
	while (temp != NULL) {
		if (difftime(biezaca_data, temp -> data_ostatnich_wiesci) > lista_komputerow.hello_interval) {
			lista_komputerow.poczatek = usun_komputer(temp);
			if (czy_wypisywac == 1) printf("Usunieto wpis z bazy");
		}
		temp = temp -> nast;
	}
	return lista_komputerow.poczatek;
}

struct wezel* usun_komputer(struct wezel* usuwany) {
	if (usuwany == lista_komputerow.poczatek) {
		lista_komputerow.poczatek = lista_komputerow.poczatek -> nast;
		odalokuj_zainteresowania(usuwany -> zainteresowania_wezel); 
		free(usuwany);
	} else {
		struct wezel* temp = lista_komputerow.poczatek;
		while (temp -> nast != usuwany) temp = temp -> nast;
		temp -> nast = temp -> nast -> nast;
		odalokuj_zainteresowania(usuwany -> zainteresowania_wezel); 
		free(usuwany);
	}
	lista_komputerow.ilosc_komputerow--;
	wypisz_liste();
	return lista_komputerow.poczatek;
}

double ustaw_hello_interval(double hello) {
	if (hello != 0) lista_komputerow.hello_interval = hello;
	else lista_komputerow.hello_interval = HELLO_INTERVAL;
}

void wypisz_liste() {
	struct wezel* temp = lista_komputerow.poczatek;
	for (int i = 0; i < lista_komputerow.ilosc_komputerow; i++) {
		struct in_addr wlasc; 
		wlasc.s_addr = htonl(temp -> wlasciciel);
		int teraz = (int)(time(NULL));
		int wtedy = (int) temp -> data_ostatnich_wiesci;
		printf("%d wpis w bazie: wlasciciel - %s, czas - %d, zainteresowania: \n", i,inet_ntoa(wlasc), teraz - wtedy);
		wypisz_zainteresowania(temp -> zainteresowania_wezel);
		temp = temp -> nast;
	}
}
