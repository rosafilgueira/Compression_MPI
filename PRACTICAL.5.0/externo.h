/*************************************************************************
 * * Copyright © the University of Edinburgh 
 * * Name:        externo.c
 * * Author:      Rosa Filgueira
 * * Description: Practical-MPI Library.
 * * Year: 2011.
 * *************************************************************************/
 int MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm); 
int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
