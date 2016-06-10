set -x 
DIRLZO2=/exports/work/inf_dir/jobs/PRACTICAL.4.0/lzo-2.05/bin
DIRPRACTICAL=/exports/work/inf_dir/jobs/PRACTICAL.4.0

module load mpich2/2.1.5 
mpicc -c mpi-example.c
mpicc -g -s mpi-example.o $DIRPRACTICAL/systimer.o $DIRPRACTICAL/snappy.o $DIRPRACTICAL/lz4.o $DIRPRACTICAL/externo.o $DIRPRACTICAL/libbcl.a $DIRLZO2/lib/liblzo2.a -o mpi-example
