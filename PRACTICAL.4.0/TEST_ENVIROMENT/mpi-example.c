#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define buff_size 8000

int checkBuf(int *sendbuf, int *recvbuf, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
	if (sendbuf[i] != recvbuf[i])
	    return 0;
    }

    return 1;
}


int main(int argc, char **argv){
 int rank, size, i;
 int type = 0;
 MPI_Status status;
 int buff[buff_size];
 int recv_buff[buff_size];
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD,&size);
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);

 // getting node name
    char *node_name = (char *) malloc (MPI_MAX_PROCESSOR_NAME * sizeof(char));
    int node_name_length;
    MPI_Get_processor_name(node_name, &node_name_length);

    printf("Process %2d on node %s\n", rank, node_name);

    int right_neighbour = (rank + 1) % size;
    int left_neighbour = (rank + size - 1) % size;
    
    for(i=0;i<buff_size;i++) 
       buff[i]=i % 10;

    if (rank % 2)
    {
	MPI_Send(buff, buff_size, MPI_INT, right_neighbour, type, MPI_COMM_WORLD);
	MPI_Recv(recv_buff, buff_size, MPI_INT, left_neighbour, type, MPI_COMM_WORLD, &status);
    }
    else
    {
	MPI_Recv(recv_buff, buff_size, MPI_INT, left_neighbour, type, MPI_COMM_WORLD, &status);
	MPI_Send(buff, buff_size, MPI_INT, right_neighbour, type, MPI_COMM_WORLD);
    }

    if (!checkBuf(buff, recv_buff, buff_size))
	printf("Error on process %d\n", rank);
    else
	printf("Check successful on process %d\n", rank);

    MPI_Finalize();
}
