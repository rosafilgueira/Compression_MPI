set -x 
DIRLZO2=/lustre/home/y15/rosaf/Compression_MPI/PRACTICAL.5.0/lzo-2.05/bin
DIRPRACTICAL=/lustre/home/y15/rosaf/Compression_MPI/PRACTICAL.5.0

module load intel-mpi-17/17.0.2.174
mpicc -c mpi-example.c
mpicc -g -s mpi-example.o $DIRPRACTICAL/systimer.o $DIRPRACTICAL/snappy.o $DIRPRACTICAL/lz4.o $DIRPRACTICAL/externo.o $DIRPRACTICAL/libbcl.a $DIRLZO2/lib/liblzo2.a -o mpi-example
