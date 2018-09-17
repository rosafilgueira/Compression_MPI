

#ifndef _MPI_DOCTOR_H
#define _MPI_DOCTOR_H


  /*
   *  Includes
   */

	#include "mpi.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <values.h>


  /*
   *  Constants
   */

	#define MDOC_MAX_BUFFER (1024*1024) 

	#define DM_MIN_LATENCY 26 
	#define DM_MAX_LATENCY 27
	#define DM_AVG_LATENCY 28


  /*
   *  Functions
   */

	void * mpi_doctor_malloc 
	(
	     double ****netest,
	     int nprocs
	) ;

	void * mpi_doctor_free 
	(
	     double ****netest,
	     int nprocs
	) ;

	int mpi_doctor_print 
	(
	     double ***netest,
	     int nprocs
	) ;

	int mpi_doctor_check 
	(
	     int fullTest, /* boolean. 1: check all with all. 0: only with the machines */
	     double *time_accuracy,
             double ***netest
	) ;

void mpi_doctor_names();

#endif /* mpi_doctor.h */


