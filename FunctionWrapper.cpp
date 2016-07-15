//
// FunctionWrapper.cpp
//
// Keith Mehl
// 7/6/2010
// CSCI-20 Example program
// Illustrate wrapper functions to use function pointers with class methods
//

#include <iostream>
#include <fstream>
using namespace std;

// first a simple class

class TestInt
{
private:
   int data;
public:
   TestInt() { data = 0; }
   TestInt( int x ) { data = x; }
   int getVal( void ) { return data; }
   void setVal( int x ) { data = x; }
   friend ostream & operator << ( ostream &out, TestInt &i );
   friend istream & operator >> ( istream &in, TestInt &i );
   int Square( void ) { return data * data; }
   int Cube( void ) { return data * data * data; }
};

ostream & operator << ( ostream &out, TestInt &i )
{
   out << i.data;
   return out;
}

istream & operator >> ( istream &in, TestInt &i )
{
   in >> i.data;
   return in;
}

// a couple of wrapper functions around getVal(), Square() and Cube()
int getVal( TestInt &i )
{
   return i.getVal();
}
int Square( TestInt &i )
{
   return i.Square();
}
int Cube( TestInt &i )
{
   return i.Cube();
}

// pf is a type
// 'pointer to a function that takes a reference to a TestInt
//  and returns an int'
typedef int (*pf)( TestInt & );

// array of three pointers to functions
const pf a[3] = { getVal, Square, Cube };

int main()
{
   int i, x;
   TestInt val(3);
   for( i = 0; i < 3; i++ )
   {
      x = (a[i])(val);
      cout << val << ' ' << x << endl;
   }
   return 0;
}


