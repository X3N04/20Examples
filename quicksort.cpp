//
// quicksort.cpp
// Keith Mehl 10/28/06
// CSCI-20 example program
// Illustrate Windows millisecond timer for a sorting algorithm
//

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "drand.h"
using namespace std;

#if 0
#includes below are to get access to the GetLocalTime(), a
Windows system call (NOT STANDARD C++ -- it will not work elsewhere!)
which takes a pointer to a _SYSTEMTIME system time struct, as shown here:
For info, go to:
http://msdn.microsoft.com/en-us/library/ms724338%28VS.85%29.aspx
typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;
#endif

#include <Windows.h>
#include <Winbase.h>

#define MAX 100000
#define DEBUG

const int MinPerHr = 60;
const int SecPerMin = 60;
const int MSPerS = 1000;

// one way to write a QuickSort algorithm
// create a helper for the user's quickSort function
// parameters : array to sort, left index, right index
// indices are to the section being sorted on this call (recursive)
void q_sort(long a[], int left, int right)
{
   long pivot;  // pivot value from array (makes a hole to put moved values)
   int l_hold, r_hold, pivot_pos;
   l_hold = left;  // save left and right indices for later use
   r_hold = right;
   // select first value as pivot, and save it off out of the array
   pivot = a[left];
   while (left < right)    // until our indices cross
   {
      // scan from right until we find something that goes on 
      // the left side or we cross the left's boundary
      while( (left < right) && (a[right] >= pivot) )
      {
         right--;
      }
      // if we found a value that goes on the left, put it there
      if( left != right )
      {
         a[left] = a[right];
         left++;
      }
      // now, similar scan from left
      while( (left < right) && (a[left] <= pivot) )
      {
         left++;
      }
      // again, if we found a value on the wrong side, move it
      if (left != right)
      {
         a[right] = a[left];
         right--;
      }
   }
   // now replace the pivot where it belongs
   a[left] = pivot;   // pivot holds the value here
   pivot_pos = left;  // pivot_pos holds the index to the pivot value
   left = l_hold;     // reset left and right to saved values
   right = r_hold;
   // if we still have two or more elements in the sections,
   // recurse with each section
   // Note that the pivot is where it belongs already, so
   // don't include it in the range being sorted
   if (left < pivot_pos)
   {
      q_sort(a, left, pivot_pos-1);
   }
   if (right > pivot_pos)
   {
      q_sort(a, pivot_pos+1, right);
   }
   return;
}

// user interface - quicksort n elements of array a
void quickSort(long a[], int n)
{
   q_sort(a, 0, n - 1);
}

int main( int argc, char **argv )
{
   static long a[MAX], param;
   unsigned long seed = 0x12345678L;
   int i, lineCt = 0;
   
   // for millisecond timing with GetLocalTime()
   _SYSTEMTIME preTime, postTime;
   long timeDiff = 0L;
   
   if( argc > 1 )
   {
      param = atol( argv[1] );
      if( param > 0 )
      {
         initrand( (unsigned long) param );
      }
      else if( param < 0 )
      {
         srand( (unsigned long)time( NULL ) );
      }
      else // param == 0
      {
         initrand( seed );
      }
   }
   else // this random number generator MUST be initialized
   {
      initrand( seed );
   }
   for( i = 0; i < MAX; i++ )
   {
      a[i] = lrand();
   }
#ifdef DEBUG  // remove this to print the arrays pre-sort and post-sort
#undef DEBUG  // comment this out to print (LOTS OF!) debug output
#endif

#ifdef DEBUG
   cout << "Max = " << MAX << ".\n" << endl;
   for( i = 0; i < MAX; i++ )
   {
      cout << right << setw( 12 ) << a[i] << flush;
      if( ++lineCt == 6 )
      {
         lineCt = 0;
         cout << endl;
      }
   }
   if( lineCt > 0 )
   {
      cout << endl;
   }
   lineCt = 0;
   cout << endl << "------------------------------------\n" << endl;
#endif

   GetLocalTime( &preTime );
   quickSort( a, MAX );
   GetLocalTime( &postTime );

#ifdef DEBUG   
   for( i = 0; i < MAX; i++ )
   {
      cout << right << setw( 12 ) << a[i] << flush;
      if( ++lineCt == 6 )
      {
         lineCt = 0;
         cout << endl;
      }
   }
   cout << endl;
#endif

   timeDiff = ( postTime.wHour * MinPerHr * SecPerMin * MSPerS +
                postTime.wMinute * SecPerMin * MSPerS +
                postTime.wSecond * MSPerS +
                postTime.wMilliseconds )
              -
              ( preTime.wHour * MinPerHr * SecPerMin * MSPerS +
                preTime.wMinute * SecPerMin * MSPerS +
                preTime.wSecond * MSPerS +
                preTime.wMilliseconds );
                 
   cout << "time to execute QuickSort : " << timeDiff 
        << " milliseconds." << endl;
   return 0;   
}

