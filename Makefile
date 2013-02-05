# Compile macros using ROOT
CC=g++


 ROOTSYS=${HOME}/root
 export PATH:=$(ROOTSYS)/bin:$(PATH)
 export PATH:=$(ROOTSYS)/bin:$(PATH)
 export LD_LIBRARY_PATH:=$(ROOTSYS)/lib:$(LD_LIBRARY_PATH)

INCDIR=$(PWD)/include
WFLAGS=-Wall -Wextra
CFLAGS=-m64 -std=c++11 -pg -ggdb -I$(ROOTSYS)/include -I$(INCDIR) -O0 $(WFLAGS)
LDFLAGS=-L$(ROOTSYS)/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lGui  -lm -ldl -rdynamic -lFoam 
ROOFITLDFLAGS=-lRooFit -lRooFitCore -lMinuit -lRooStats #-lMathMore

all: produce-plots

produce-plots: produce-plots.o 
	$(CC) produce-plots.o -o produce-plots $(LDFLAGS) 
produce-plots.o: ./produce-plots.C 
	$(CC) $(CFLAGS) -c ./produce-plots.C
clean:
	rm -rf *o  produce-plots
