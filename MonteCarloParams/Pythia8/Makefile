# Compile macros using ROOT
CC=g++

RIVET_PREFIX=$(HOME)/rivet/local
export PATH:=$(RIVET_PREFIX)/bin:$(PATH)
export PATH:=$(RIVET_PREFIX)/bin:$(PATH)
export LD_LIBRARY_PATH:=$(RIVET_PREFIX)/lib:$(LD_LIBRARY_PATH)

INCDIR=$(PWD)/include
WFLAGS=-Wall -Wextra
CFLAGS=-m64 -pg -I$(RIVET_PREFIX)/include -I$(INCDIR) -O3 $(WFLAGS)
LDFLAGS=-m64 -pg -L$(RIVET_PREFIX)/lib 	-L$(RIVET_PREFIX)/lib/archive -lpythia8 -lLHAPDF \
	-lhepmcinterface -lHepMC


all: runPythia

runPythia: runPythia.o 
	$(CC) runPythia.o -o runPythia $(LDFLAGS) 

runPythia.o: ./runPythia.C 
	$(CC) $(CFLAGS) -c ./runPythia.C 
clean:
	rm -rf *o  runPythia
