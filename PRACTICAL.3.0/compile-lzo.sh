#*************************************************************************
#* Copyright © the University of Edinburgh 
#* Name:        externo.c
#* Author:      Rosa Filgueira
#* Description: Practical-MPI Library.
#* Year: 2011.
#*************************************************************************/
set -x
MYPWD=$(pwd)
DIRLZO2=$MYPWD/lzo-2.05/bin
DIRLZO=$MYPWD/lzo-2.05
gcc \
-I${DIRLZO2}/include/lzo/ \
-I${DIRLZO2}/include/ \
-I${DIRLZO}/examples/ \
-I${DIRLZO}/src/ \
-I${DIRLZO}/ \
-c lzopack2.c
