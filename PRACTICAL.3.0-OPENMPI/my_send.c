#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
  {
  int ret;
  /* ...gather whatever statistics you want ... */
  ret = PMPI_Send(buf, count, datatype, dest, tag, comm);
  /* ...gather whatever statistics you want ... */
  return ret;
}
