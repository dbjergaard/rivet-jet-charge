CC=g++

INCDIR=$(PWD)/include
LIBDIR:=$(shell rivet-config --libdir)
PREFIX:=$(shell rivet-config --prefix)
RIVETINCDIR:=$(shell rivet-config --includedir)
LDFLAGS:=$(shell rivet-config --ldflags)
WFLAGS= -Wall -Wextra
CFLAGS=-m64 -pg -I$(INCDIR) -I$(RIVETINCDIR) -O2 $(WFLAGS) -pedantic -ansi
all: rivet-lib
rivet-lib: libBOOSTFastJets.so
	$(CC) -shared -fPIC $(CFLAGS) -o "RivetMC_GENSTUDY_JETCHARGE.so" MC_GENSTUDY_JETCHARGE.cc -lBOOSTFastJets -L ./ $(LDFLAGS)
libBOOSTFastJets.so:
	$(CC) -shared -fPIC $(CFLAGS) src/BOOSTFastJets.cxx -o libBOOSTFastJets.so -lfastjet -lfastjettools $(LDFLAGS)
install:
	cp libBOOSTFastJets.so $(LIBDIR)
#	cp RivetMC_GENSTUDY_JETCHARGE.so $(LIBDIR) 
#	cp MC_GENSTUDY_JETCHARGE.plot $(PREFIX)/share
#	cp MC_GENSTUDY_JETCHARGE.info $(PREFIX)/share
clean:
	rm -f *.o  *.so
