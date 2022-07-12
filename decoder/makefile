CC=gcc
IDIR=./inc
CFLAGS=-I$(IDIR) -Wall
LDFLAGS=-m32



DEPS= ./inc/align.h  ./inc/api.h  ./inc/brg_endian.h  ./inc/crypto_aead.h  ./inc/Cyclist.h  ./inc/Xoodoo.h  ./inc/Xoodoo-SnP.h  ./inc/Xoodyak.h ./inc/types.h
OBJ= ./src/main.o ./src/encrypt.o ./src/Xoodyak.o ./src/Xoodoo-reference.o

%.o: %.c $(DEPS)
	$(CC) $(LDFLAGS)  -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) $(LDFLAGS)  -o main.out $^ $(CFLAGS)
	rm $(OBJ)	

