/*************************************************************************
*/
/* Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Basic Compression Library */
#include "rle.h"
#include "shannonfano.h"
#include "huffman.h"
#include "rice.h"
#include "lz.h"
#include "snappy.h"

/* Timing */
#include "systimer.h"
#define TYPE int
#define TIEMPOS 0
/*************************************************************************
* TestFile()
*************************************************************************/

int estudio_cabecera(char *in)
{
 char *ptr1, *ptr2;
 int r;

 ptr1=(char *)in+4;
 ptr2=(char *)in+5;
  if ((*ptr1==1)&&(*ptr2==2))
   {
    r=1;
   }
  else
      if ((*ptr1==3)&&(*ptr2==4))
      {
         r=1;
       }
        else
          r=0;
 return r;
}
int anadir_cabecera(char *in,char *out,int len)
{
 int tam_cab;
 char *ptr1, *ptr2,*ptr3;
 int *ptr; 
// printf("-->ENTRO A ANADIR CABECERA\n");
  tam_cab=3*sizeof(char)+sizeof(int);
  ptr=(int*)(out)+0;
  *ptr=len;
  ptr1=(char *)(out)+4;
  *ptr1=3;
   ptr2=(char *)(out)+5;
   *ptr2=4;
   ptr2=(char *)(out)+6;
   *ptr2=0;
   memcpy((char*)(out)+tam_cab,in,len);
   len=len +(3*sizeof(char))+sizeof(int);
    return len;
}

int compresion(int algo, char *in, char *buf, int insize, int rank, int tipo)
{ int tam_cab,tam_total,out_size,*ptr_int,kk;
  char* ptr1;
  tam_total= (3*sizeof(char) + insize);
  tam_cab=(3*sizeof(char))+sizeof(int); 
  out_size=compresion_xxx(algo,in,buf+tam_cab,insize,rank);

 kk=out_size+tam_cab;
  
  if (kk < insize)
    {   ptr_int=(int*)(buf)+0;
       if (tipo==1)
         *ptr_int = out_size+(3*sizeof(char))+sizeof(int);  
       else
         *ptr_int =insize;
        ptr1=(char*)(buf)+4;
        *ptr1=1;
        ptr1=(char*)(buf)+5;
        *ptr1=2;
        ptr1=(char*)(buf)+6;
        *ptr1=(char)algo;
        out_size=out_size+(3*sizeof(char))+sizeof(int);

    }
  else
   {
       ptr_int=(int*)(buf)+0;
       if (tipo==1)
         *ptr_int = out_size+(3*sizeof(char))+sizeof(int) ;  
       else
         *ptr_int =insize; 
       ptr1=(char*)(buf)+4;
       *ptr1=3;
       ptr1=(char*)(buf)+5;
       *ptr1=4;
       ptr1=(char*)(buf)+6;
       *ptr1=0;
       memcpy(buf+tam_cab,in,insize);
       out_size=insize+(3*sizeof(char))+sizeof(int);
   }
  
     return out_size;  
}

