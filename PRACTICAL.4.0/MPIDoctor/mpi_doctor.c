
  /*
   *  Includes
   */

	#include "mpi_doctor.h"


  /*
   *  Functions
   */

  /*
   * netst is the matrix where all the measures are going to be stored.
   * Has 3 levels.
   */ 	

void mpi_doctor_names()
{
    int root = 0;
    int rank, size;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);


    char *node_name = (char *) malloc (MPI_MAX_PROCESSOR_NAME * sizeof(char));
    int node_name_length;
    MPI_Get_processor_name(node_name, &node_name_length);
    printf("%d %s\n", rank, node_name);
    free(node_name);
}

void * mpi_doctor_malloc 
(
     double ****netest,
     int nprocs
)
{
     int i, j, k ;
     int level3 ;


     /* level 1 */
     (*netest) = (void *)malloc(nprocs*sizeof(void *)) ;
     if ( NULL == (*netest) )
          {printf("Nivel 1 fallo\n");
            return NULL ;
           } 

     /* level 2 */
     for (i=0; i< nprocs; i++) 
     {
          (*netest)[i] = (void *)malloc(nprocs*sizeof(void *)) ;

          //printf("\nlevel 2 netest[%d/%d]=%p",i,nprocs,(*netest)[i]) ;

	  if ( NULL == (*netest)[i] )
	  {
              printf("Nivel 2 fallo\n");
	      return NULL ;
	  } 
     }
     /* level 3 */
     level3 = 22 ;
     for (i=0; i< nprocs; i++)
          for (j=0; j< nprocs; j++) 
          {
                (*netest)[i][j] = (double *)malloc(level3*sizeof(double)) ;
		if ( NULL == (*netest)[i][j] )
		  {printf("Nivel 3 fallo\n");
		    return NULL ;
		   } 

			for(k=0; k<level3; k++)
			    (*netest)[i][j][k] = 0.0 ;
		  }

	     return netest ;
	}

	void * mpi_doctor_free 
	(
	     double ****netest,
	     int nprocs
	)
	{
	     int i, j ;
	     int level3 ;


	     /* level 3 */
	     level3 = 22 ;
	     for (i=0; i< nprocs; i++)
		  for (j=0; j< nprocs; j++) {
			free( (*netest)[i][j] ) ;
		  }

	     /* level 2 */
	     for (i=0; i< nprocs; i++)
		  free( (*netest)[i] ) ;

	     /* level 1 */
     free( (*netest) ) ;
     (*netest) = NULL ;

     return (*netest) ;
}

int mpi_doctor_print 
(
     double ***netest,
     int nprocs
)
{
   long node_source, node_destine ;

   printf("Minimum Latency Table \n") ;
   printf("      ") ;
   for (node_destine = 0; node_destine < nprocs; node_destine++) 
   {
      printf("%3ld  ",node_destine) ;
   }

   for (node_source = 0; node_source < nprocs; node_source++) 
   {
      printf("%3ld  ",node_source) ;
      for (node_destine = 0; node_destine < node_source; node_destine++) 
      {
           printf("%lf  ", netest[node_source][node_destine][DM_MIN_LATENCY]) ;
/*
           printf("%lf,%lf,%lf  ",
                  netest[node_source][node_destine][DM_MIN_LATENCY],
                  netest[node_source][node_destine][DM_MAX_LATENCY],
                  netest[node_source][node_destine][DM_AVG_LATENCY]) ;
*/
      }
      printf("\n") ;
   }
   printf("Maximum Latency Table \n") ;
   printf("      ") ;
   for (node_destine = 0; node_destine < nprocs; node_destine++) 
   {
      printf("%3ld  ",node_destine) ;
   }

   for (node_source = 0; node_source < nprocs; node_source++) 
   {
      printf("%3ld  ",node_source) ;
      for (node_destine = 0; node_destine < node_source; node_destine++) 
      {
           printf("%lf  ", netest[node_source][node_destine][DM_MAX_LATENCY]) ;
/*
           printf("%lf,%lf,%lf  ",
                  netest[node_source][node_destine][DM_MIN_LATENCY],
                  netest[node_source][node_destine][DM_MAX_LATENCY],
                  netest[node_source][node_destine][DM_AVG_LATENCY]) ;
*/
      }
      printf("\n") ;
   }
   printf("Average Latency Table \n") ;
   printf("      ") ;
   for (node_destine = 0; node_destine < nprocs; node_destine++) 
   {
      printf("%3ld  ",node_destine) ;
   }

   for (node_source = 0; node_source < nprocs; node_source++) 
   {
      printf("%3ld  ",node_source) ;
      for (node_destine = 0; node_destine < node_source; node_destine++) 
      {
           printf("%lf  ", netest[node_source][node_destine][DM_AVG_LATENCY]) ;
/*
           printf("%lf,%lf,%lf  ",
                  netest[node_source][node_destine][DM_MIN_LATENCY],
                  netest[node_source][node_destine][DM_MAX_LATENCY],
                  netest[node_source][node_destine][DM_AVG_LATENCY]) ;
*/
      }
      printf("\n") ;
   }

   return (0) ;
}

