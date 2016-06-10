#*************************************************************************
#* Copyright © the University of Edinburgh 
#* Name:        externo.c
#* Author:      Rosa Filgueira
#* Description: Practical-MPI Library.
#* Year: 2011.
#*************************************************************************/
cd "$(dirname "$0")"
./compile-lzo.sh
make

# precompile externo.c
mpicc -c externo.c
