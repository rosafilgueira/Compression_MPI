#*************************************************************************
#* Copyright © the University of Edinburgh 
#* Name:        externo.c
#* Author:      Rosa Filgueira
#* Description: Practical-MPI Library.
#* Year: 2011.
#*************************************************************************/
set -x 

DIRLZO2=/Users/rosa/OTRAS_COSAS/ACCELOGIC/PRACTICAL.3.0.revision/lzo-2.05/bin
DIRPRACTICAL=/Users/rosa/OTRAS_COSAS/ACCELOGIC/PRACTICAL.3.0.revision


gcc -c sequential.c -v
gcc -g -s -v sequential.o $DIRPRACTICAL/systimer.o $DIRPRACTICAL/libbcl.a $DIRLZO2/lib/liblzo2.a -o sequential-example
