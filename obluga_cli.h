#ifndef OBSLUGA_CLI_H
#define OBSLUGA_CLI_H


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

void obsluz_cli(int cli_sock);

#endif
