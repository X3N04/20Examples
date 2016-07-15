//
// sudoku.cpp
//
// Keith Mehl
// 10/30/07
// Solve Sudoku puzzles
// represents the grid with a 9x9 array of char
// input file format is digits and spaces, 9 x 9 with trailing .\n'
//
// optional command line arguments:
// argv[0] - input text file name, 
// argv[1] - any character to start debug output
// to get debug output, you MUST put a filename first
//

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// for debugging - print to cout if debug flag is on, per 2nd argument
// the DEBUG macro works (and is programmed) exactly like cout
#define DEBUG if(debug)cout

// Represent a place in the grid where we need to put a number.
// I do this to avoid having to calculate the next position that
// needs to be filled with a digit every time.  Instead I figure
// out the list of open places once, and pass it around as needed.
struct Place
{
   int row, col;
};

// for debugging only - do we need to print debug information?
bool debug = false;

// for debugging only - tracks the recursion depth
int depth = 0;

// return true if value can go into grid[row][col], else false
bool IsLegal( char grid[][9], char value, int row, int col )
{
   // starting indices of each row/col in 3x3 subgrid
   // subgrid indices 0/1/2 start at 0, 3/4/5 at 3, 6/7/8 at 6
   // this moves complexity out of the code into a table.
   static int lim[3] = { 0, 3, 6 };
   int i, j, r, c;
   // if value already in this row, illegal
   for( i = row, j = 0; j < 9; j++ )
   {
      if( grid[i][j] == value )
      {
         return false;
      }
   }
   // if value already in this column, illegal
   for( i = 0, j = col; i < 9; i++ )
   {
      if( grid[i][j] == value )
      {
         return false;
      }
   }
   // if in the 3x3 subgrid around grid[row][col], illegal
   r = row / 3;
   c = col / 3;
   for( i = lim[r]; i <= lim[r] + 2; i++ )
   {
      for( j = lim[c]; j <= lim[c] + 2; j++ )
      {
         if( grid[i][j] == value )
         {
            return false;
         }
      }
   }
   return true;
}

// we're cTries-s far into list of positions with nTries total numbers to place
// and the list of places to put numbers is in the places[] array
bool Solve( char grid[][9], Place places[], int cTries, int nTries )
{
   char ch;  // the current try
   depth++;
   DEBUG << "In Solve, depth " << depth << ", cTries " << cTries << endl;;
   if( cTries == nTries )  // no more places to try?
   {
      return true;
   }
   // for each possible digit at this position
   for( ch = '1'; ch <= '9'; ch++ )
   {
      // if it's legal to put the digit down, try it
      if( IsLegal( grid, ch, places[cTries].row, places[cTries].col ) )
      {
         DEBUG << "Trying " << ch << " at location " 
               << places[cTries].row << ", " << places[cTries].col << endl;
         grid[places[cTries].row][places[cTries].col] = ch;
         // and recursively try to solve with the next open position
         if( Solve( grid, places, cTries+1, nTries ) == true )
         {
            return true;
         }
         // failed, so 'unwind' and loop to try next digit
         grid[places[cTries].row][places[cTries].col] = ' ';
      }
   }
   // out of loop, so this one failed
   DEBUG << "Returning failure from Solve at depth " << depth-- << '.' << endl;
   return false;
}

//
// parse command line
// decide if we need to print debug info
// open input file by default name or argument on command line
// read the grid, building the list of empty places
// solve the grid
// if success from Solve, print result
// else print "No solution" message
//
int main( int argc, char *argv[] )
{
   int i, j;
   char grid[9][9] = { '\0' };
   Place places[100] = { 0, 0, 0 };  // too many possible places
   int ctPlaces = 0; // add one each time we find an empty place
   bool solved = false;
   char filename[100] = "sudoku.txt";
   ifstream fin;

   // user supplied arguments?   
   if( argc > 1 )
   {
      // first arg must be file name
      strcpy( filename, argv[1] );
      // any optional second arg is "do debugging"
      if( argc > 2 )
      {
         debug = true;
      }
   }
   fin.open( filename );
   // did file open fail?
   if( !fin )
   {
      cout << "File error on " << filename << endl; // give up...
      return -1;
   }
   // read the file data into the working grid
   // building the list of empty spots as we go
   for( i = 0; i < 9; i++ )
   {
      for( j = 0; j < 9; j++ )
      {
         fin.get( grid[i][j] );      // get any single character from file
         if( !isdigit(grid[i][j]) )  // empty?
         {
            places[ctPlaces].row = i; // add this position to list of tries
            places[ctPlaces].col = j;
            ctPlaces++;              // and count it
         }
      }
      fin.ignore( 1, '\n' ); // skip the trailing '\n'
   }
   fin.close();
   // dump the original grid, replacing spaces with '_'s
   for( i = 0; i < 9; i++ )
   {
      for( j = 0; j < 9; j++ )
      {
         cout << ( grid[i][j] == ' ' ? '_' : grid[i][j]);
      }
      cout << endl;
   }
   // are we debugging?  if so, dump the list of empty places
   DEBUG << "ctPlaces = " << ctPlaces << ", empty places list:\n";
   if( debug )
   {
      for( i = 0; i < ctPlaces; i++ )
      {
         cout << "Index " << setw(2) << i << " row "
              << places[i].row << " col " << places[i].col << endl;
      }
   }
   if( ctPlaces == 0 )
   {
      cout << "Grid is already solved!" << endl;
   }
   // solve, using the recursive backtracking algorithm
   solved = Solve( grid, places, 0, ctPlaces );

   // did it work?
   if( solved )
   {
      cout << "Solution is:" << endl;
      for( i = 0; i < 9; i++ )
      {
         for( j = 0; j < 9; j++ )
         {
            cout << grid[i][j];
         }
         cout << endl;
      }
   }
   else // oops, no solution...
   {
      cout << "Has no solution\n" << flush;
   }
   cout << endl;
   return 0;
}

#if 0
// sample run with a really difficult problem:
------46-
-----3--9
-84--7--1
2-----71-
5-38-----
9--1--53-
-25--8--6
-----2985
-----5--7
Solution is:
792581463
651243879
384967251
246359718
513874692
978126534
125798346
437612985
869435127
#endif

