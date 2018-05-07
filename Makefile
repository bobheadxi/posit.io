.PHONY: deps

CC=gcc
CFLAGS=-lstdc++ -O 

all: deps

deps:
	git submodule update --init --recursive

clean:
	rm -f *.o

posit.o: posit.cpp
	$(CC) $(CFLAGS) -c posit.cpp

#####################
# PROOF-OF-CONCEPTS #
#####################

poc: main.o posit.o
	$(CC) $(CFLAGS) -o poc.o main.o posit.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
