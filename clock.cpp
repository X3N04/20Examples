//
// clock.cpp
//
// Keith Mehl 10/23/03
// CSCI-20 example program
// Illustrate use of struct _timeb and _ftime() to measure elapsed
// time between two events (entry of 2 characters from keyboard).
//

#include <iostream>
using namespace std;
#include <sys/types.h>  // WARNING!  These are WINDOWS headers,
#include <sys/timeb.h>  // NOT ANSI or ISO/ANSI headers!
/*
// These are C headers and a C struct declaration, not C++ declarations.
// The struct declaration uses the struct keyword in the timing code
These headers define the following struct and allow access to _ftime()
struct _timeb {
   time_t time;            // unsigned long seconds since 00:00:00 1/1/70
   unsigned short millitm; // milliseconds in current second
   short timezone;         // don't worry about these two fields for this
   short dstflag;
};
void _ftime(struct _timeb *); not exactly the prototype but close enough
*/

typedef unsigned short USHORT;  // set up short aliases for the unsigned types
typedef unsigned int   UINT;
typedef unsigned long  ULONG;

int main()
{
   struct _timeb t1, t2;   // the C way of declaring these structs
   char ch;                // for user input (any non-whitespace character)
   USHORT ms;
   ULONG  s = 0;           // for seconds/ms calculations

   cout << "Enter a character : " << flush;
   cin  >> ch;
   _ftime( &t1 );

   cout << "Enter another character : " << flush;
   cin  >> ch;
   _ftime( &t2 );

   if( t1.time != t2.time ) // not within the same second?
   {
      s = t2.time - t1.time;
      ms = ( ((USHORT)s) * 1000 ) + (t2.millitm - t1.millitm);
   }
   else                     // otherwise within same second
   {
      ms = t2.millitm - t1.millitm;
   }
   cout << "Time in ms is " << ms << ".\n" << flush;
   return 0;
}

