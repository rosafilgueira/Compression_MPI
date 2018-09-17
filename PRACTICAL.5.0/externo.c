/*************************************************************************
* Copyright © the University of Edinburgh 
* Name:        externo.c
* Author:      Rosa Filgueira
* Description: Practical-MPI Library.
* Year: 2011.
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"


#define SIZE 100
#define PRINT 0

int Num_messages;
typedef struct{
  int state;
  int type; // 1 recv, 0 send;
  void * buf_user;
  void * buf_recv;
  MPI_Request request_recv;
} table;

table table_request[SIZE];
int start_int,start_float,start_double,start_byte;
int algorithm_int, algorithm_float, algorithm_double,algorithm_byte;
int low_limit_int, low_limit_float, low_limit_double, low_limit_byte;
int up_limit_int, up_limit_float, up_limit_double, up_limit_byte;
int messages_without_compress_int,messages_without_compress_float, messages_without_compress_double, messages_without_compress_byte;
int num_messages_int, num_messages_float,num_messages_double,num_messages_byte; 
int interval_start_int, interval_start_float,interval_start_double,interval_start_byte; 
int interval_end_int, interval_end_float,interval_end_double,interval_end_byte; 
int mistake_consecutive_int, mistake_consecutive_float,mistake_consecutive_double,mistake_consecutive_byte; 
int Rank, Size;
// this array will hold node_ids of each process running
int *process_nodes;

/*bandwidth
 These values depend on the cluster!
 We only need the bandwidth for calculate the speedup
*/


double bandwidth(int total_len)
{
  double band;
    if (total_len <=4096 )
        band=0.030;
        else if ((total_len>4096)&&(total_len<=8192))
        band=0.026;
        else if ((total_len>8192)&&(total_len<=16384))
        band=0.022;
        else if ((total_len>16384)&&(total_len<=32768))
        band= 0.020;
        else if ((total_len>32768)&&(total_len<=65536))
        band= 0.018;
        else if ((total_len>65536)&&(total_len<=131072))
        band= 0.014;
        else
        band= 0.012;

return band;
}


double speculate_send_time(int size)
{
    return size * bandwidth(size);
}


void init_process_nodes(int process_count)
{
    process_nodes = (int *) malloc (process_count * sizeof(int));
}

void free_process_nodes()
{
    free(process_nodes);
}

/* This function is written this way for eddie because of simplicity,
   on some other cluster it should most probably be changed to some hash function
   node names on eddie are like "eddie439"
*/
int get_node_id(char* node_name, int node_name_length)
{
    int i, node_id = 0;
    for (i = 5; i < node_name_length; ++i)
    {
	node_id = node_id * 10 + node_name[i] - '0';
    }
    return node_id;
}

void gather_process_nodes()
{
    int rank, size;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // getting node name
    char *node_name = (char *) malloc (MPI_MAX_PROCESSOR_NAME * sizeof(char));
    int node_name_length;
    MPI_Get_processor_name(node_name, &node_name_length);

    // getting node id
    int node_id = get_node_id(node_name, node_name_length);

    init_process_nodes(size);
    process_nodes[rank] = node_id;
    MPI_Allgather(MPI_IN_PLACE, 1, MPI_INT, process_nodes, 1, MPI_INT, comm);
}

int is_mpicw(MPI_Comm comm)
{
    int result;
    MPI_Comm_compare(comm, MPI_COMM_WORLD, &result);

    if (result == MPI_IDENT ||
        result == MPI_CONGRUENT)
	return 1;
    else
	return 0;
}

// returns false if the communicator isn't MPI_COMM_WORLD or
// if the processes are on different nodes
int different_nodes(MPI_Comm comm, int dest)
{
	// if comm is not MPI_COMM_WORLD we need to translate ranks
	if (is_mpicw(comm))
	{
    if (process_nodes[Rank] != process_nodes[dest])
		return 1;
	    else
		return 0;
	}
  
  return 0;
}

/* Adaptive Compression functions*/
//###########################################################
//These two fuctions have values that needed to be precalculated
//in each cluster: Bandwidth and relation of compression and decompression 
//for every algorithm compression.
//###########################################################


 /*select_relation_decompression
 give the time of decompression depending on:
 1.Compression Algorithm (type_algo)
 2.Compression Factor (fact)
 */
