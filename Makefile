CC = clang 
CFLAGS = -Wall -Wpedantic -Wextra -Werror $(shell pkg-config --cflags gmp)
LFLAGS  = $(shell pkg-config --libs gmp)
OBJECTS = randstate.o numtheory.o rsa.o  

all: decrypt encrypt keygen 

decrypt: $(OBJECTS) decrypt.o
		$(CC) $(LFLAGS) -o decrypt $(OBJECTS) decrypt.o

encrypt: $(OBJECTS) encrypt.o
		$(CC) $(LFLAGS) -o encrypt $(OBJECTS) encrypt.o

keygen: $(OBJECTS) keygen.o
		$(CC) $(LFLAGS) -o keygen $(OBJECTS) keygen.o

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf decrypt encrypt keygen $(OBJECTS) decrypt.o encrypt.o keygen.o

format:
		clang-format -i -style=file *.c *.h

