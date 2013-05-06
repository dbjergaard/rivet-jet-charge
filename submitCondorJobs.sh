#!/bin/bash
#backup a sample of the previous job's output
timestamp=`stat -c%Z ~/logs/rivet/Pythia6`
echo -n "Backing up sample of logs from "; date -d @${timestamp}
for f in `ls -lah ~/logs/rivet/{Pythia{6,8},Sherpa,Herwig++}/*.0 | grep '[0-9\.]*G'`
do
    echo "File larger than 1GB" > f
done
tar cf ${timestamp}_sample_output.tar ~/logs/rivet/{Pythia{6,8},Sherpa,Herwig++}/*.0
gzip ${timestamp}_sample_output.tar
echo "Done."
echo "Cleaning up old logs and aida files..."
rm -f ~/logs/rivet/{Pythia{6,8},Sherpa,Herwig++}/*
rm -rf ~/rivet/Analysis/rivet-jet-charge/HighStatsAida/{Pythia{6,8},Sherpa}/*
rm -f ~/rivet/Analysis/rivet-jet-charge/MonteCarloParams/Herwig++/*_part*
echo "Done."
#submit
echo "Updating to latest analysis code"
cd ~/rivet/Analysis/rivet-jet-charge
git pull
source ../../rivetenv.sh
rivet-buildplugin RivetMC_GENSTUDY_JETCHARGE.so MC_GENSTUDY_JETCHARGE.cc
echo "Done."
for f in CondorScripts/{Herwig++,Pythia{6,8},Sherpa}.job
do 
    echo $f
    condor_submit $f; 
done
echo "Done"