float select_relation_decompression(int type_algo, float fact)
{ float a;
   switch (type_algo)
   {
   case 12: if (fact <=25)
             a=0.19;
            else if(fact <=50)
             a=0.20;
             else if (fact <=75)
              a=0.22;
               else
                  a=1.01;

           break;
   case 1:
           if (fact<=25)
             a=0.34;
            else if(fact <=50)
             a=0.30;
             else if (fact <=75)
              a=0.25;
               else
                  a=0.15;
           break;

   case 2:
           if (fact<=25)
             a=1.39;
            else if(fact <=50)
             a=1.32;
             else if (fact <=75)
              a=1.01;
               else
                  a=0.35;
           break;
   case 5:
           if (fact<=25)
             a=0.46;
            else if(fact <=50)
             a=0.46;
             else if (fact <=75)
              a=0.45;
               else
                  a=0.44;
           break;
   default:
           if (fact<=25)
             a=0.98;
            else if(fact <=50)
             a=0.96;
             else if (fact <=75)
              a=0.93;
               else
                  a=0.90;
           break;
}
return a;
}

/***********************************************************/
/*start_init
Initializes the "start" variables.
If a start variable value is 1, this means, that for this type of data, we dont have yet the algorithm
to compress the data.
Could initialize all of start variables or only one specific. Depends of parameter len.
*/

void start_init(int len)
{
if (len==0)
{
  start_int=1;
  start_float=1;
  start_double=1;
  start_byte=1;

 algorithm_int=0; 
 algorithm_float=0; 
 algorithm_double=0; 
 algorithm_byte=0; 
 
 low_limit_int=0; 
 low_limit_float=0; 
 low_limit_double=0; 
 low_limit_byte=0; 
 
 up_limit_int=0; 
 up_limit_float=0; 
 up_limit_double=0; 
 up_limit_byte=0;
 
 messages_without_compress_int=0; 
 messages_without_compress_float=0; 
 messages_without_compress_double=0; 
 messages_without_compress_byte=0;

 num_messages_int=0;
 num_messages_float=0;
 num_messages_double=0;
 num_messages_byte=0;

 interval_start_int=0;
 interval_start_float=0;
 interval_start_double=0;
 interval_start_byte=0;


 interval_end_int=0;
 interval_end_float=0;
 interval_end_double=0;
 interval_end_byte=0;

 mistake_consecutive_int=0;
 mistake_consecutive_float=0;
 mistake_consecutive_double=0;
 mistake_consecutive_byte=0;
}
else
  {
   switch (len)
   {
    case 4:
           start_int=1;
           algorithm_int=0;
           low_limit_int=0;
           up_limit_int=0;
           messages_without_compress_int=0;
           num_messages_int=0; 
           interval_start_int=0; 
           interval_end_int=0; 
           mistake_consecutive_int=0; 
           break;
   case 8:
           start_float=1;
           algorithm_float=0;
           low_limit_float=0;
           up_limit_float=0;
           messages_without_compress_float=0; 
           num_messages_float=0; 
           interval_start_float=0; 
           interval_end_float=0; 
           mistake_consecutive_float=0; 
           break;
  case 16:      
           start_double=1;
           algorithm_double=0;
           low_limit_double=0;
           up_limit_double=0;
           messages_without_compress_double=0; 
           num_messages_double=0; 
           interval_start_double=0; 
           interval_end_double=0; 
           mistake_consecutive_double=0; 
            break;
  default:
             start_byte=1;    
             algorithm_byte=0;
             low_limit_byte=0;
             up_limit_byte=0;
             messages_without_compress_byte=0; 
             num_messages_byte=0; 
             interval_start_byte=0; 
             interval_end_byte=0; 
             mistake_consecutive_byte=0; 
            break;

 }

}
}
/*initial_evaluation
Gives the value of the start value depending on the len. 
This means the type of data.
*/

