#!/bin/bash
#backup a sample of the previous job's output
timestamp=`stat -c%Z ~/logs/rivet/Pythia6`
for f in `ls -lah ~/logs/rivet/{Pythia{6,8},Sherpa,Herwig++}/*.0 | grep '[0-9\.]*G'`
do
    echo "File larger than 1GB" > f
done
tar cvf ${timestamp}_sample_output.tar ~/logs/rivet/{Pythia{6,8},Sherpa,Herwig++}/*.0
gzip ${timestamp}_sample_output.tar

#submit
# for f in CondorScripts/{Herwig++,Pythia{6,8},Sherpa}.job
# do 
#     condor_submit $f; 
# done

