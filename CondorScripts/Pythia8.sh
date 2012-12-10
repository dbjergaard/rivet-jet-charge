#!/bin/bash -l

JOB_ID=$1
RIVET_PREFIX=/home/dmb60/rivet
RIVET_ANALYSIS_DIR=${RIVET_PREFIX}/Analysis/rivet-jet-charge
OUTDIR=${RIVET_ANALYSIS_DIR}/HighStatsAida/Pythia8
mkdir -p ${OUTDIR}
source ${RIVET_PREFIX}/rivetenv.sh
source ${RIVET_PREFIX}/agileenv.sh
FIFONAME=/tmp/pythia8_fifo_${JOB_ID}.hepmc
mkfifo ${FIFONAME}
cd ${RIVET_ANALYSIS_DIR}/MonteCarloParams/Pythia8/
cat wJetProd.cnf | sed 's/seed = 1234/seed = 1370${JOB_ID}/g' > wJetProd${JOB_ID}.cnf
./runPythia wJetProd${JOB_ID}.cnf ${FIFONAME} &
rivet --analysis-path=${RIVET_ANALYSIS_DIR} -a MC_GENSTUDY_JETCHARGE --histo-file=${OUTDIR}/Pythia8_part${JOB_ID}.aida ${FIFONAME}