int initial_evaluation(int len)
{
    return 0;
/*if (len==4)
  return start_int;
else
  if (len==8)
   return start_float;
   else
      if (len==16)
        return start_double;
             else
                return start_byte; */


}
/* time_algorithm
Returns the time of send a message compressed by the algorithm specified in the 
parameter algorithm.
*/

double time_algorithm (const void *buf, int total_len, int algorithm)
{double t1,t2,t3,t_c,t_d,time_algorithm;
 float fact,relation;
 char *vSend2;
 int data_c;
 double band; 
 t1=MPI_Wtime();
 vSend2=(char*) malloc ((total_len+10000)*sizeof(char));
 data_c=compresion(algorithm,buf,vSend2,total_len,0,1);
 t2=MPI_Wtime();
 t3=MPI_Wtime();

 t_c=(2*t2-t1-t3)*1.e06;
 fact=(float)((total_len-data_c)*100)/(float)(total_len);
relation=select_relation_decompression(algorithm,fact);
t_d=t_c*relation;
 band=bandwidth(data_c);
 time_algorithm=t_c+t_d+(data_c*band);
 return time_algorithm;
}
/*maximun
Return the algorithm with higher speedup.
In case the higher speedup of sending a message is less than 1
returns 0. This means, no compress for this type of data.

*/

int maximun(double speed[])
{
double max;
int i,max_algo, max_result;
max=speed[0];
max_algo=0;
for (i=1;i<4;i++)
  if (speed[i]>max)
    {max=speed[i];
     max_algo=i;
    }
 if (max<1)
 max_result=0;
 else
  if (max_algo==0)
    max_result=12;
  else
    if (max_algo==1)
    max_result=1;
     else
       if (max_algo==2)
        max_result=2;
      else
         max_result=5;
 
return max_result;
}

/* search_algorithm
Returns the best algorithm for a type of data.
Could be LZO,RLE, HUFF, RICE or NONE.
*/
int search_algorithm(const void *buf, int len, int total_len)
{
int algorithm,algorithm_result;
double time_rle,time_lzo,time_huff,time_rice, time_lz4, time_snappy, time_orig;
double band, speed[6];
/* compression with RLE*/
 algorithm=1;
 time_rle=time_algorithm(buf,total_len,algorithm);
/* compression with LZO*/
 algorithm=12;
 time_lzo=time_algorithm(buf,total_len,algorithm);
/* compression with HUFF*/
 algorithm=2;
 time_huff=time_algorithm(buf,total_len,algorithm);
/* compression with RICE*/
 algorithm=8;
 time_rice=time_algorithm(buf,total_len,algorithm);

/* compression with Snappy */
 algorithm=13;
time_snappy=time_algorithm(buf,total_len,algorithm);
/* compression with LZ4 */
 algorithm=14;
 time_lz4=time_algorithm(buf,total_len,algorithm);
 

 // calculating communication time without compression
 band=bandwidth(total_len);
 time_orig=total_len*band;
 //printf ("valores de tiempo: orig %lf, rle %lf,lzo %lf, huff %lf ,rice %lf\n",time_orig,time_rle,time_lzo,time_huff,time_rice); 
 
 speed[0]=time_orig/time_lzo;
 speed[1]=time_orig/time_rle;
 speed[2]=time_orig/time_huff;
 speed[3]=time_orig/time_rice;
 speed[4]=time_orig/time_snappy;
 speed[5]=time_orig/time_lz4;

 //printf("Valores de speed: lzo %lf, rle %lf, huff %lf, rice %lf \n",speed[0],speed[1],speed[2],speed[3]);
 algorithm_result=maximun(speed);
 if (len==4)
    { 
     algorithm_int=algorithm_result;
     start_int=0;
     if (algorithm_result==0)
        low_limit_int=total_len;
     else
        up_limit_int=total_len;
    }
 else
    if (len==8)
     {
      algorithm_float=algorithm_result;
      start_float=0;
      if (algorithm_result==0)
         low_limit_float=total_len;
      else
         up_limit_float=total_len;
     }
    else
       if (len==16)
       {
         algorithm_double=algorithm_result;
         start_double=0;
         if (algorithm_result==0)
            low_limit_double=total_len;
         else
            up_limit_double=total_len;
       }
        else
         {
          algorithm_byte=algorithm_result;
          start_byte=0;
          if (algorithm_result==0)
            low_limit_byte=total_len;
          else
            up_limit_byte=total_len;
         }
          
return algorithm_result;  

}

