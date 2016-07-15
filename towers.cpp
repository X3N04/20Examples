//
// towers.cpp
//
// Keith Mehl 11/22/04
// CSCI-20 example program
// Illustrate use of recursion to solve the "Towers of Hanoi"
//
#include <iostream>
using namespace std;

// move n disks from peg "from" to peg "to" using spare peg "spare"
void Tower( int n, char from, char to, char spare )
{
   if( n > 0 )
   {
      Tower( n-1, from, spare, to );
      cout << "Move disk " << n << " from " << from << " to " << to << endl;
      Tower( n-1, spare, to, from );      
   }
   return;
}

int main()
{
   int numDisks;
   cout << "Enter number of disks : " << flush;
   cin  >> numDisks;
   Tower( numDisks, 'A', 'B', 'C' );
   return 0;
}

#if 0 // output for 5 disks
Enter number of disks : 5
Move disk 1 from A to B
Move disk 2 from A to C
Move disk 1 from B to C
Move disk 3 from A to B
Move disk 1 from C to A
Move disk 2 from C to B
Move disk 1 from A to B
Move disk 4 from A to C
Move disk 1 from B to C
Move disk 2 from B to A
Move disk 1 from C to A
Move disk 3 from B to C
Move disk 1 from A to B
Move disk 2 from A to C
Move disk 1 from B to C
Move disk 5 from A to B
Move disk 1 from C to A
Move disk 2 from C to B
Move disk 1 from A to B
Move disk 3 from C to A
Move disk 1 from B to C
Move disk 2 from B to A
Move disk 1 from C to A
Move disk 4 from C to B
Move disk 1 from A to B
Move disk 2 from A to C
Move disk 1 from B to C
Move disk 3 from A to B
Move disk 1 from C to A
Move disk 2 from C to B
Move disk 1 from A to B
#endif


