#!/bin/sh
#$ -N hello
#$ -cwd
#$ -pe openmpi_fillup_mark2 16
#$ -R y
#$ -l h_rt=00:30:00

. /etc/profile.d/modules.sh
module load mpich2/2.1.5 

mpicc -g -Wall -c mpi_doctor.c -o mpi_doctor.o
mpicc -g -Wall -c main.c -o main.o
mpicc -g -Wall -o latency  main.o mpi_doctor.o