/* select_algorithm
Returns the algorithm for this type of data.
This was calculated by search_algorithm.

*/
int select_algorithm(const void *buf, int len, int total_len)
{

    return 14;
   /*int aux_algorithm;

   if (len==4)
    {
     if ((total_len>=up_limit_int) && (up_limit_int!=0))
        return algorithm_int;
     else if ((total_len<=low_limit_int) && (low_limit_int!=0))
         return 0;
      else
       { 
        aux_algorithm=search_algorithm(buf,len,total_len);
        return aux_algorithm;
       }
     }
    else
      if (len==8)
        {
         if ((total_len>=up_limit_float)&& (up_limit_float!=0))
             return algorithm_float;
         else if ((total_len <=low_limit_float)&&(low_limit_float!=0))
              return 0;
         else 
            {
            aux_algorithm=search_algorithm(buf,len,total_len);
            return  aux_algorithm;
            }
        }
   else
      if (len==16)
       {
         if ((total_len>=up_limit_double )&& (up_limit_double!=0))
             return algorithm_double;
         else if ((total_len <=low_limit_double)&& (low_limit_double!=0))
              return 0;
         else 
            {
            aux_algorithm=search_algorithm(buf,len,total_len);
            return  aux_algorithm;
            }
       }
   else
       {
         if (total_len>=up_limit_byte)
             return algorithm_byte;
         else if (total_len <=low_limit_byte)
              return 0;
         else 
            {
            aux_algorithm=search_algorithm(buf,len,total_len);
            return  aux_algorithm;
            }
       } */
}

 void update_messages_without_compression(int len)
 {
   switch (len)
   {
   case 4: messages_without_compress_int++; 
           break;
   case 8: messages_without_compress_float++;
           break; 
   case 16:messages_without_compress_double++;
           break; 
   default: messages_without_compress_byte++; 
           break;
   }
}

void add_message (int len)
{
switch (len)
{
case 4: num_messages_int++;
        break;
case 8: num_messages_float++;
        break;
case 16:num_messages_double++;
        break;
default:num_messages_byte++;
        break;
}

}


 void check_error(int len)
 { 
   int distance;
   switch (len)
   {
   case 4:   
  	if (interval_start_int==0)
        	  interval_start_int=num_messages_int;
    	else
        {
         	interval_end_int=num_messages_int;
          	distance=interval_end_int-interval_start_int;
          	interval_start_int=num_messages_int;
          	if (distance<5)
          		mistake_consecutive_int++;
        }
        break;
   case 8:
  	if (interval_start_float==0)
        	  interval_start_float=num_messages_float;
    	else
        {
         	interval_end_float=num_messages_float;
          	distance=interval_end_float-interval_start_float;
          	interval_start_float=num_messages_float;
          	if (distance<5)
          		mistake_consecutive_float++;
        }
        break;
   case 16:
  	if (interval_start_double==0)
        	  interval_start_double=num_messages_double;
    	else
        {
         	interval_end_double=num_messages_double;
          	distance=interval_end_double-interval_start_double;
          	interval_start_double=num_messages_double;
          	if (distance<5)
          		mistake_consecutive_double++;
        }
        break;
   default:
  	if (interval_start_byte==0)
        	  interval_start_byte=num_messages_byte;
    	else
        {
         	interval_end_byte=num_messages_byte;
          	distance=interval_end_byte-interval_start_byte;
          	interval_start_byte=num_messages_byte;
          	if (distance<5)
          		mistake_consecutive_byte++;
        }
        break;
  
   } 
 }

 int check_reevaluation(int type, int len)
 { if (type==1)
   switch (len)
   {
   case 4: 
           if (mistake_consecutive_int > 3)
              return 1;
           else
              return 0;
           break;
   case 8: 
           if (mistake_consecutive_float > 3)
              return 1;
           else
              return 0;
           break;
  case 16: 
         if (mistake_consecutive_double > 3) 
            return 1;
         else
            return 0;
        break;
  default:
        if (mistake_consecutive_byte > 3)
           return 1;
        else
           return 0;
        
           
   }

   else 
   {
   switch (len)
   {
   case 4: if (messages_without_compress_int>1000)
              return 1; 
           else
              return 0;
           break;
   case 8: if (messages_without_compress_float>1000)
              return 1; 
           else
              return 0;
           break; 
   case 16: if (messages_without_compress_double>1000)
              return 1; 
           else
              return 0;
           break; 
   default: if (messages_without_compress_byte>1000)
              return 1; 
           else
              return 0;
           break;
   }
  } 
 }

