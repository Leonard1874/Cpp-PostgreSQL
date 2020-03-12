CC=g++
CFLAGS=-O3 -ggdb3
EXTRAFLAGS=-lpqxx -lpq

all: test

test: main.cpp exerciser.hpp exerciser.cpp query_funcs.hpp query_funcs.cpp
	$(CC) $(CFLAGS) -o test main.cpp exerciser.cpp query_funcs.cpp $(EXTRAFLAGS)

clean:
	rm -f *~ *.o test

clobber:
	rm -f *~ *.o
