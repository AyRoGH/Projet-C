CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c90 #-Wpedantic
CFILES=main.c console.c souterrain.c
OFILES=$(CFILES:.c=.o)
EXEC=main

all: $(EXEC)

$(EXEC): $(OFILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(OFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OFILES) $(EXEC)