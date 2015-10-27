#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdint.h>
#include <arpa/inet.h>
 
void wylistuj(int socket_serwer);
void interesy(int socket_serwer, char* zainteresowanie);
void dodaj_zainteresowanie(int socket_serwer, char* zainteresowanie);

int main(int argc, char** argv) {
	struct sockaddr_un adres_serwera = {AF_UNIX, "pogadanki_z_cli"};
	int socket_serwer;
	socket_serwer = socket(PF_UNIX, SOCK_STREAM, 0);
	connect(socket_serwer, (const struct sockaddr*) &adres_serwera, sizeof(adres_serwera.sun_family) + sizeof(adres_serwera.sun_path));
	if (argv[1] == NULL) {
		wylistuj(socket_serwer);	
	} else if (strcmp(argv[1], "-i") == 0) {
		if (argv[2] == NULL) perror("Brak zainteresowania"), exit(0);
		interesy(socket_serwer, argv[2]);	
	} else if (strcmp(argv[1], "-a") == 0) {
		if (argv[2] == NULL) perror("Brak zainteresowania"), exit(0);
		dodaj_zainteresowanie(socket_serwer, argv[2]);
	}
	shutdown(socket_serwer, SHUT_RDWR);
	return 0;
}

void wylistuj(int socket_serwer){
	int dlugosc = strlen("wylistuj") + 1;
	send(socket_serwer, &dlugosc, 4, 0);
	send(socket_serwer, "wylistuj", dlugosc, 0);
	
	recv(socket_serwer, &dlugosc, 4, 0);
	char* wiadomosc = malloc(dlugosc);
	recv(socket_serwer, wiadomosc, dlugosc, 0);
	
	int wskaznik = 0;
	int liczba_komputerow = ((int*)wiadomosc)[wskaznik];
	wskaznik = wskaznik + 4;
	for(int i = 0; i < liczba_komputerow; i++) {
		int adres = *((int*)(wiadomosc + wskaznik));
		wskaznik = wskaznik + 4;
		int sec  = *((int*)(wiadomosc + wskaznik));
		wskaznik = wskaznik + 4;
		
		struct in_addr wlasc;
		wlasc.s_addr = htonl(adres);
		printf("Adres: %s, czas: %d\n", inet_ntoa(wlasc), sec);
		int num_inters = *((int*)(wiadomosc + wskaznik));
		wskaznik = wskaznik + 4;
		for(int j = 0; j < num_inters; j++) {
			printf("\t%s\n", (char*)(wiadomosc + wskaznik));
			wskaznik = wskaznik + strlen(wiadomosc + wskaznik) + 1;
		}
	}
	free(wiadomosc);
}

void interesy(int socket_serwer, char* zainteresowanie) {
	int dlug_inter = strlen("interesy") + 1;
	send(socket_serwer, &dlug_inter, 4, 0);
	send(socket_serwer, "interesy", dlug_inter, 0);
	int dlug_zaint = strlen(zainteresowanie) + 1;
	send(socket_serwer, &dlug_zaint, 4, 0);
	send(socket_serwer, zainteresowanie, dlug_zaint, 0);
	int dlugosc;
	recv(socket_serwer, &dlugosc, 4, 0);
	char* wiadomosc = malloc(dlugosc);
	recv(socket_serwer, wiadomosc, dlugosc, 0);
	int wskaznik = 0;
	int liczba_komputerow = 0;
	liczba_komputerow = *((int*)wiadomosc);
	wskaznik = wskaznik + 4;
	for(int i = 0; i < liczba_komputerow; i++) {
		int adres = *((int*)(wiadomosc + wskaznik));
		wskaznik = wskaznik + 4;
		struct in_addr wlasc;
		wlasc.s_addr = htonl(adres);
		printf("Adres komputera %d:  %s \n",i, inet_ntoa(wlasc));

	}
	free(wiadomosc);
}

void dodaj_zainteresowanie(int socket_serwer, char* zainteresowanie) {
	int dlug_inter = strlen("dodaj_in") + 1;
	send(socket_serwer, &dlug_inter, 4, 0);
	send(socket_serwer, "dodaj_in", dlug_inter, 0);
	int dlug_zaint = strlen(zainteresowanie) + 1;
	send(socket_serwer, &dlug_zaint, 4, 0);
	send(socket_serwer, zainteresowanie, dlug_zaint, 0);
	int dlugosc;
	recv(socket_serwer, &dlugosc, 4, 0);
	char* wiadomosc = malloc(dlugosc);
	recv(socket_serwer, wiadomosc, dlugosc, 0);
	int wskaznik = 0;
	int liczba_zainteresowan = 0;
	liczba_zainteresowan = *((int*)wiadomosc);
	wskaznik = wskaznik + 4;
	printf("Liczba zainteresowan to  %d, zainteresowania: \n", htonl(liczba_zainteresowan));
	for (int i = 0; i < htonl(liczba_zainteresowan); i++) {
		printf("\t%s \n", (char*)wiadomosc + wskaznik);
		wskaznik = wskaznik + strlen(wiadomosc + wskaznik) + 1;
	}
	free(wiadomosc);
}





