#!/bin/bash -l

JOB_ID=$1
RIVET_PREFIX=${HOME}/rivet
RIVET_ANALYSIS_DIR=${RIVET_PREFIX}/Analysis/rivet-jet-charge
OUTDIR=${RIVET_ANALYSIS_DIR}/HighStatsAida/Sherpa/part${JOB_ID}
mkdir -p $OUTDIR
source ${RIVET_PREFIX}/rivetenv.sh
source ${RIVET_PREFIX}/agileenv.sh
cd ${RIVET_ANALYSIS_DIR}/MonteCarloParams/Sherpa/
export RIVET_ANALYSIS_PATH=${RIVET_ANALYSIS_DIR}
Sherpa -f Run.dat -j 2 -e 25000 -R 1370${JOB_ID} -a Rivet -A $OUTDIR

