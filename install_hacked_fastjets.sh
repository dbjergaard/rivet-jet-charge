#!/bin/bash

# copy local fastjets to build directory
echo "This is a dumb script, read it before running!"
cp ./include/Rivet/Projections/FastJets.hh ../../build/rivet/include/Rivet/Projections/FastJets.hh
cp ./src/Projections/FastJets.cc ../../build/rivet/src/Projections/FastJets.cc
cd ../../build/rivet
make -j 2 && make install 
cd -

