#*************************************************************************
#* Copyright © the University of Edinburgh 
#* Name:        externo.c
#* Author:      Rosa Filgueira
#* Description: Practical-MPI Library.
#* Year: 2011.
#*************************************************************************/
set -x 
DIRLZO2=/exports/work/inf_dir/jobs/PRACTICAL.3.0/lzo-2.05/bin
DIRPRACTICAL=/exports/work/inf_dir/jobs/PRACTICAL.3.0

gcc -c sequential.c
gcc -g -s sequential.o $DIRPRACTICAL/systimer.o $DIRPRACTICAL/libbcl.a $DIRLZO2/lib/liblzo2.a -o sequential-example
