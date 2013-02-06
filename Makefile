# Compile macros using ROOT
CC=g++


 ROOTSYS=${HOME}/root
 export PATH:=$(ROOTSYS)/bin:$(PATH)
 export PATH:=$(ROOTSYS)/bin:$(PATH)
 export LD_LIBRARY_PATH:=$(ROOTSYS)/lib:$(LD_LIBRARY_PATH)

INCDIR=$(PWD)/include
RIVETINCDIR=/home/dave/rivet/local/include
WFLAGS= -Wall -Wextra
CFLAGS=-m64 -std=c++11 -pg -I$(ROOTSYS)/include -I$(INCDIR) -I$(RIVETINCDIR) -O2 $(WFLAGS) -pedantic -ansi
LDFLAGS=-L$(ROOTSYS)/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lGui  -lm -ldl -rdynamic -lFoam 
ROOFITLDFLAGS=-lRooFit -lRooFitCore -lMinuit -lRooStats #-lMathMore

all: rivet-lib

rivet-lib:
	$(CC) -shared -fPIC $(CFLAGS) -o "RivetMC_GENSTUDY_JETCHARGE.so" MC_GENSTUDY_JETCHARGE.cc

produce-plots: produce-plots.o 
	$(CC) produce-plots.o -o produce-plots $(LDFLAGS) 
produce-plots.o: ./produce-plots.C 
	$(CC) $(CFLAGS) -c ./produce-plots.C
clean:
	rm -rf *o  produce-plots
