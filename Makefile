CC=g++

INCDIR=$(PWD)/include
RIVETINCDIR=$(HOME)/rivet/local/include
WFLAGS= -Wall -Wextra
CFLAGS=-m64 -pg -I$(INCDIR) -I$(RIVETINCDIR) -O2 $(WFLAGS) -pedantic -ansi
all: rivet-lib
rivet-lib: libBOOSTFastJets.so
	$(CC) -shared -fPIC $(CFLAGS) -o "RivetMC_GENSTUDY_JETCHARGE.so" MC_GENSTUDY_JETCHARGE.cc -lBOOSTFastJets -L ./ 
libBOOSTFastJets.so:
	$(CC) -shared -fPIC $(CFLAGS) src/BOOSTFastJets.cxx -o libBOOSTFastJets.so -lfastjet -lfastjettools -L $(HOME)/rivet/local/lib
clean:
	rm -f *.o  *.so
