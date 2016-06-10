../precompile.sh
set -x 
DIRLZO2=/exports/home/s1264372/diss/compression_files/lzo-2.05/bin
gcc -c algotest.c
gcc -g -s algotest.o ../systimer.o ../snappy.o ../lz4.o ../libbcl.a $DIRLZO2/lib/liblzo2.a -o algotest
