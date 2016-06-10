#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TRUE 1
#define FALSE 0

enum Type {
	Char,
	Int,
	Float,
	Double
};

double rand_double()
{
      double r = (double)rand();
      return r;
}


float rand_float()
{
      float r = (float)rand();
      return r;
}

// inclusive
int rand_range(int min, int max)
{
	int delta = max - min;
	return min + rand_lim(delta);
}

int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

// be vary size is in bytes not length!
void generate_char(char *buf, int size, int redundancy)
{
	int count = size / (sizeof(char));
	int i = 0;

	for ( ; i < count * redundancy / 100; i++)
		buf[i] = 0;

	for ( ; i < count; ++i)
		buf[i] = rand_range(1, 255);
}

// be vary size is in bytes not length!
void generate_int(int *buf, int size, int redundancy)
{
	int count = size / (sizeof(int));
	int i = 0;

	for ( ; i < count * redundancy / 100; i++)
		buf[i] = 0;

	for ( ; i < count; ++i)
		buf[i] = rand_range(1, 999999999);
}

// be vary size is in bytes not length!
void generate_float(float *buf, int size, int redundancy)
{
	int count = size / (sizeof(float));
	int i = 0;

	for ( ; i < count * redundancy / 100; i++)
		buf[i] = 0.0;

	float randomf = 0.0;	
	for ( ; i < count; ++i)
	{
		randomf = rand_float();

		// just to ensure there will be no more zeros, because this can affect the redundancy
		while (randomf == 0.0)
			randomf = rand_float();

		buf[i] = randomf;
	}
}

// be vary size is in bytes not length!
void generate_double(double *buf, int size, int redundancy)
{
	int count = size / (sizeof(double));
	int i = 0;

	for ( ; i < count * redundancy / 100; i++)
		buf[i] = 0.0;

	double randomd = 0.0;	
	for ( ; i < count; ++i)
	{
		randomd = rand_double();

		// just to ensure there will be no more zeros, because this can affect the redundancy
		while (randomd == 0.0)
			randomd = rand_double();

		buf[i] = randomd;
	}
}

// checks if two buffers equal
int equals(void *in, void *out, int size)
{
  char *in_char = (char *) in;
  char *out_char = (char *) out;

  int i = 0;
  for ( ; i < size; ++i)
  {
    if (in_char[i] != out_char[i])
    {
      printf("in_char[%d] = %d, out_char[%d] = %d", i, in_char[i], i, out_char[i]);
      return FALSE;
    }
  }

  return TRUE;

}

void check_decompression_result(void *in, void *out, int size)
{
  if (equals(in, out, size)) 
  {
    printf("Decompression Successful\n");
  }
  else
  {
    printf("Decompression Unsuccessful!!!\n");
  }
}

void dumbfunc()
{
}


void perform_tests(int type)
{
  int sizesCount = 5;
  int sizes[] = 
	{ 102400, // 100KB
		512000, // 500KB
		921600, // 900KB
		1048576,  // 1MB
		2097152  //  2MB
	};

	int redundanciesCount = 5;
	int redundancies[] =
	{
		0,
		25,
		50,
		75,
		100
	};

  // this is just to initialize randomizer
	srand(time(NULL));
	
	int worstCase = 10000;
	int sizeIndex = 0;
	for ( ; sizeIndex < sizesCount; ++sizeIndex)
	{
		int redundancyIndex = 0;
		for ( ; redundancyIndex < redundanciesCount; ++redundancyIndex)
		{
		  
			int size = sizes[sizeIndex];
			int redundancy = redundancies[redundancyIndex];

			printf("Type: %d\nSize: %d\nRedundancy: %d\n", type, size, redundancy);
	

			int algorithm = 1;
			int out_size;
			for ( ; algorithm < 15; ++algorithm)
			{
			   if (algorithm == 9 || algorithm == 10) continue;
			   switch (type)
			    {
			    case Char:
			      dumbfunc();
			      char *buf_char_in = (char *) malloc (size);
			      char *buf_char_out = (char *) malloc (size + worstCase);
			      char *buf_char_decompressed = (char *) malloc (size);
			      generate_char(buf_char_in, size, redundancy);
			      out_size = compresion_xxx( algorithm, buf_char_in, buf_char_out, size, 0); //what's rank for?    
			      descompresion_xxx( algorithm, buf_char_out, buf_char_decompressed, out_size, size);
			      check_decompression_result( buf_char_in, buf_char_decompressed, size);
			      free(buf_char_in);
			      free(buf_char_out);
			      free(buf_char_decompressed);
			      break;
			    case Int:
			      dumbfunc();
			      int *buf_int_in = (int *) malloc (size);
			      int *buf_int_out = (int *) malloc (size + worstCase);
			      int *buf_int_decompressed = (int *) malloc (size);
			      generate_int(buf_int_in, size, redundancy);
			      out_size = compresion_xxx( algorithm, buf_int_in, buf_int_out, size, 0);
			      descompresion_xxx( algorithm, buf_int_out, buf_int_decompressed, out_size, size);
			      check_decompression_result( buf_int_in, buf_int_decompressed, size);
			      free(buf_int_in);
			      free(buf_int_out);
			      free(buf_int_decompressed);
			      break;
			    case Float:
			      dumbfunc();
			      float *buf_float_in = (float *) malloc (size);
			      float *buf_float_out = (float *) malloc (size + worstCase);
			      float *buf_float_decompressed = (float *) malloc (size);
			      generate_float(buf_float_in, size, redundancy);
			      out_size = compresion_xxx( algorithm, buf_float_in, buf_float_out, size, 0);
			      descompresion_xxx( algorithm, buf_float_out, buf_float_decompressed, out_size, size);
			      check_decompression_result( buf_float_in, buf_float_decompressed, size);
			      free(buf_float_in);
			      free(buf_float_out);
			      free(buf_float_decompressed);
			      break;
			    case Double:
			      dumbfunc();
			      double *buf_double_in = (double *) malloc (size);
			      double *buf_double_out = (double *) malloc (size + worstCase);
			      double *buf_double_decompressed = (double *) malloc (size);
			      generate_double(buf_double_in, size, redundancy);
			      out_size = compresion_xxx( algorithm, buf_double_in, buf_double_out, size, 0);
			      descompresion_xxx( algorithm, buf_double_out, buf_double_decompressed, out_size, size);
			      check_decompression_result( buf_double_in, buf_double_decompressed, size);
			      free(buf_double_in);
			      free(buf_double_out);
			      free(buf_double_decompressed);
			      break;
			    }
			  }
			printf("\n");


		}
	}
}

int main(int argc, char **argv)
{
  perform_tests(Char);
  perform_tests(Int);
  perform_tests(Float);
  perform_tests(Double);
}
