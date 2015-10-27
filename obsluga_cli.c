#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <stdint.h>
#include <sys/un.h>
#include <errno.h>

#include "baza_zainteresowan.h"
#include "obsluga_pakietu.h"
#include "siec.h"

void wylistuj_interesy(int socket);
void odbierz_interesy(int socket);
void dodaj_interesy(int socket);

void obsluz_cli(int socket) {
	struct pakiet* przychodzace;
	przychodzace = malloc(sizeof(struct pakiet));
	recv(socket, &(przychodzace -> rozmiar), 4, 0);
	przychodzace -> dane = malloc(sizeof(char) * przychodzace -> rozmiar);
	recv(socket, przychodzace -> dane, przychodzace -> rozmiar, 0);
	if(strcmp("wylistuj", przychodzace -> dane) == 0) {
		wylistuj_interesy(socket);
	} else if (strcmp("interesy", przychodzace -> dane) == 0) {
		odbierz_interesy(socket);
	} else if (strcmp("dodaj_in", przychodzace -> dane) == 0) {
		dodaj_interesy(socket);
	} 
	else {
		int len = strlen("Nieznana komenda") + 1;
		send(socket, &len, 4, 0);
		send(socket, "Nieznana komenda", len, 0);
	}
	free(przychodzace);
	free(przychodzace -> dane);
	close(socket);
}

void wylistuj_interesy(int socket) {
	struct pakiet* wychadzace = wszystkie_do_pakietu();
	send(socket, &wychadzace -> rozmiar, 4, 0);
	send(socket, wychadzace -> dane, wychadzace -> rozmiar, 0);
	free(wychadzace-> dane);
	free(wychadzace);

}

void odbierz_interesy(int socket) {
		struct pakiet* zainteresowanie;
		zainteresowanie = malloc(sizeof(struct pakiet));
		recv(socket, &(zainteresowanie -> rozmiar), 4, 0);
		zainteresowanie -> dane = malloc(sizeof(char) * zainteresowanie -> rozmiar);
		recv(socket, (zainteresowanie -> dane), zainteresowanie -> rozmiar, 0);
		struct pakiet* wychadzace = konkretne_zainteresowania_do_pakietu((char*)zainteresowanie -> dane);
		send(socket, &wychadzace -> rozmiar, 4, 0);
		send(socket, wychadzace -> dane, wychadzace -> rozmiar, 0);
		free(zainteresowanie -> dane);
		free(zainteresowanie);
		free(wychadzace-> dane);
		free(wychadzace);

}

void dodaj_interesy(int socket){
		struct pakiet* zainteresowanie;
		zainteresowanie = malloc(sizeof(struct pakiet));
		recv(socket, &(zainteresowanie -> rozmiar), 4, 0);
		zainteresowanie -> dane = malloc(sizeof(char) * zainteresowanie -> rozmiar);
		recv(socket, (zainteresowanie -> dane), zainteresowanie -> rozmiar, 0);
		struct pakiet* wychadzace = dodaj_zainteresowanie((char*)zainteresowanie -> dane);
		send(socket, &wychadzace -> rozmiar, 4, 0);
		send(socket, wychadzace -> dane, wychadzace -> rozmiar, 0);
		free(zainteresowanie -> dane);
		free(zainteresowanie);
		free(wychadzace-> dane);
		free(wychadzace);
}
