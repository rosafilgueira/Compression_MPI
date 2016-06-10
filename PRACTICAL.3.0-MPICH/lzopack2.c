/*************************************************************************
* Copyright © the University of Edinburgh 
* Name:        externo.c
* Author:      Rosa Filgueira
* Description: Practical-MPI Library.
* Year: 2011.
*************************************************************************/
#include "lzo-2.05/include/lzo/lzoconf.h"
#include "lzo-2.05/include/lzo/lzo1x.h"
#include <stdio.h>
#include <string.h>
/* portability layer */
#define WANT_LZO_MALLOC 1
#define WANT_LZO_FREAD 1
#define WANT_LZO_WILDARGV 1
static int init =0;
#include "lzo-2.05/examples/portab.h"
/* magic file header for lzo
pack-compressed files */
static const unsigned char magic[7] =
    { 0x00, 0xe9, 0x4c, 0x5a, 0x4f, 0xff, 0x1a };
/*************************************************************************
// file IO
**************************************************************************/
int do_compress2(char *in,char*out, lzo_uint in_len)
{
    lzo_uint r = 0;
    lzo_bytep wrkmem = NULL;
    lzo_uint32 wrk_len = 0;
    lzo_uint32 flags = 1;       /* do compute a checksum */
    int method = 1;             /* compression method: LZO1X */
    lzo_uint32 checksum;
    long total=0;
    lzo_uint aux,aux2; 
    lzo_uint out_len;
    int block;
    int tam_cab;
/***************************************/
/*
 *  * Step 1: write magic header, flags & block size, init checksum
 *   */ if (init==0)
      { 
         lzo_init();
         init=1;
      }
    /*************decision de comprimir***********************/
    checksum = lzo_adler32(0,NULL,0);
   // block=in_len;
   // (*out)= (char*) malloc((block +10000)*sizeof(int));
/*
 *  * Step 2: allocate compression buffers and work-memory
 *   */
    wrk_len = LZO1X_1_MEM_COMPRESS;
   //TARDA MUCHO-->  wrk_len = LZO1X_999_MEM_COMPRESS;
    wrkmem = (lzo_bytep) lzo_malloc(wrk_len);
    if (in == NULL || (out) == NULL || wrkmem == NULL)
    {
        r = 1;
    }
/*
 *  * Step 3: process blocks
 *   */
//        checksum = lzo_adler32(checksum,in,in_len);
//        lzo_memset(wrkmem,0xff,wrk_len);
        r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
        // TARDA MUCHO-->r = lzo1x_999_compress(in,in_len,out,&out_len,wrkmem);
        if (r != LZO_E_OK || out_len > in_len + in_len / 16 + 64 + 3)
        {
            /* this should NEVER happen */
            printf("internal error - compression failed: %d\n", r);
        }
       lzo_free(wrkmem);
       return out_len;
}
int do_decompress2(lzo_bytep in, char *out,int in_len,int block_size)
{
    int r = 0;
    lzo_bytep buf = NULL;
    lzo_uint buf_len;
    unsigned char m [ sizeof(magic) ];
    lzo_uint32 flags;
    int method;
    lzo_uint32 checksum;
    int out_len;
    lzo_uint new_len ;
   
/* 
 * Step 0: check params
 */
/*
 * Step 1: check magic header, read flags & block size, init checksum
 */ if (init==0)
      { 
         lzo_init();
        init=1;
      }
     //checksum = lzo_adler32(0,NULL,0);

/*
 * Step 2: allocate buffer for in-place decompression
 */
       
          new_len = block_size;
          r = lzo1x_decompress_safe(in,in_len,out,&new_len,NULL);
             if (  r != 0 )
                    printf("Errorrrr: NO descomprime bien !!!!!!!!!!!!!!!! longitud comprimida %d original%d\n",in_len,block_size) ;
          return new_len;  
}
