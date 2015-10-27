#ifndef SIEC_H
#define SIEC_H


void odbieranie(int socket, int dlugosc_gniazda, int czy_wypisywac);
uint32_t wysylanie(int socket, struct pakiet* p, struct sockaddr_in adres_zrodla, int dlugosc_gniazda, char* adres_celu, int czy_wypisywac);

#endif