/* Request functions*/
//###########################################################
int hash(MPI_Request  request_position) {
    printf("request_position %d\n", (int)&request_position);	
    return abs((int)&request_position % SIZE);
}

void table_init() {
  int i;
  for (i=0;i<SIZE;i++){
    table_request[i].state= 0;
    table_request[i].type= 0;
   }
}

int first_free(int position)
{
int npos;
if (table_request[position].state==0)
 return position;
else
  {
   npos= abs(position+1 %SIZE);
   if (npos>=SIZE)
     npos=0;
   while ((table_request[npos].state==1)&&(npos!=position))
   {
     npos=abs((npos+1) % SIZE) ;
     if (npos>=SIZE)
       npos=0;
   }
   if (npos==position)
    return -1;
  else
   return npos;
   
  }
}


int search_position(MPI_Request request)
{int pos,npos;
  pos=hash(request);
  if ((table_request[pos].request_recv==request)&&(table_request[pos].state==1))
  return pos;
  else
  { 
    npos=abs((pos+1) % SIZE);
    if (npos>=SIZE)
      npos=0;
    while ((table_request[npos].state==1)&&(npos!=pos))
     {
      if ((table_request[npos].state==1)&&((table_request[npos].request_recv)==request)) 
        return npos;
      npos=abs((npos+1)%SIZE);
      if (npos>=SIZE)
         npos=0;
     }
  }
  return -1;
}

int free_position(int position)
 {
  table_request[position].state=0;
  table_request[position].request_recv=0;
  table_request[position].type=0;

 }

/* MPI Re-definition functions*/
//###########################################################

  int MPI_Init(int *argc, char ***argv){
   table_init();
   start_init(0);
   int initResult = PMPI_Init(argc,argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
   MPI_Comm_size(MPI_COMM_WORLD, &Size);
   gather_process_nodes();
   Num_messages=0;
   return initResult;
  }

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm){
   int reevaluation,start,rank,len,total_len,data_c,flag, algorithm,type_revaluation;
   char *vSend2; 
   float fact;
   //MPI_Comm_rank(comm,&rank);
   flag=0;
   MPI_Type_size(datatype,&len);
   total_len=len*count;
   if (total_len > 2048 && different_nodes(comm, dest))
   { 
     //printf("total_len is %d before compression\n", total_len);
    /*1.Size it is mayor than 2KB.*/
    /*2.Check if it is the first time than compress this type of messages*/
     start=initial_evaluation(len);
     if (start==1)
       algorithm=search_algorithm(buf,len,total_len);
     else
       algorithm=select_algorithm(buf,len,total_len);
 
     if (algorithm!=0)
      {
        add_message(len); 
        vSend2=(char*) malloc ((total_len+10000)*sizeof(char));
        data_c=compresion(algorithm,buf,vSend2,total_len,0,0);
        //printf("compressed_len is %d after compression\n",data_c);
	      PMPI_Send(vSend2,data_c,MPI_CHAR,dest,tag,comm);
        free(vSend2);
	/* commenting for now
        fact=(float)((total_len-data_c)*100)/(float)(total_len);
        if (fact <1)
          check_error(len);
        type_revaluation=1;
	*/
      } 
     else
      {
       PMPI_Send(buf,total_len,MPI_CHAR,dest,tag,comm);
       update_messages_without_compression(len);
       type_revaluation=2; 
      }
     /*reevaluation=check_reevaluation(type_revaluation,len);
       if (reevaluation==1)
       start_init(len);
     */
   }
   else{
     PMPI_Send(buf,total_len,MPI_CHAR,dest,tag,comm);
  }
   Num_messages++;
   return 0;      
 }

 int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status){
   int len,type_algo,total_len,data_c,len_recv,flag;
   char *vSend3;
   int rank; 
 //  MPI_Comm_rank(comm,&rank);
   MPI_Type_size(datatype,&len);
   total_len=len*count;
   int total_count;
   vSend3=(char*) malloc ((total_len+5000)*sizeof(char));
   len_recv=total_len+sizeof(int)+(3*sizeof(char));
   PMPI_Recv(vSend3,len_recv,MPI_CHAR,source,tag,comm,status);
   MPI_Get_count(status,MPI_CHAR,&total_count);
   flag=estudio_cabecera(vSend3);
    if (flag==1)
    { total_len=total_count;  
      descompresion(vSend3,buf,total_len,0);
    }
   else
    memcpy(buf,vSend3,total_count);
    free(vSend3);
  return 0; 
  }

 int MPI_Irecv( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request ) {
   int len,total_len,position,position_free;
   void *buf_tmp; 
   int rank; 
   // MPI_Comm_rank(comm,&rank);
   MPI_Type_size(datatype,&len);
   total_len=(len*count)+7;
   buf_tmp=(char*)malloc(total_len*sizeof(char)); 
   PMPI_Irecv(buf_tmp,total_len,MPI_CHAR,source,tag,comm,request);
  
   position=hash(*request);
   position_free=first_free(position);
   table_request[position_free].state=1;
   table_request[position_free].request_recv=*request;
   table_request[position_free].buf_user=buf;
   table_request[position_free].buf_recv=buf_tmp;
   table_request[position_free].type=1;
 }

