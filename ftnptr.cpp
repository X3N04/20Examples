/*
** ftnptr.cpp
** Keith Mehl  03/23/2010
** CSCI-20 Example program
** Illustrate using pointers to functions
*/
#include <iostream>
using namespace std;

/* first, several functions to handle the menu options... */
void handle1( int n )
{
   cout << "Handling input of " << n << endl;
   return;
}
void handle2( int n )
{
   cout << "This handles " << n << " as input" << endl;
   return;
}
void handle3( int n )
{
   cout << "And in here, we take care of " << n << endl;
   return;
}
void handled( int n )
{
   cout << "Here we handle the default case (other input " << n << ")\n" << flush;
   return;
}
int main( void )
{
       // typedef a pointer to function that takes int and returns void
   typedef void (*PF)( int );
       // an array of pointers to functions
   PF pf[4] = { handle1, handle2, handle3, handled };
   int inval, i = 0;
   while( i != 3 ) {
      cout << "Menu option 1\nMenu option 2\nMenu option 3\n"
           << "Exit        4\nEnter option : "
           << flush;
      cin  >> inval;
      i = inval < 1 || inval > 3 ? 3 : inval - 1;
        // dereference the pointer to the function, pass in argument (user input)
      pf[i]( inval );
   }
   return 0;
}


