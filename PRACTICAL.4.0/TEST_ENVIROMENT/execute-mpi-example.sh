#!/bin/sh
#$ -N mpi_example
#$ -cwd
#$ -pe openmpi_fillup_mark2 16
#$ -R y
#$ -l h_rt=00:30:00

. /etc/profile.d/modules.sh

module load mpich2/2.1.5 

mpirun -np 16 ./mpi-example
