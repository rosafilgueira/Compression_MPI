/*************************************************************************
* Copyright © the University of Edinburgh 
* Name:        externo.c
* Author:      Rosa Filgueira
* Description: Practical-MPI Library (for MPICH).
* Year: 2011.
*************************************************************************/

----- Installing PRACTICAL MPI -----
1. Enter in lzo-2.05 directory:
	  1.a ./compile.sh 
	  1.b make 
	  1.c make install

2. Go to the previous directory:
	cd ..

3. ./precompile.sh 

----- Testing the enviroment -----
4. Enter in TEST_ENVIROMENT directory
	cd TEST_ENVIROMENT/

5. Edit compile-sequential.sh and change the variables DIRLZO2 and
DIRPRACTICAL  

6. Edit compile-mpi.sh and change the variable DIRLZO2 (the full path of xxxx/lzo-2.05/bin) and DIRPRACTICAL (the full 
directory path where externo.c is stored)

7a. if you want to compile and execute a sequential example (sequential.c)
     ./compile-sequential.sh

7b. if you want to compile and execute a parallel example (mpi-example.c)
    ./compile-mpi.sh

