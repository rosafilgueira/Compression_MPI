/*************************************************************************
* Copyright © the University of Edinburgh 
* Name:        externo.c
* Author:      Rosa Filgueira
* Description: Practical-MPI.4.0 Library.
* Year: 2014.
/*************************************************************************
----- Installing PRACTICAL MPI -----
1. Enter in lzo-2.05 directory:
	 ./compile.sh 
	  make 
	  make install

2. Go to the previous directory:
	cd ..
3. Execute compile-lzo.sh
    ./compile-lzo.sh 

4. Execute precompile.sh
	./precompile.sh

----- Testing Installation -----
5. Enter in Test directory
	cd TEST_ENVIROMENT/

6. if you want to compile and execute a sequential example (sequential.c)
    6.a.1 ./compile-sequential.sh
    6.a.2 ./sequential-example rle
	Note: You can run ./sequential-example , and see the compression algorithms availables. rle is just one of them.

7. if you want to compile and execute a sequential example (sequential.c)
    7.b.1 ./compile-mpi.sh
    7.b.2 (qsub) execute-mpi-example.sh   ---> Important: EDIT execute-mpi-example.sh to indicate your budget under #PBS -A (e.g. z04 )
	Note: you can do small test in the login node
		module load intel-mpi-17
 		export OMP_NUM_THREADS=1
 		mpirun -np 6 ./mpi-example  


