SHELL = /bin/bash

CC = gcc
CC_OPTS = -std=gnu99

all: demon cli

demon: main.c siec.c obsluga_pakietu.c obsluga_cli.c baza_zainteresowan.c
	$(CC) $(CC_OPTS) -o demon main.c siec.c obsluga_pakietu.c obsluga_cli.c baza_zainteresowan.c
	
cli: cli.c
	$(CC) $(CC_OPTS) -o cli cli.c
