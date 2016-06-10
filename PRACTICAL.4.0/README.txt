----- Installing PRACTICAL MPI -----
1. Enter in lzo-2.05 directory:
	 ./compile.sh 
	  make 
	  make install

2. Go to the previous directory:
	cd ..


3. Edit compile-lzo.sh script and change the variables DIRLZO2 and DIRLZO
  DIRLZO2=XXXX/lzo-2.05/bin
  DIRLZO=XXXX/lzo-2.05

4. Execute precompile.sh for

----- Testing the enviroment -----
5. Enter in Test directory
	cd TEST_ENVIROMENT/

6. Edit compile-sequential.sh and change the variable DIRLZO 

7. Edit compile-mpi.sh and change the variable DIRLZO, and also the module that you should to load for execute mpicc. I think that it is module load openmpi/ethernet/gcc/1.4.2-gcc_4.1.2 but I am not sure.


8a. if you want to compile and execute a sequential example (sequential.c)
    3.a.0 Edit compile-sequential.sh (Only check if you have installed everything ok)
    3.a.1 ./compile-sequential.sh
    3.a.2 ./execute-sequential-example.sh

9b. if you want to compile and execute a sequential example (sequential.c)
    3.b.0 Edit compile-mpi.sh (Only check if you have installed everything ok)
    3.b.1 ./compile-mpi.sh
    3.b.2 ./execute-mpi-example.sh


