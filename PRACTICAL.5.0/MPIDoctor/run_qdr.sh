#!/bin/bash
#PBS -N latency_qdr
#PBS -l select=10:ncpus=36
#PBS -l place=scatter:excl
#PBS -l walltime=00:30:00
#PBS -A z04

module load mpt
module load intel-mpi-17
export OMP_NUM_THREADS=1
cd $PBS_O_WORKDIR

mpirun -np 24 ./latency

