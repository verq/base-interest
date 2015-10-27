#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "baza_zainteresowan.h"
#include "obsluga_pakietu.h"

#define MAXBUFSIZE 65535 

void odbieranie(int socket, int dlugosc_gniazda, int czy_wypisywac) {

	struct sockaddr_in adres_wysylajacego;
	adres_wysylajacego.sin_addr.s_addr =  INADDR_ANY;
	int potrzebny_rozmiar = recvfrom(socket, NULL, 0, MSG_PEEK | MSG_TRUNC, NULL, NULL);
	if (potrzebny_rozmiar >= MAXBUFSIZE) perror("Za duzy rozmiar pakietu"), exit(0);
	struct pakiet* temp_pakiet = malloc(sizeof (struct pakiet));
	temp_pakiet -> dane = malloc(potrzebny_rozmiar * sizeof (char));
	temp_pakiet -> rozmiar = potrzebny_rozmiar;

	int status = recvfrom(socket, temp_pakiet -> dane, potrzebny_rozmiar, 0, (struct sockaddr*) &adres_wysylajacego, &dlugosc_gniazda);
	if (status < 0) perror("Blad odbierania"), exit(0);
	struct zainteresowania *temp_interesy = pakiet_do_zainteresowania(temp_pakiet);
	if (temp_interesy == NULL) perror("Odebrano niepoprawny pakiet"), exit(0);
	wstaw_komputer(ntohl(adres_wysylajacego.sin_addr.s_addr), time(NULL), temp_interesy, czy_wypisywac);
	sprawdz_liczniki(czy_wypisywac); //aktualizuje liczniki i usuwa zbyt stare komputery z listy
	wypisz_liste();

}

uint32_t wysylanie(int socket, struct pakiet* p, struct sockaddr_in adres_zrodla, int dlugosc_gniazda, char* adres_celu, int czy_wypisywac) {
	adres_zrodla.sin_addr.s_addr = inet_addr(adres_celu); 
	int status = sendto(socket, p -> dane, p -> rozmiar, 0, (const struct sockaddr*) &adres_zrodla, dlugosc_gniazda);
	if (status < 0) perror("Blad wysylania"), exit(0);
	sprawdz_liczniki(czy_wypisywac);
	return time(NULL);
}
