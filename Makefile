.PHONY: deps

CC=gcc
CFLAGS=-O -lstdc++

all: deps

deps:
	git submodule update --init --recursive

clean:
	rm -f *.o
	rm -f *.obj

posit.obj:
	$(CC) $(CFLAGS) -c posit.cpp -o posit.obj

#####################
# PROOF-OF-CONCEPTS #
#####################

poc: clean poc.obj posit.obj
	$(CC) $(CFLAGS) -o poc.o poc.obj posit.obj

poc.obj:
	$(CC) $(CFLAGS) -c poc.cpp -o poc.obj
