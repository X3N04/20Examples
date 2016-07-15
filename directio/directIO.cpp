//
// directIO.cpp
//
// Keith Mehl
// 4/15/2010
// CSCI-20 example program
// Illustrate direct file IO
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;
#pragma pack(1)  // do not allow the compiler to put a hole into the struct
struct DATA 
{
   char key[6];    // item number
   char desc[25];  // description -- notice this is NOT a string (C or C++ style)!
   char tax;       // 'Y'/'N' taxable
   char price[8];  // 6.2 format price with assumed fixed point
};
#pragma pack()   // restore normal packing

int main()
{
   int i, nrecs, records[4] = { 1, 3, 2, 0 };  // for "random" file access
   long pos;            // for file access offsets from position 0 in file
   char inbuf[sizeof(DATA)+4];  // read buffer with a bit extra for '\n', '\0'
   DATA d;                      // struct for the file read/write data
   char itemNo[7] = "";         // work buffers for character buffer
   char desc[26] = "";          //   to C-string conversions
   char tax;
   char price[9] = "";
   double dPrice = 0.0;         // for string-to-double conversion for price

   memset( itemNo, '\0', 7 );   // fill buffers with all '\0' (NUL) characters
   memset( desc, '\0', 26 );    // this puts trailing NUL characters on C-strings
   memset( price, '\0', 9 );    // after copying fields from the data structure
   
   ifstream dataFile( "test.txt" ); // input from text file
   
   // open flags: may read or write file at any point (writes not appended at end)
   // not ASCII characters with '\n' end-of-line, delete any data in file
   fstream myfile( "data.txt",
                   fstream::in | fstream::out | fstream::binary | fstream::trunc );
   if( !dataFile )
   {
      cout << "Data file open failed\n";
      return -1;
   }
   if( !myfile )
   {
      cout << "direct file open failed\n";
      dataFile.close();
      return -2;
   }
   // extract data from input "flat file", e.g. just characters
   // build structure and write it to output file
   nrecs = 0;
   while( nrecs < 4 && !dataFile.eof() )
   {
      dataFile.getline( inbuf, sizeof(DATA)+2, '\n' );
      memcpy( &d, inbuf, sizeof(DATA) );
      cout << "i = " << nrecs << " data <" << inbuf << '>' << endl;
      myfile.write( (char *)&d, sizeof(DATA) );
      nrecs++;
   }
   // now seek "randomly" into data file and extract records, printing them
   for( i = 0; i < nrecs; i++ )
   {
      pos = records[i] * sizeof(DATA);
      myfile.seekg( pos );
      myfile.read( (char *)&d, sizeof(DATA) );
      memcpy( itemNo, d.key, sizeof(d.key) );
      memcpy( desc, d.desc, sizeof(d.desc) );
      memcpy( price, d.price, sizeof( d.price) );
      tax = d.tax;
      dPrice = atof( price ) / 100.0;
      cout << "Index " << i << " record " << records[i]
           << "\n   item no : " << itemNo << ", desc : " << desc 
           << "\n   taxable : " << tax << ", price : $" << dPrice 
           << '\n' << endl;
   }
   dataFile.close();
   myfile.close();
   return 0;
}

