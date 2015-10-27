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
#include "obsluga_cli.h"


char* adres_celu = "224.0.0.213";
uint16_t port_celu = 6969;
int wypisuj = 0;


int max(int a, int b);

int main(int argc, char *argv[]) {
	if (argv[1] == NULL) perror("Brak pliku"), exit(0);
	wlasne_zaint = plik_do_zainteresowania(argv[1]);
	struct pakiet* p1 = zainteresowania_do_pakiet(wlasne_zaint);
	if (argv[2] != NULL && strcmp(argv[2], "-v") == 0) wypisuj = 1;

	int socket_udp, status;
	
	struct sockaddr_in adres_zrodla;
	struct ip_mreqn opcje_multicastu;
	unlink("pogadanki_z_cli");
	struct sockaddr_un adres_serwera = {AF_UNIX, "pogadanki_z_cli"};
	
	int socket_unix = 0;
	int socket_unix_sluchanie = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if(socket_unix_sluchanie < 0) perror("Blad tworzenia soketa unixowego"), exit(0);
	status = bind(socket_unix_sluchanie, (const struct sockaddr*) &adres_serwera, 
						sizeof(adres_serwera.sun_family) + sizeof(adres_serwera.sun_path)); 
	if(status < 0)	perror("Blad binda unix"), exit(0);
	if(listen(socket_unix_sluchanie, 5) < 0) perror("Blad sluchania"), exit(0);
	
	memset(&adres_zrodla, 0, sizeof(struct sockaddr_in));
	memset(&opcje_multicastu, 0, sizeof(struct ip_mreqn));
	adres_zrodla.sin_family = AF_INET;
        adres_zrodla.sin_port = htons(port_celu); 

        int dlugosc_gniazda = sizeof(struct sockaddr_in);
        
        opcje_multicastu.imr_multiaddr.s_addr = inet_addr(adres_celu);
        opcje_multicastu.imr_address.s_addr   = INADDR_ANY;
 	opcje_multicastu.imr_ifindex = 0;
	
	socket_udp = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_udp < 0) perror("Blad tworzenia socketa"), exit(0);
	
        status = bind(socket_udp, (struct sockaddr*) &adres_zrodla, dlugosc_gniazda);
	if (status < 0) perror("Blad binda"), exit(0);
	
	status = setsockopt(socket_udp, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&opcje_multicastu, sizeof(struct ip_mreq));
	if (status < 0) perror("Blad ustawiania opcji mulitcastowych"), exit(0);
	
	inicjalizuj(); //inicjalizuje liste do ktorej zapisywane dane o zainteresowaniach
	uint32_t czas_ost_wyslania = wysylanie(socket_udp, p1, adres_zrodla, dlugosc_gniazda, adres_celu, wypisuj);
	while( 1 ) {	
		struct timeval czasomierz;
		czasomierz.tv_sec = 30; czasomierz.tv_usec = 0;
		fd_set read_fd_set;	
		FD_ZERO(&read_fd_set);
		int socket_max = socket_udp;
		FD_SET(socket_max, &read_fd_set);
		if (socket_unix != 0) { socket_max = max(socket_udp, socket_unix); FD_SET(socket_unix, &read_fd_set);}
		else { socket_max = max(socket_udp, socket_unix_sluchanie); FD_SET(socket_unix_sluchanie, &read_fd_set);}

		if (difftime(time(NULL), czas_ost_wyslania) > lista_komputerow.hello_interval/6) {
			czas_ost_wyslania = wysylanie(socket_udp, p1, adres_zrodla, dlugosc_gniazda, adres_celu, wypisuj);
			if (wypisuj == 1) printf("Wyslano pakiet\n");
		}

		int retval = select(socket_max+1, &read_fd_set, NULL, NULL, &czasomierz);
		if (retval == 0) {
			czas_ost_wyslania = wysylanie(socket_udp, p1,  adres_zrodla, dlugosc_gniazda, adres_celu, wypisuj);
			if (wypisuj == 1) printf("Wyslano pakiet\n");
		} else {
			if( FD_ISSET(socket_udp, &read_fd_set)) {
				odbieranie(socket_udp, dlugosc_gniazda, wypisuj);
				if (wypisuj == 1) printf("Odebrano pakiet\n");
			}
			if(socket_unix == 0 && FD_ISSET(socket_unix_sluchanie, &read_fd_set)) {
				socket_unix = accept(socket_unix_sluchanie, NULL, 0);
				if (socket_unix < 0) socket_unix = 0;
			}
			if(socket_unix != 0 && FD_ISSET(socket_unix, &read_fd_set)) { 
				obsluz_cli(socket_unix);	
				socket_unix = 0;
			}
		}
	}
	close(socket_udp);
	close(socket_unix);
	close(socket_unix_sluchanie);
}

int max(int a, int b) {
	if (a < b) return b;
	else return a;
}