int descompresion( char *buf, char *out, int size, int tipo)
{
    char * ptr1, *ptr2,*ptr3;
    int tam_cab,outsize,insize,*ptr;
    int algo;
    tam_cab= (3*sizeof(char) + sizeof(int));
    ptr= (int *)buf+0;
    if (tipo==1)
    {outsize= *ptr; 
    insize=size;
    }
    else
    {insize= *ptr; 
    outsize=size;
    }
    ptr1=(char *)buf+4;
    ptr2=(char *)buf+5;
    ptr3=(char *)buf+6;
       if ((*ptr1==1) &&(*ptr2==2))
         { outsize= outsize-tam_cab;
             algo=(int)*ptr3;
             descompresion_xxx(algo, buf+tam_cab, out, outsize,insize);
         }
       else
          {
           memcpy(out,buf+tam_cab,insize);
          } 
 return insize;

}
int compresion_xxx(  int algo, char *in, char *buf, int insize, int rank)
{
  int outsize;
  unsigned int *work,i;
  double         t0,  t_comp;
  FILE * pf1, *pf2,*pf3,*pf4;
  char cadena1[1000],cadena2[1000],cadena3[1000],nombre_fichero1[1000],nombre_fichero2[1000],nombre_fichero3[1000];
  unsigned long long size;
  switch( algo )
  {
    case 1:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = RLE_Compress( in, buf, insize );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 2:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = Huffman_Compress( in, buf, insize );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 3:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = Rice_Compress( in, buf, insize, RICE_FMT_UINT8 );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 4:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = Rice_Compress( in, buf, insize, RICE_FMT_UINT16 );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 5:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = Rice_Compress( in, buf, insize, RICE_FMT_UINT32 );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 6:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = Rice_Compress( in, buf, insize, RICE_FMT_INT8 );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 7:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = Rice_Compress( in, buf, insize, RICE_FMT_INT16 );
       t_comp = GetTime() -   t0;
      break;
    case 8:
       if(TIEMPOS==1) t0 = GetTime();
      outsize = Rice_Compress( in, buf, insize, RICE_FMT_INT32 );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 9:
       if(TIEMPOS==1) t0 = GetTime();
     outsize = LZ_Compress( in, buf, insize );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 10:
      work = malloc( sizeof(unsigned int) * (65536+insize) );
      if( work )
      {
         if(TIEMPOS==1) t0 = GetTime();
        outsize = LZ_CompressFast( in, buf, insize, work );
        if(TIEMPOS==1) t_comp = GetTime() -   t0;
        free( work );
      }
      else
      {
        printf( "unable to allocate working buffer!\n" );
        outsize = 0;
      }
      break;
    case 11:
         if(TIEMPOS==1) t0 = GetTime();
      outsize = SF_Compress( in, buf, insize );
         if(TIEMPOS==1) t_comp = GetTime() -   t0;
      break;
    case 12: 
       if(TIEMPOS==1) t0 = GetTime();
     outsize = do_compress2( in, buf, insize );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
        break;
    case 13:
       if(TIEMPOS==1) t0 = GetTime();
       
       int err;
        //     size_t outlen;
  
       	struct snappy_env env;
	snappy_init_env(&env);
	err = snappy_compress(&env, in, insize, buf, &outsize);
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
        break;
  case 14:
       if(TIEMPOS==1) t0 = GetTime();
     outsize = LZ4_compress( in, buf, insize );
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
       break;
   /* case 13:
        strcpy(cadena1,"/tmp/Datos_ev");
        sprintf(nombre_fichero1,"%s",cadena1);
        pf1=fopen(nombre_fichero1,"a");
        fwrite(&insize,1,sizeof(int),pf1); 
        fwrite(in,1,insize,pf1); 
        fclose(pf1);
        if(TIEMPOS==1) t0 = GetTime();
        fpc_compress(20,(unsigned long long *)in,(long long)insize,(unsigned long long*)(buf),&size);
        outsize=(int)size;
       if(TIEMPOS==1) t_comp = GetTime() -   t0;
        //printf("Los valores devueltos al comprimir son orig %d y compr%d\n",insize,outsize); 
     break;*/
    default:
      /* Should never happen... */
      outsize = 0;
  }

  if(outsize > 0)

       {
       /* strcpy(cadena1,"/datos/arcos/Datos_ev");
        sprintf(nombre_fichero1,"%s-%d",cadena1,rank);
        pf1=fopen(nombre_fichero1,"a");
        fwrite(&insize,sizeof(int),1,pf1); 
        fwrite(in,insize,1,pf1); 
        fclose(pf1);*/
       }

    /* Show compression result */
       if (TIEMPOS==1) 
           printf("Compression algorithm %d, input size %d, output size %d, time %.3f ms\n",algo,insize,outsize,1000.0 *t_comp);
       // strcpy(cadena1,"/tmp/Compresion_ev");
       // sprintf(nombre_fichero1,"%s",cadena1);
       // pf1=fopen(nombre_fichero1,"a");
       // fprintf(pf1, "\n%.1f \t",(float)((insize-outsize)*100)/(float)(insize)); 
       // strcpy(cadena2,"/tmp/Tiempo_ev");
       // sprintf(nombre_fichero2,"%s",cadena2);
       //pf2=fopen(nombre_fichero2,"a");
       // fprintf(pf2, "\n%.2f \t",(float)(1024)*1000.0*t_comp/(float)(insize)); 
       // fclose(pf1);
       // fclose(pf2);
       // strcpy(cadena3,"/tmp/Size_ev");
        //sprintf(nombre_fichero3,"%s",cadena3);
       // pf3=fopen(nombre_fichero3,"a");
       // fprintf(pf3, "\n%.1f \t",(float)insize/(float)(1024)); 
       // fclose(pf3);
       // pf4=fopen("/tmp/Datos","w");
       // for(i=0;i<insize;i++)
       //   fprintf(pf4,"%c",in[i]);
       // fclose(pf4);   

  return outsize;
}
int descompresion_xxx(  int algo, char *buf, char *out, int outsize, int insize)
  {unsigned int *work;
   double  t0,  t_uncomp;
  FILE * pf1, *pf2;
  char cadena1[1000],cadena2[1000],nombre_fichero1[1000],nombre_fichero2[1000];
  long long osize, isize;
  
  switch( algo )
  {
    case 1:
       if(TIEMPOS==1) t0 = GetTime();
      RLE_Uncompress( buf, out, outsize );
       if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 2:
      if(TIEMPOS==1) t0 = GetTime();
      Huffman_Uncompress( buf, out, outsize, insize );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 3:
      if(TIEMPOS==1) t0 = GetTime();
      Rice_Uncompress( buf, out, outsize, insize, RICE_FMT_UINT8 );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 4:
      if(TIEMPOS==1) t0 = GetTime();
      Rice_Uncompress( buf, out, outsize, insize, RICE_FMT_UINT16 );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 5:
      if(TIEMPOS==1) t0 = GetTime();
      Rice_Uncompress( buf, out, outsize, insize, RICE_FMT_UINT32 );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 6:
      if(TIEMPOS==1) t0 = GetTime();
      Rice_Uncompress( buf, out, outsize, insize, RICE_FMT_INT8 );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 7:
      if(TIEMPOS==1) t0 = GetTime();
      Rice_Uncompress( buf, out, outsize, insize, RICE_FMT_INT16 );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 8:
      if(TIEMPOS==1) t0 = GetTime();
      Rice_Uncompress( buf, out, outsize, insize, RICE_FMT_INT32 );
      if(TIEMPOS==1) t_uncomp = GetTime() -  t0;
      break;
    case 9:
      if(TIEMPOS==1) t0 = GetTime();
      LZ_Uncompress( buf, out, outsize );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 10:
      work = malloc( sizeof(unsigned int) * (65536+insize) );
      if( work )
      {
      if(TIEMPOS==1) t0 = GetTime();
        LZ_Uncompress( buf, out, outsize );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      }
      else
      {
        printf( "unable to allocate working buffer!\n" );
      }
      break;
    case 11:
      if(TIEMPOS==1) t0 = GetTime();
      SF_Uncompress( buf, out, outsize, insize );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 12:
      if(TIEMPOS==1) t0 = GetTime();
      do_decompress2( buf, out, outsize,insize );
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    case 13:
      if(TIEMPOS==1) t0 = GetTime();
      int err = snappy_uncompress(buf, outsize, out);
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
  case 14:
      if(TIEMPOS==1) t0 = GetTime();
      LZ4_decompress_fast(buf, out, insize);
      if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
      break;
    


 
    /* case 13:
     if(TIEMPOS==1) t0 = GetTime();
     osize=(long long)outsize;
     isize=(long long)insize;
     fpc_decompress((unsigned long long * )(buf),osize,(unsigned long long*)(out),isize);    
     if(TIEMPOS==1) t_uncomp = GetTime() -   t0;
     //printf("FPC!!!! isize es %ld y osize es %ld -- insize es %d y outsize %d\n",isize,osize,insize,outsize); 
    break;*/
     default:
      /* Should never happen... */
      outsize = 0;
  }

//  if(insize > 0)
//    /* Show compression result */
     if(TIEMPOS==1)    
	printf( "Decompression algorithm %d, compressed size %d, uncompressed size %d, time %.3f ms\n",algo,outsize,insize,1000.0 *   t_uncomp );
     //     if (TIEMPOS==1)    
     //   printf("(tiempo por 1kb en funcion compressed-size:%.2f) ( Tiempo por 1kb en funcion descompressed-size:%.2f) \n",(float)(1024)*1000.0*t_uncomp/(float)(outsize),(float)(1024)*1000.0*t_uncomp/(float)(insize));
      //  strcpy(cadena1,"/tmp/Tiempo_dev");
      //  sprintf(nombre_fichero1,"%s",cadena1);
      //  pf1=fopen(nombre_fichero1,"a");
      //  fprintf(pf1, "\n%.2f \t",(float)(1024)*1000.0*t_uncomp/(float)(insize)); 
    //fclose(pf1);


  /* Free all memory */

}




/*************************************************************************
* Help()
*************************************************************************/

void Help( char *prgname )
{
  printf( "Usage: %s algo file\n\n", prgname );
  printf( "algo can be one of the following:\n" );
  printf( "  rle     RLE Compression\n" );
  printf( "  sf      Shannon-Fano compression\n" );
  printf( "  huff    Huffman compression\n" );
  printf( "  lz      LZ77 Compression\n" );
  printf( "  lz_f    LZ77 Compression - fast method\n" );
  printf( "  rice8   Rice compresison of 8-bit data\n" );
  printf( "  rice16  Rice compresison of 16-bit data\n" );
  printf( "  rice32  Rice compresison of 32-bit data\n" );
  printf( "  rice8s  Rice compresison of 8-bit signed data\n" );
  printf( "  rice16s Rice compresison of 16-bit signed data\n" );
  printf( "  rice32s Rice compresison of 32-bit signed data\n" );
  printf( "  lzo     LZO compresison of data\n" );
  printf( "  lz4     LZ4 compresison of data\n" );
  printf( "  snappy  Snappy compresison of data\n" );
  printf( "file is the name of a file to be tested.\n" );
}


/*************************************************************************
*************************************************************************/

