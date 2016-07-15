//
// linkList.cpp
//
// Keith Mehl 8/24/06
// CSCI-20 Example program -- illustrate simple singly-linked linked list
//

#include <iostream>
using namespace std;

// The user manages the data elements themselves
struct DATA
{
   int key;
   char data[25];
};

// the linked list routines manage the list "carrier" elements
struct LISTEL
{
   DATA *ptr;
   LISTEL *next;
};

class LINKLIST
{
private:
   LISTEL *head;          // the data in the user's list
   bool TraverseOK;       // Is it OK to continue a traversal?
   static LISTEL *free;   // maintain a free list to reduce heap fragmentation
   LISTEL *trav_p;        // for use by traverse
   LISTEL *getListEl( void );  // Get a carrier node
   void freeListEl( LISTEL *p ); // Free a carrier node
   void PrintRevHelper( LISTEL *p );  // used for reverse print in PrintList()
public:
   static int Forward; // parameter values - print direction in PrintList
   static int Reverse;
   LINKLIST( void ) { head = NULL; TraverseOK = false; }
   ~LINKLIST( void );
   void AddEl( DATA *e );  // add sorted in ascending order
   DATA *removeByKey( int key );
   void PrintList( int direction );
   int IsInList( int key);
   int IsEmpty( void ) { return head == NULL; }
   DATA *traverse( int first );  // iterator
};

LISTEL * LINKLIST::free = NULL;
int LINKLIST::Forward = 1;
int LINKLIST::Reverse = -1;

// allocate storage for a carrier node, unless stashed on free list
LISTEL *LINKLIST::getListEl( void )
{
   LISTEL *p;
   if( free != NULL ) {
      p = free;
      free = free->next;
   } else {
      p = new LISTEL; // this can fail, I'm ignoring that possibility
   }
   p->ptr = NULL;
   p->next = NULL;
   return p;
}

// 'deallocate' carrier node storage 
// actually just stash it on free list for reuse later
void LINKLIST::freeListEl( LISTEL *p )
{
   p->next = free;
   p->ptr = NULL;
   free = p;
   return;
}

// destructor - free all carrier **and data** nodes
// This is the only place where the List touches data (except the key)
LINKLIST::~LINKLIST( void )
{
   LISTEL *p = head, *q;
   while( p != NULL ) {
      q = p;
      p = p->next;
      delete q->ptr;  // only time the list routines change the data!
      delete q;
   }
   p = free;
   while( p != NULL ) {  // remember to clean out the free list too
      q = p;
      p = p->next;
      delete q;
   }
   free = NULL;// since there might be another LINKLIST object later!
}

// print a single element.  This could have an ostream & parameter too
void LINKLIST::PrintRevHelper( LISTEL *p )
{
   if( p != NULL ) {
      PrintRevHelper( p->next );
      // in a "real" OO environment, we could use a "call-back"
      // function into the client here to print the data...
      // e.g., use a class with a data print method
      cout << p->ptr->key << '\t' << p->ptr->data << endl;
   }
   return;
}

// add an element at its ordered position into the list
void LINKLIST::AddEl( DATA *e )
{
   LISTEL *d = getListEl();
   LISTEL *p, *q;
   TraverseOK = false;  // if inside a traversal now, stop it
   d->ptr = e;
   // "inchworm" down the list to find insert position
   for( p = head, q = NULL;
        p != NULL && p->ptr->key < d->ptr->key;
        q = p, p = p->next );
   if( q == NULL ) {
      d->next = head;
      head = d;
   } else {
      d->next = p;
      q->next = d;
   }
   return;
}

