#!/bin/sh
#$ -N latency_qdr
#$ -cwd
#$ -pe openib_smp12_qdr 24
#$ -R y
#$ -l h_rt=00:30:00

. /etc/profile.d/modules.sh

module load mpich2/2.1.5 

mpirun -np $NSLOTS ./latency
