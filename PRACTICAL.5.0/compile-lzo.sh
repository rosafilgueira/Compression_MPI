set -x
DIRLZO2=$HOME/Compression_MPI/PRACTICAL.5.0/lzo-2.05/bin
DIRLZO=$HOME/Compression_MPI/PRACTICAL.5.0/lzo-2.05
gcc \
-I${DIRLZO2}/include/lzo/ \
-I${DIRLZO2}/include/ \
-I${DIRLZO}/examples/ \
-I${DIRLZO}/src/ \
-I${DIRLZO}/ \
-c lzopack2.c
