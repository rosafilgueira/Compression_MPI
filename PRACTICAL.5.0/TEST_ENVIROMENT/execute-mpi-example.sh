#!/bin/bash
#PBS -N compression_MPI
#PBS -l select=6:ncpus=36
#PBS -l place=scatter:excl
#PBS -l walltime=00:02:00
#PBS -A z04


# Change to the directory that the job was submitted from
cd $PBS_O_WORKDIR

# Load any required modules
module load mpt
module load intel-mpi-17

# Set the number of threads to 1
#   This prevents any threaded system libraries from automatically
#   using threading.
export OMP_NUM_THREADS=1

# Launch the parallel job
#   Using 144 MPI processes and 36 MPI processes per node
#
#   '-ppn' option is required for all HPE MPT jobs otherwise you will get an error similar to:
#       'mpiexec_mpt error: Need 36 processes but have only 1 left in PBS_NODEFILE.'
#
mpirun -np 6 ./mpi-example  > my_stdout.txt 2> my_stderr.txt
