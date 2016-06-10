cd "$(dirname "$0")"
./compile-lzo.sh

# precompile snappy
gcc -Wall -g -O2 -DNDEBUG=1  -DSG=1   -c -o snappy.o ./snappy-c-master/snappy.c

# precompile lz4
gcc -c -O3 ./lz4-read-only/lz4.c -o ./lz4.o 

make

# precompile externo.c
. /etc/profile.d/modules.sh
module load mpich2/2.1.5 
mpicc -c externo.c
