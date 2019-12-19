CC = g++
LD = g++
CCFLAGS = -Wall -pedantic
LDFLAGS =

main.out: main.o lin-2-list-barrier.o -lrt
	$(LD) $(LDFLAGS) -o main.out main.o lin-2-list-barrier.o -pthread  -lrt
main.o: main.cpp lin-2-list-barrier.h lin-2-list-barrier.c
	$(CC) $(CCFLAGS) -c main.cpp -pthread -lrt
lin-2-list-barrier.o: lin-2-list-barrier.c lin-2-list-barrier.h
	$(CC) $(CCFLAGS) -c lin-2-list-barrier.c
clean:
	rm *.o
    
