module load mpt
module load intel-mpi-17

mpicc -g -Wall -c mpi_doctor.c -o mpi_doctor.o
mpicc -g -Wall -c main.c -o main.o
mpicc -g -Wall -o latency  main.o mpi_doctor.o



