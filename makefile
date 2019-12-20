SRC = main.c list.c list.h
CC = gcc
OUT = run

KEYS = -lrt -lpthread

all: main

main:
	$(CC) $(SRC) $(KEYS) -o $(OUT)

clean:
	rm $(OUT)
