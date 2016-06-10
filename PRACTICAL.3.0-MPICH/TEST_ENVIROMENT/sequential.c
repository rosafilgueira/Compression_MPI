/*************************************************************************
* Copyright © the University of Edinburgh 
* Name:        externo.c
* Author:      Rosa Filgueira
* Description: Practical-MPI Library.
* Year: 2011.
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
int buff[1000];
char *buff_compressed, *buff_decompressed;
int i, compressed_size, original_size,algo;

if( argc < 2 )
  {
    printf("argc is %d, Please, algorithm name \n",argc);
    Help(argv[0]);
     return 0;
  }

  algo = 0;
  if( strcmp( argv[1], "rle" ) == 0 )     algo = 1;
  if( strcmp( argv[1], "huff" ) == 0 )    algo = 2;
  if( strcmp( argv[1], "rice8" ) == 0 )   algo = 3;
  if( strcmp( argv[1], "rice16" ) == 0 )  algo = 4;
  if( strcmp( argv[1], "rice32" ) == 0 )  algo = 5;
  if( strcmp( argv[1], "rice8s" ) == 0 )  algo = 6;
  if( strcmp( argv[1], "rice16s" ) == 0 ) algo = 7;
  if( strcmp( argv[1], "rice32s" ) == 0 ) algo = 8;
  if( strcmp( argv[1], "sf" ) == 0 )      algo = 11;
  if( strcmp( argv[1], "lzo" ) == 0 )     algo = 12;
  if( !algo )
  {
   printf("Sorry, we don't have this compression algorithm\n");
   return;
  }

 for(i=0;i<1000;i++) 
    buff[i]=10;

 original_size=1000*sizeof(int);
 buff_compressed=(char*) malloc((original_size+100)*sizeof(char));
 compressed_size=compresion(algo,buff,buff_compressed,original_size,0,0);
 // We can save in the head the original size of the buffer (type=0), or the compressed size (type=1)
 //compressed_size=compresion(algo,buff,buff_compressed,original_size,0,1);
 printf("Compression_algorithm:%d, Original size:%d, Compressed size %d\n",algo,original_size, compressed_size); 

  buff_decompressed=(char*) malloc((original_size+1000)*sizeof(int));
  descompresion(buff_compressed,buff_decompressed,compressed_size,0);
  //If we compressed with type=1, we should decompressed with type=1 and use original_size.
  //descompression(buff_compressed,buff_decompressed,original_size,1);

  printf ("Check, position 99 of initial buffer=%d, position 99 of decompressed buffer= %d\n",buff[99],((int *)buff_decompressed)[99]);

}