int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int type_revaluation,reevaluation,len,total_len,data_c,flag,position,position_free,algorithm;
  char *vSend2;
  float fact;
  int start; 
  // MPI_Comm_rank(comm,&rank);
   flag=0; 
   MPI_Type_size(datatype,&len);
   total_len=len*count;
   if (total_len > 2048 && different_nodes(comm, dest))
   {
     start=initial_evaluation(len);
     if (start==1)
       algorithm=search_algorithm(buf,len,total_len);
     else
       algorithm=select_algorithm(buf,len,total_len);
    if (algorithm!=0)
    {
     vSend2=(char*) malloc ((total_len+10000)*sizeof(char));
     data_c=compresion(algorithm,buf,vSend2,total_len,0,0);
     PMPI_Isend(vSend2,data_c,MPI_CHAR,dest,tag,comm,request);
     fact=(float)((total_len-data_c)*100)/(float)(total_len);
     if (fact <1)
       check_error(len);
     type_revaluation=1;
    }
    else
      {  
       PMPI_Isend(buf,total_len,MPI_CHAR,dest,tag,comm,request);
       update_messages_without_compression(len);
       type_revaluation=2; 
      }
       reevaluation=check_reevaluation(type_revaluation,len);
       if (reevaluation==1)
       start_init(len);
   }
   
   else
    PMPI_Isend(buf,total_len,MPI_CHAR,dest,tag,comm,request);
   
   Num_messages++;
   position=hash(*request);
   position_free=first_free(position);
   table_request[position_free].state=1;
   table_request[position_free].request_recv=*request;
   table_request[position_free].type=0;
    return 0;      
  }

int MPI_Wait( MPI_Request  *request, MPI_Status   *status) {
  int flag=0;
  int total_len, total_count,position,original_size,rank;
  char *vSend;
  MPI_Request request2;
    
   request2=*request;
   PMPI_Wait(request,status);
   position= search_position(request2);
   if(table_request[position].type==1)
    { 
     if (position!=-1) 
     {
     flag=estudio_cabecera(table_request[position].buf_recv);
     MPI_Get_count(status,MPI_CHAR,&total_count);
     total_len=total_count;
     if (flag==1)
      {
      vSend=(char*) malloc ((total_len+10000)*sizeof(char));
      original_size=descompresion(table_request[position].buf_recv,table_request[position].buf_user,total_len,0);
      }
     else
      {original_size=total_count;
       memcpy(table_request[position].buf_user,table_request[position].buf_recv,total_count);
      } 
     free(table_request[position].buf_recv);
     free_position(position);
    }
   else
     printf("Problems!!\n");
 
   }
    else
     free_position(position);
  return original_size; 
  }

