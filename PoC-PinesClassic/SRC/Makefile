CC=gcc      ## Compilador a utilizar
CFLAGS= -g -Wall -pedantic   #FLAGS con depuración, Warning activados y sintaxis ANSI.

# Flags para indicar la localizacion de los includes y librerias
#IFLAGS=-I./
#LDFLAGS=-L./ -L/usr/lib/openssl-1.1
# Librerias a enlazar-lm enlaza la biblioteca matematica, -pthread enlaza la biblioteca de hilos

LIBS = -lcrypto -lnfc

# Objetivo all ejecutara lo indicado en la variable EJS y despues limpiara.
all:
	# rm Clav_NFC.elf
	gcc -o Clav_NFC.elf Clav_NFC.c ../../Auxiliares/mifare.c $(CFLAGS) $(LIBS)
   