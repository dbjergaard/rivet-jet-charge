# Compile macros using ROOT
CC=g++

RIVET_PREFIX=/home/dave/rivet/local
export PATH:=$(RIVET_PREFIX)/bin:$(PATH)
export PATH:=$(RIVET_PREFIX)/bin:$(PATH)
export LD_LIBRARY_PATH:=$(RIVET_PREFIX)/lib:$(LD_LIBRARY_PATH)

INCDIR=$(PWD)/include
WFLAGS=-Wall -Wextra
CFLAGS=-m32 -pg -ggdb -I$(RIVET_PREFIX)/include -I$(INCDIR) -O0 $(WFLAGS)
LDFLAGS=-m32 -pg -L$(RIVET_PREFIX)/lib 

all: runPythia

runPythia: runPythia.o 
	$(CC) runPythia.o -o runPythia $(LDFLAGS) 

runPythia.o: ./runPythia.C 
	$(CC) $(CFLAGS) -c ./runPythia.C 
clean:
	rm -rf *o  runPythia