// remove the first match to key (may have duplicates)
DATA *LINKLIST::removeByKey( int key )
{
   LISTEL*p = head, *q = NULL;
   DATA *d;
   while( p != NULL && p->ptr->key != key ) {
      q = p;
      p = p->next;
   }
   if( p == NULL ) {
      return NULL;  // indicate key not found
   }
   else if( q == NULL ) {
      head = head->next;
   } else {
      q->next = p->next;
   }
   d = p->ptr;
   freeListEl( p ); // don't delete here, manage fragmentation on heap!
   TraverseOK = false;  // if inside a traversal now, stop it
   return d;
}

// print the list, given a direction
// this would normally have a ostream parameter too
void LINKLIST::PrintList( int direction )
{
   LISTEL *p = head;
   if( direction == Forward ) {
      while( p != NULL ) {
         // in a "real" OO environment, we would use a "call-back"
         // function into the client here to print the data...
         cout << p->ptr->key << '\t' << p->ptr->data << endl;
         p = p->next;
      }
   } else {
      PrintRevHelper( p );
   }
   return;
}

// return true if the key is in the list
int LINKLIST::IsInList( int key )
{
   LISTEL *p = head;
   while( p != NULL && p->ptr->key < key ) {
      p = p->next;
   }
   return p != NULL && p->ptr->key == key;
}

// iterator, either first call or subsequent call, returns pointer to
// element's data or NULL if traversed off the list
DATA * LINKLIST::traverse( int first )  // this is the iterator
{
   if( first ) {
      trav_p = head;
      TraverseOK = true;
   } else if( TraverseOK == false ) {  // no traversal if list changed
      return NULL;                     // since start of last traversal
   } else if( trav_p != NULL ) {
      trav_p = trav_p->next;
   }
   if( trav_p != NULL ) {  // trav_p points to carrier, return data
      return trav_p->ptr;
   }
   return NULL;
}

// a driver program to test the linked list class
// rather than maintaining a free list of DATA elements,
// we just allocate and free them as needed
int main()
{
   int choice = 0, key;
   DATA d, *e;   // a temporary work DATA and a pointer to allocated RAM
   LINKLIST list;

   while( choice != 8 ) {
      cout << "\n\n1. add an element\n"       
           << "2. see if an element exists\n"
           << "3. remove an element by key\n" 
           << "4. print forward\n"
           << "5. print reverse\n"
           << "6. traverse (first)\n"
           << "7. traverse (next)\n"
           << "8. quit\n" << "\nEnter choice : " 
           << flush;
      cin >> choice;
      switch( choice )
      {
      case 1:  cout << "Enter key and data : " << flush;
               cin >> d.key >> d.data;
               e = new DATA;  // allocate some RAM to add to the list
               (*e) = d;  // DON'T just enter the local struct into the list!!!
               list.AddEl( e );
               break;
      case 2:  cout << "Enter key : " << flush;
               cin >> key;
               if( list.IsInList( key ) ) {
                  cout << "Key " << key << " is in the list\n";
               } else {
                  cout << "Nope, the key " << key << " isn't there\n";
               }
               break;
      case 3:  cout << "Enter key : " << flush;
               cin >> key;
               e = list.removeByKey( key );
               if( e == NULL ) {
                  cout << "The key " << key << " wasn't there!\n";
               } else {
                  cout << "Key " << e->key << " has data " << e->data << endl;
                  delete e;
               }
               break;
      case 4:  list.PrintList( LINKLIST::Forward );
               break;
      case 5:  list.PrintList( LINKLIST::Reverse );
               break;
      case 6:  e = list.traverse( true );
               if( e == NULL ) {
                  cout << "There is no first key!\n";
               } else {
                  cout << "Key " << e->key << " has data " << e->data << endl;
               }
               break;
      case 7:  e = list.traverse( false );
               if( e == NULL ) { // Note: this can happen if TraverseOK is false
                  cout << "There is no next key!\n";
               } else {
                  cout << "Key " << e->key << " has data " << e->data << endl;
               }
               break;
      case 8:  cout << "Goodbye!" << endl;
               break;
      default: cout << "What???\n\n" << flush;
      }
   }
   return 0;
}


