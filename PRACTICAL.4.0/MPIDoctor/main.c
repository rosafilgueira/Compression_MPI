
#include "mpi.h"
#include "mpi_doctor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
   int ret , rank, size;
   double time_accuracy ;
   double ***netest ;


   setbuf(stdin, NULL);

   MPI_Init(&argc,&argv); 
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);

   mpi_doctor_malloc(&netest,size) ;
   if (NULL == netest)
   {
       printf("ERROR: fail to alloc memory :| \n") ;
       return (0) ;
   }

   mpi_doctor_names();
   mpi_doctor_check(1, &time_accuracy, netest) ;
   if (rank == 0)
   {
       printf("\n time_accuracy = %f usec \n", time_accuracy);
       mpi_doctor_print(netest,size) ;
   }

   mpi_doctor_free(&netest,size) ;
   MPI_Finalize();
   return (0) ;
}




