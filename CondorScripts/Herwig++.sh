#!/bin/bash -l

JOB_ID=$1
RIVET_PREFIX=/home/dmb60/rivet
RIVET_ANALYSIS_DIR=${RIVET_PREFIX}/Analysis/rivet-jet-charge
source ${RIVET_PREFIX}/rivetenv.sh
source ${RIVET_PREFIX}/agileenv.sh
cd ${RIVET_ANALYSIS_DIR}/MonteCarloParams/Herwig++/
export RIVET_ANALYSIS_PATH=${RIVET_ANALYSIS_DIR}
Herwig++ run -N 25000 -s 1370${JOB_ID} --tag=`echo "_part${JOB_ID}"` LHC.run

