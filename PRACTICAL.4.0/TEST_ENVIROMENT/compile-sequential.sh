set -x 
DIRLZO2=/exports/work/inf_dir/jobs/PRACTICAL.4.0/lzo-2.05/bin
DIRPRACTICAL=/exports/work/inf_dir/jobs/PRACTICAL.4.0

gcc -c sequential.c
gcc -g -s sequential.o $DIRPRACTICAL/systimer.o $DIRPRACTICAL/snappy.o $DIRPRACTICAL/lz4.o $DIRPRACTICAL/libbcl.a $DIRLZO2/lib/liblzo2.a -o sequential-example