int mpi_doctor_check 
(
     int fullTest, /* boolean. 1: check all with all, 0: only the machines */
     double *time_accuracy,
     double ***netest
)
{
   int myproc, nprocs;
   double t0, t1, time, latencia_b, pendiente;
   char *a, *b;
   int size, i, j, k,pos ;
   int node_source, node_destine ;
   MPI_Status status ;
   double array[10],aux;


   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myproc);

#ifdef DEBUG 
   printf("DEBUG: hi from %d of %d\n", myproc, nprocs);
#endif

   MPI_Barrier(MPI_COMM_WORLD);


   /* 
    * 1) Timer accuracy test 
    */
   t0 = MPI_Wtime();
   t1 = MPI_Wtime();
   while (t1 == t0) t1 = MPI_Wtime();
   *time_accuracy = (t1 - t0) * 1000000 ;

#ifdef DEBUG 
   if (myproc == 0)
      printf("DEBUG: Timer accuracy of ~%f usecs\n\n", *time_accuracy) ;
#endif

   MPI_Barrier(MPI_COMM_WORLD);


   /* 
    * 2) Communications between nodes 
    *   - Blocking sends and recvs
    *   - No guarantee of prepost, so might pass through comm buffer
    */

   size = MDOC_MAX_BUFFER * sizeof(char) * 150;
   a = (char *) malloc(size) ;
   b = (char *) malloc(size) ;

   for (i = 0; i < size; i++) {
        a[i] = (char) i;
        b[i] = 0;
   }

   for (node_source = 0; node_source < nprocs; node_source++) 
   {
      //printf("o") ; /*DEB*/
      for (node_destine = 0; node_destine < node_source; node_destine++) 
      {
           //printf("d") ; /*DEB*/
           for (i=3;i<17;i++)
           netest[node_source][node_destine][i] = 0.0 ;
	   netest[node_source][node_destine][DM_MAX_LATENCY] = 0.0 ; 
	   netest[node_source][node_destine][DM_AVG_LATENCY] = 0.0 ; 
	   netest[node_source][node_destine][DM_MIN_LATENCY] = MAXDOUBLE ;
           //##################################################################################################
           //##################################################################################################
           //##################################################################################################
           //  LATENCY
           //##################################################################################################
           //##################################################################################################
           //##################################################################################################
		   /*1º
		    *Get the latency array
		    */
		   size=1; 
		  for(i=0;i<10;i++)
		  {
		      MPI_Barrier(MPI_COMM_WORLD);
		      t0 = MPI_Wtime();

		      if (myproc == node_source) 
		      {
			 MPI_Send(a, size, MPI_BYTE, node_destine, 0, MPI_COMM_WORLD);
			 MPI_Recv(b, size, MPI_BYTE, node_destine, 0, MPI_COMM_WORLD, &status);
		      } 
		      else if (myproc == node_destine) 
		      {
			 MPI_Recv(a, size, MPI_BYTE, node_source, 0, MPI_COMM_WORLD, &status);
			 MPI_Send(b, size, MPI_BYTE, node_source, 0, MPI_COMM_WORLD);
		      }
		      t1 = MPI_Wtime();
		      time = 1.e6 * (t1 - t0) / 2.0;

		      if (myproc==node_source) {
			 array[i] = time ;
		     }
		 }
		if (myproc==node_source) {
		   
		   /*2º
		    *Sort with the burble method
		    */
		   
			for(i=0;i<10-1;i++) // Make N-1 iterations.
			   for(j=0;j<10-i-1;j++) // Look the N-i-1 pairs.
			      if(array[j+1]<array[j]) // If the element j+1 is smaller than j:
			      {
				 aux=array[j+1]; // The elements have to be interchanged
				 array[j+1]=array[j]; // of the posicion j and j+1
				 array[j]=aux; // by using an auxiliar variable.
			       }
                   ///////////////////
                   //
                   //
                   //for (i=0;i<10;i++)
                   // printf("Node source %d Latency %d is %lf\n",myproc,i,array[i]);
                   //
                   //
		   /*3º
		    *Get the average of the 5 first values
		    */
		     latencia_b=0.0;
		     for(i=0;i<5;i++)
		     latencia_b+=array[i];
		     latencia_b=latencia_b/(double)5.0; 
                     printf("Latency : %lf usec, source(%d) -> destination (%d)\n", latencia_b, node_source, node_destine) ;
               }
                    MPI_Barrier(MPI_COMM_WORLD);
           //##################################################################################################
           //##################################################################################################
           //##################################################################################################
           //  SLOPE (PENDIENTE in Spanish)
           //##################################################################################################
           //##################################################################################################
           //##################################################################################################
                           pos=0;
			    for (size = 2; size <= 134217728; size *=2)
			   {
			      //printf(".") ; /*DEB*/
			      MPI_Barrier(MPI_COMM_WORLD);
			      t0 = MPI_Wtime();

			      if (myproc == node_source) 
			      {
				 MPI_Send(a, size, MPI_BYTE, node_destine, 0, MPI_COMM_WORLD);
				 MPI_Recv(b, size, MPI_BYTE, node_destine, 0, MPI_COMM_WORLD, &status);
			      } 
			      else if (myproc == node_destine) 
			      {
				 MPI_Recv(a, size, MPI_BYTE, node_source, 0, MPI_COMM_WORLD, &status);
				 MPI_Send(b, size, MPI_BYTE, node_source, 0, MPI_COMM_WORLD);
			      }

			      t1 = MPI_Wtime();
			      time = 1.e6 * (t1 - t0) / 2.0;

			      pendiente=((time-latencia_b)/size);

			      // this may happen if transmission time was slower than the average latency
			      if (pendiente < 0) pendiente = 0;

                             //if (myproc==node_source)
                            if (myproc==node_source)
                           {  netest[node_source][node_destine][pos] = pendiente ; 
                             //printf("Latency: %lf node_source (%d)-> node_destine (%d)-->iteration (i=%d),size:%d slope%lf\n",latencia_b,node_source,node_destine,pos,(int)size,pendiente) ;
			       printf("%5d %5d %9d %8.3lf MiB/s %8.3lf \n",node_source,node_destine,(int)size, 1.0 / pendiente, pendiente) ;
                             pos++;
                          }

                         }// HERE I HAVE 10 measures for each size. Now we are going to sort all of then and get the average

       }
      printf("\n") ; /*DEB*/
   }

   //*********************MATRIX NETEST COMPLET***************************/

   MPI_Barrier(MPI_COMM_WORLD);

   /********SORT THE 10 MEASURES STORED IN NETEST****************************/

   for (node_source = 0; node_source < nprocs; node_source++) 
   {
      for (node_destine = 0; node_destine < node_source; node_destine++) 
      {       
               if (myproc==node_source)
                {
 
			for(i=0;i<26;i++) // Make N-1 iterations.
			   for(j=0;j<26-i;j++) // Look N-i-1 pairs.
			      if( netest[node_source][node_destine][j+1]<netest[node_source][node_destine][j]) // If the element j+1 is smaller than element j, then:
			      {
				 aux=netest[node_source][node_destine][j+1]; // Interchage the positions
				 netest[node_source][node_destine][j+1]=netest[node_source][node_destine][j]; // of j and j+1
				 netest[node_source][node_destine][j]=aux; // by using an aux variable.
                                //printf("values of i %d and of j %d aux %lf\n",i,j,aux);
			       }


               //for (i=0;i<17;i++)
               //   printf("Array sorted node_sourc %d-> node_dest %d, posision %i, value %lf\n",node_source, node_destine,i, netest[node_source][node_destine][i] );
               }
      }
  }
  /*******FIND THE HIEST, LOWEST AND THE AVERAGE ************************/
   
   for (node_source = 0; node_source < nprocs; node_source++) 
   {
      for (node_destine = 0; node_destine < node_source; node_destine++) 
	    {// I do not take account the two slowest and the two higest measures	
          if (myproc==node_source)
          {  
           netest[node_source][node_destine][DM_MIN_LATENCY] = netest[node_source][node_destine][1]; 
           netest[node_source][node_destine][DM_MAX_LATENCY] = netest[node_source][node_destine][24]; 
           //To get the average, I do not take account the 2 smallest average, and the 2 highiest averages.
           for (i=1;i<25;i++)
           netest[node_source][node_destine][DM_AVG_LATENCY] += netest[node_source][node_destine][i]; 
           netest[node_source][node_destine][DM_AVG_LATENCY]= (netest[node_source][node_destine][DM_AVG_LATENCY]/24);
          }
      }
   }

   for(i=1; i<nprocs; i++)
       for(j=0;j<nprocs;j++)
           for(k=26;k<29;k++)
                    if (myproc==i)
		   MPI_Send(&(netest[i][j][k]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
               else if (myproc==0)
		   MPI_Recv(&(netest[i][j][k]), 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);

   return (0) ;
}


