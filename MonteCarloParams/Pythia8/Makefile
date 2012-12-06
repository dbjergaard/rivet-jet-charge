# Compile macros using ROOT
CC=g++


ROOTSYS=/home/dave/root
export PATH:=$(ROOTSYS)/bin:$(PATH)
export PATH:=$(ROOTSYS)/bin:$(PATH)
export LD_LIBRARY_PATH:=$(ROOTSYS)/lib:$(LD_LIBRARY_PATH)

INCDIR=$(PWD)/include
WFLAGS=-Wall -Wextra
CFLAGS=-m32 -pg -ggdb -I$(ROOTSYS)/include -I$(INCDIR) -O0 $(WFLAGS)
LDFLAGS=-m32 -pg -L$(ROOTSYS)/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lGui  -lm -ldl -rdynamic -lFoam 
ROOFITLDFLAGS=-lRooFit -lRooFitCore -lMinuit -lRooStats #-lMathMore

all: JetColor

JetColor: JetColor.o JetColor_dict.o
	$(CC) JetColor.o JetColor_dict.o -o JetColor $(LDFLAGS) 
#SimpleCounting: SimpleCounting.o style.o printing.o
#	$(CC) SimpleCounting.o style.o printing.o -o SimpleCounting $(LDFLAGS)

JetColor.o: ./bin/JetColor.C ./JetColor_LinkDef.h
	$(CC) $(CFLAGS) -c ./bin/JetColor.C
	rootcint -v4 -f JetColor_dict.cxx -c -p JetColor_LinkDef.h
JetColor_dict.o: ./JetColor_dict.cxx
	$(CC) $(CFLAGS) -fPIC -c ./JetColor_dict.cxx
clean:
	rm -rf *o  JetColor
