.PHONY: deps

CC=gcc
CFLAGS=-O
LINKFLAGS=-lstdc++

all: deps

deps:
	git submodule update --init --recursive vendor

clean:
	rm -f *.o
	rm -f *.obj

posit.obj:
	$(CC) $(CFLAGS) -c posit.cpp -o posit.obj

#####################
# PROOF-OF-CONCEPTS #
#####################

poc: clean poc.obj posit.obj
	$(CC) $(CFLAGS) $(LINKFLAGS) -o poc.o poc.obj posit.obj

poc.obj:
	$(CC) $(CFLAGS) -c poc.cpp -o poc.obj
