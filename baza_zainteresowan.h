#ifndef BAZA_ZAINTERESOWAN_H
#include <time.h>

struct wezel {
	uint32_t wlasciciel;
	uint32_t data_ostatnich_wiesci;
	struct zainteresowania* zainteresowania_wezel;
	struct wezel* nast; 
};

struct lista {
	double hello_interval;
	int ilosc_komputerow;
	struct wezel* poczatek; 
};

struct wezel* inicjalizuj();
struct wezel* wstaw_komputer(uint32_t wlas, uint32_t ostatnie_wiesci, struct zainteresowania* inter, int czy_wypisywac); //rowniez aktualizuje!
int sprawdz_czy_na_liscie(uint32_t wlas);
double ustaw_hello_interval(double hello);
struct wezel* aktualizuj(uint32_t wlas, uint32_t ostatnie_wiesci, struct zainteresowania* inter);
struct wezel* usun_komputer(struct wezel* usuwany);
struct wezel* sprawdz_liczniki(int czy_wypisywac);
void wypisz_liste();

struct lista lista_komputerow;
#endif
