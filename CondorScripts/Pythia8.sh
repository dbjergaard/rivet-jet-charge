#!/bin/bash -l

JOB_ID=$1
PDF=$2
RIVET_PREFIX=${HOME}/rivet
RIVET_ANALYSIS_DIR=${RIVET_PREFIX}/Analysis/rivet-jet-charge
OUTDIR=${RIVET_ANALYSIS_DIR}/HighStatsAida/Pythia8
export LHAPATH=${HOME}/rivet/local/share/lhapdf/PDFsets
mkdir -p ${OUTDIR}
source ${RIVET_PREFIX}/rivetenv.sh
source ${RIVET_PREFIX}/agileenv.sh
cd ${RIVET_ANALYSIS_DIR}/MonteCarloParams/Pythia8/
FIFONAME=/tmp/pythia8_fifo_${JOB_ID}.hepmc
mkfifo ${FIFONAME}
cat wJetProd.cnf | sed "s/seed = 1234/seed = 1370${JOB_ID}/g" > wJetProd${JOB_ID}.cnf
cat wJetProd.cnf | sed "s/pSet = 2/pSet = ${PDF}/g" > wJetProd${JOB_ID}.cnf
./runPythia wJetProd${JOB_ID}.cnf ${FIFONAME} &
rivet --analysis-path=${RIVET_ANALYSIS_DIR} -a MC_GENSTUDY_JETCHARGE --histo-file=${OUTDIR}/Pythia8_part${JOB_ID}.aida ${FIFONAME}
