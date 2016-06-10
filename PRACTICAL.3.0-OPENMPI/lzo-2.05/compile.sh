#*************************************************************************
#* Copyright © the University of Edinburgh 
#* Name:        externo.c
#* Author:      Rosa Filgueira
#* Description: Practical-MPI Library.
#* Year: 2011.
#*************************************************************************/
set -x
#export CFLAGS='-m32'
#export LDFLAGS='-m32'
MYPWD=$(pwd)
./configure --prefix=$MYPWD/bin
