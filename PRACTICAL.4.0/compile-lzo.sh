set -x
DIRLZO2=/exports/work/inf_dir/jobs/PRACTICAL.4.0/lzo-2.05/bin
DIRLZO=/exports/work/inf_dir/jobs/PRACTICAL.4.0/lzo-2.05
gcc \
-I${DIRLZO2}/include/lzo/ \
-I${DIRLZO2}/include/ \
-I${DIRLZO}/examples/ \
-I${DIRLZO}/src/ \
-I${DIRLZO}/ \
-c lzopack2.c
