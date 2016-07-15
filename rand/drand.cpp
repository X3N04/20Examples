//
// drand.cpp
// Keith Mehl 05/03/05
// Illustrate random number generators for long or floating point numbers
// Adapted from http://www.pell.portland.or.us/~orc/Code/Misc/random.c
//

/************************************************************************
 This random number generator originally appeared in "Toward a Universal
 Random Number Generator" by George Marsaglia and Arif Zaman.
 Florida State University Report: FSU-SCRI-87-50 (1987)

 It was later modified by F. James and published in "A Review of Pseudo-
 random Number Generators"

 Converted from FORTRAN to C by Phil Linttell, James F. Hickling
 Management Consultants Ltd, Aug. 14, 1989.

 THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.
       (However, a newly discovered technique can yield
         a period of 10^600. But that is still in the development stage.)

 It passes ALL of the tests for random number generators and has a period
   of 2^144, is completely portable (gives bit identical results on all
   machines with at least 24-bit mantissas in the floating point
   representation).

 The algorithm is a combination of a Fibonacci sequence (with lags of 97
   and 33, and operation "subtraction plus one, modulo one") and an
   "arithmetic sequence" (using subtraction).

 On a Vax 11/780, this random number generator can produce a number in
    13 microseconds.
************************************************************************/

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "drand.h"

#define TRUE    1
#define FALSE   0

// static here means 'not accessable from other files in the project'
static float u[97], c, cd, cm;
static int i97, j97, initialized = 0;

#if 0 // now #included in drand.h
void initrand(unsigned long seed);
long lrand();
double drand();
#endif

/************************************************************************
 This is the initialization routine for the random number generator RANMAR()
 NOTE: The seed variables can have values between:    0 <= IJ <= 31328
                                                      0 <= KL <= 30081
 The random number sequences created by these two seeds are of sufficient
 length to complete an entire calculation with. For example, if several
 different groups are working on different parts of the same calculation,
 each group could be assigned its own IJ seed. This would leave each group
 with 30000 choices for the second seed. That is to say, this random
 number generator can create 900 million different subsequences -- with
 each subsequence having a length of approximately 10^30.

 Use IJ = 1802 & KL = 9373 to test the random number generator. The
 subroutine RANMAR should be used to generate 20000 random numbers.
 Then display the next six random numbers generated multiplied by 4096*4096
 If the random number generator is working properly, the random numbers
 should be:
           6533892.0  14220222.0   7275067.0
           6172232.0   8354498.0  10633180.0
************************************************************************/

void initrand(unsigned long seed)
{
   float s, t;
   int i, j, k, l, m;
   int ii, jj;
	long ij, kl;

   /* Change FALSE to TRUE in the next statement to test the random routine. */

   initialized = FALSE;

	ij = seed % 31328;
	kl = (seed >> 16) % 30081;

   i = (int)fmod(ij/177.0, 177.0) + 2;
   j = (int)fmod(ij      , 177.0) + 2;
   k = (int)fmod(kl/169.0, 178.0) + 1;
   l = (int)fmod(kl      , 169.0);

   for ( ii=0; ii<=96; ii++ )
   {
      s = (float)0.0;
      t = (float)0.5;
      for ( jj=0; jj<=23; jj++ )
      {
         m = (int)fmod( fmod(i*j,179.0)*k , 179.0 );
         i = j;
         j = k;
         k = m;
         l = (int)fmod( 53.0*l+1.0 , 169.0 );
         if ( fmod(l*m,64.0) >= 32)
            s = s + t;
         t = (float)(0.5 * t);
      }
      u[ii] = s;
   }

   c  = (float)(  362436.0 / 16777216.0);
   cd = (float)( 7654321.0 / 16777216.0);
   cm = (float)(16777213.0 / 16777216.0);

   i97 = 96;
   j97 = 32;

   initialized = TRUE;
}

// static here means not accessable from another file
// e.g., something like a private function - not an entry
static int ranmar(float rvec[], int len)
{
   float uni;
   int ivec;

   if ( !initialized )
      return 1;

   for ( ivec=0; ivec < len; ivec++)
   {
      uni = u[i97] - u[j97];
      if ( uni < 0.0F )
         uni = uni + 1.0f;
      u[i97] = uni;
      i97--;
      if ( i97 < 0 )
         i97 = 96;
      j97--;
      if ( j97 < 0 )
         j97 = 96;
      c = c - cd;
      if ( c < 0.0F )
         c = c + cm;
      uni = uni - c;
      if ( uni < 0.0F )
         uni = uni + 1.0f;
      rvec[ivec] = uni;
   }
   return 0;
}


double drand()
{
	float v[1];
	ranmar(v,1);
	return (double)(v[0]);
}


long lrand()
{
	return (long)(LONG_MAX * drand());
}

#if 0 // #if 1 to compile as a standalone program, not a library
int main()
{
   int i;
   double d = 0.0;

   // this random number generator MUST be seeded before use!
   initrand(0x12345678L);  // seed the random number generator
   
   for( i = 0; i < 20000; i++ )
   {
      d += drand();
   }
   for( i = 0; i < 10; i++)
   {
      printf( "%d:\t%22.20lf\n", i, drand() ); // 4096.0 * 4096.0 * drand()
   }
   for( i = 0; i < 10; i++ )
   {
      printf( "%d:\t%014ul\n", i, lrand() );
   }
   return 0;
}
#endif