int MPI_Waitall(int count, MPI_Request array_of_request[], MPI_Status array_of_statuses[])
{int i;

  for (i=0;i<count;i++)
    MPI_Wait(&array_of_request[i],&array_of_statuses[i]);
}

int MPI_Bcast( void *buf,int count, MPI_Datatype datatype,int root, MPI_Comm comm)
{ int type_revaluation,reevaluation,start,rank,total_len,flag,algorithm,len,data_c,len_recv;
  char *vSend2;
  float fact;  
  MPI_Comm_rank(comm,&rank);
  MPI_Type_size(datatype,&len);
  total_len=len*count;
  vSend2=(char*) malloc ((total_len+10000)*sizeof(char));
  if (rank==root)
   {
    if (total_len > 2048)
     {
     start=initial_evaluation(len);
     if (start==1)
       algorithm=search_algorithm(buf,len,total_len);
     else
       algorithm=select_algorithm(buf,len,total_len);
       if (algorithm!=0)
       {
        data_c=compresion(algorithm,buf,vSend2,total_len,0,1);
        PMPI_Bcast(vSend2,data_c,MPI_CHAR,root,comm);
        fact=(float)((total_len-data_c)*100)/(float)(total_len);
        if (fact <1)
         check_error(len);
        type_revaluation=1;
      }
      else
      {
       PMPI_Bcast(buf,total_len,MPI_CHAR,root,comm);
       update_messages_without_compression(len);
       type_revaluation=2; 
      }
       reevaluation=check_reevaluation(type_revaluation,len);
       if (reevaluation==1)
       start_init(len);
      }
     else
     PMPI_Bcast(buf,total_len,MPI_CHAR,root,comm);
  }
  else
   { 
     len_recv=total_len+sizeof(int)+(3*sizeof(char));
     PMPI_Bcast(vSend2,len_recv,datatype,root,comm);
     flag=estudio_cabecera(vSend2);
      if (flag==1)
       descompresion(vSend2,buf,len_recv,1);
     else
      memcpy(buf,vSend2,len_recv);
   }
  free (vSend2);
  return 0; 

}

int MPI_Scatter ( const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
 int size,len,data,desp,rank,i;
 char *buf;
 MPI_Status status; 
 MPI_Request request;
 MPI_Type_size(sendtype,&len);
 MPI_Comm_rank(comm,&rank);
 data=sendcount*len; 
 
 MPI_Comm_size(comm,&size);
 desp=0;
 if (rank==root)
 { 
  for (i=0;i<size;i++)
  {
   if (i==root)
   memcpy(recvbuf,sendbuf+desp,data);
   else
   MPI_Isend(sendbuf+desp, data, MPI_CHAR,i,0,comm,&request); 
   desp=desp+data;
  }
}
else 
MPI_Irecv(recvbuf,data,MPI_CHAR,root,0,comm,&request);
  MPI_Wait(&request,&status);
return 0;
}

int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
 int size,len,data,desp,rank,i;
 char *buf;
 MPI_Status status; 
 MPI_Request request;
 MPI_Type_size(sendtype,&len);
 MPI_Comm_rank(comm,&rank);
 data=sendcount*len; 
 
 MPI_Comm_size(comm,&size);
 desp=0;
 if (rank!=root)
   MPI_Isend(sendbuf, data, MPI_CHAR,root,0,comm,&request); 

 else
  { 
    desp=0;
    for (i=0;i<size;i++)
    {
      if (i==root)
       memcpy(recvbuf+desp,sendbuf,data);
      else
       {
       MPI_Irecv(recvbuf+desp,data,MPI_CHAR,i,0,comm,&request);
       MPI_Wait(&request,&status);
       }
      desp=desp+data;
    }

  }
  

return 0;
}

int MPI_Finalize(){
   if (PRINT==1)
   printf("Num_messages is %d\n",Num_messages);
   int finalize_result = PMPI_Finalize();
   free_process_nodes();
   return finalize_result;
}
