#!/bin/bash -l

JOB_ID=$1
TUNE=$2
RIVET_PREFIX=${HOME}/rivet
RIVET_ANALYSIS_DIR=${RIVET_PREFIX}/Analysis/rivet-jet-charge
OUTDIR=${RIVET_ANALYSIS_DIR}/HighStatsAida/Pythia6
export LHAPATH=${RIVET_PREFIX}/local/share/lhapdf/PDFsets
export AGILE_GEN_PATH=${RIVET_PREFIX}/local/generators 
mkdir -p ${OUTDIR}
source ${RIVET_PREFIX}/rivetenv.sh
source ${RIVET_PREFIX}/agileenv.sh
# do I need to add the TUNE code to the seed?
SEED_PARAM=`echo "MRPY(1)=1370${JOB_ID}"`
cd ${RIVET_ANALYSIS_DIR}
FIFONAME=/tmp/pythia6_fifo_${TUNE}_${JOB_ID}.hepmc
mkfifo ${FIFONAME}
agile-runmc Pythia6:426 --beams=LHC:7000 -n 25000 \
    -P MonteCarloParams/Pythia6/wInclusiveProduction.pars \
    -p PYTUNE=${TUNE} -p ${SEED_PARAM} -o ${FIFONAME} &
sleep 1 #let things settle after starting AGILe
rivet --analysis-path=${RIVET_ANALYSIS_DIR} -a MC_GENSTUDY_JETCHARGE \
      --histo-file=${OUTDIR}/Pythia6_tune${TUNE}_part${JOB_ID}.aida ${FIFONAME}
rm ${FIFONAME}
