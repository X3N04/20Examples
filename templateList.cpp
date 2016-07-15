//
// templateList.cpp
//
// Keith Mehl
// 12/9/08
// CSCI-20 example program, illustrate template classes 
// and a general linked list using a generic type
// 
#include <iostream>
#include <string>
using namespace std;

// template key class - defines a key in type-independent manner
template <class K> class KEY
{
private:
   K key;
public:
   bool operator >( KEY &rhs ) { return key > rhs.key; }
   bool operator >=( KEY &rhs ) { return key >= rhs.key; }
   bool operator ==( KEY &rhs ) { return key == rhs.key; }
   bool operator <( KEY &rhs ) { return key < rhs.key; }   
   bool operator <=( KEY &rhs ) { return key <= rhs.key; }
   bool operator !=( KEY &rhs ) { return key != rhs.key; }
   void setKey( K &k ) { key = k; }
   const K &getKey( void ) const { return key; }
};

// since the default constructor of the ListElement class takes a
// reference to an int, it can't take a constant. This allows the
// constructor to take the reference to defaultKey.  
// in this context, static means it can't be an entry from another file
static int defaultKey = 0;

// Client's list element class
class ListElement
{
private:
   KEY<int> key;
   string name;   // plus any additional fields for example data
public:
   // these operator X methods look like they are comparing ListElements,
   // but they actually compare keys.  This is so the linked list
   // routines can compare elements without worrying about the details!
   // inline definitions - this is "quick-and-dirty", not production code
   bool operator >( ListElement &rhs ) { return key > rhs.key; }
   bool operator >=( ListElement &rhs ) { return key >= rhs.key; }
   bool operator ==( ListElement &rhs ) { return key == rhs.key; }
   bool operator <( ListElement &rhs ) { return key < rhs.key; }   
   bool operator <=( ListElement &rhs ) { return key <= rhs.key; }
   bool operator !=( ListElement &rhs ) { return key != rhs.key; }
   // also, since the key is an int, define operators on int keys
   bool operator >( int rhs ) { return key.getKey() > rhs; }
   bool operator >=( int rhs ) { return key.getKey() >= rhs; }
   bool operator ==( int rhs ) { return key.getKey() == rhs; }
   bool operator <( int rhs ) { return key.getKey() < rhs; }
   bool operator <=( int rhs ) { return key.getKey() <= rhs; }
   bool operator !=( int rhs ) { return key.getKey() != rhs; }

   // and constructor, mutator/observer methods for the list element fields
   ListElement( int k, string &n ) { key.setKey( k ); name = n; }
   ListElement() { key.setKey( defaultKey ); name = ""; }
   void SetName( string &n ) { name = n; };
   string &getName( void ) { return name; }
   void setKey( int k ) { key.setKey( k ); }
   int getKey( void ) { return key.getKey(); }
   // very simple callback method for linked list's print list method's use
   void PrintCallback( void ) { cout << key.getKey() << '\t' << name << endl; }
};

// the linked list routines manage the list "carrier" elements
// they are never seen by the user
struct CARRIER
{
   ListElement *ptr;
   CARRIER *next;
};

// the linked list class - a 'container class'
// it holds data objects used by the client
class LINKLIST
{
private:
   CARRIER *head;        // pointer to the first element in the list
   static CARRIER *free; // static means one var shared by all objects
   CARRIER *getListEl( void );    // get a carrier
   void freeListEl( CARRIER *p ); // release a carrier
   void PrintRevHelper( CARRIER *p ); // recursive print helper method
   static int numLists; // how many lists share free list
   bool traverseOK;      // if traversing, did we change the list?
public:
   const static int Forward; // shared const literals
   const static int Reverse; // for list print direction
   LINKLIST( void ) { numLists++; head = NULL; } // one more empty list
   ~LINKLIST( void );        // destructor
   void AddEl( ListElement *e );  // add sorted in ascending order
   ListElement *removeByKey( int key );
   void PrintList( int direction );
   int IsInList( int key);
   int IsEmpty( void ) { return head == NULL; }
   ListElement *traverse( int first );  // iterator
};

// static class member initializers - executed at load time
CARRIER * LINKLIST::free = NULL;
const int LINKLIST::Forward = 1;
const int LINKLIST::Reverse = -1;
int LINKLIST::numLists = 0;

// get a carrier node - check free list first, else allocate
CARRIER *LINKLIST::getListEl( void )
{
   CARRIER *p;
   if( free != NULL ) {
      p = free;
      free = free->next;
   } else {
      p = new CARRIER;
   }
   p->ptr = NULL;
   p->next = NULL;
   return p;
}

// free element - actually just stash it on 
// the free list to try to limit heap fragmentation
void LINKLIST::freeListEl( CARRIER *p )
{
   p->next = free;
   p->ptr = NULL;
   free = p;
   return;
}

// destructor - free list elements and carriers
// and free list of carriers only if needed (last list destroyed)
LINKLIST::~LINKLIST( void )
{
   cout << "In destructor, list count " << numLists << endl;
   CARRIER *p = head, *q;
   while( p != NULL ) {
      q = p;
      p = p->next;
      delete q->ptr; // destruct data elements
      delete q;      // and carrier elements
   }
   numLists--;
   if( numLists == 0 ) {  // if last list, delete
      p = free;           // the free list of carriers
      while( p != NULL ) {
         q = p;
         p = p->next;
         delete q;
      }
   }
}
// recursive helper to print list in reverse sequence
void LINKLIST::PrintRevHelper( CARRIER *p )
{
   if( p != NULL ) {
      PrintRevHelper( p->next );
      // in a "real" OO environment, we would use a "call-back"
      // function into the client here to print the data...
      p->ptr->PrintCallback();
   }
   return;
}

// user supplies data element *e to add to list by key
void LINKLIST::AddEl( ListElement *e )
{
   CARRIER *d = getListEl(), *p, *q;
   d->ptr = e;
   // "inchworm" down the list to find where to insert the element
   // empty for() loop body - does all the work in the loop header
   for( p = head, q = NULL; 
        p != NULL && *(p->ptr) < *(d->ptr);
        q = p, p = p->next );  // empty loop body!
   if( q == NULL ) {
      d->next = head;
      head = d;
   } else {
      d->next = p;
      q->next = d;
   }
   traverseOK = false; // cannot traverse any more
   return;
}

// remove the first match to key (key may have duplicates)
ListElement *LINKLIST::removeByKey( int key )
{
   CARRIER*p = head, *q = NULL;
   ListElement *d;
   // equivalent while() loop to for() loop above
   while( p != NULL && *(p->ptr) != key ) {
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
   traverseOK = false; // cannot traverse any more
   return d;
}

// forward can be iterative, reverse needs help (or use a stack)
void LINKLIST::PrintList( int direction )
{
   CARRIER *p = head;
   if( direction == 1 ) {
      while( p != NULL ) {
         // in a "real" OO environment, we would use a "call-back"
         // function into the client here to print the data...
         p->ptr->PrintCallback();
         p = p->next;
      }
   } else {
      PrintRevHelper( p );
   }
   return;
}

// return true if key is in list, else false
int LINKLIST::IsInList( int key )
{
   CARRIER *p = head;
   while( p != NULL && *(p->ptr) < key ) {
      p = p->next;
   }
   return p != NULL && *(p->ptr) == key;
}

// iterator - each call after first=true advances pointer
// down the list returning the assuciated data pointer,
// until end of list when you get NULL back
// by agreement with the client, ListElement cannot be changed
ListElement * LINKLIST::traverse( int first )
{ // a static local variable has permanent duration
   static CARRIER *p = NULL;
   if( first ) {
      p = head;
      traverseOK = true; // new traversal is always O.K.
   } else if( !traverseOK ) {
      p = NULL; // can't keep traversing a modified list
   } else if( p != NULL ) {
      p = p->next;
   }
   if( p != NULL ) {
      return p->ptr;
   }
   return NULL;
}

// a simple menu-driven client
int main()
{
   int choice = 0, key;
   ListElement d, *e;
   LINKLIST list;
   string n;
   int k;

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
      case 1:  cout << "Enter key and name : " << flush;
               cin >> k >> n;
               e = new ListElement( k, n );
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
                  cout << "Key " << key << " has name " << e->getName() << endl;
               }
               delete e;
               break;
      case 4:  list.PrintList( LINKLIST::Forward );
               break;
      case 5:  list.PrintList( LINKLIST::Reverse );
               break;
      case 6:  e = list.traverse( true );
               if( e == NULL ) {
                  cout << "There is no first key!\n";
               } else {
                  cout << "Key " << e->getKey() << " has name " << e->getName() << endl;
               }
               break;
      case 7:  e = list.traverse( false );
               if( e == NULL ) {
                  cout << "There is no next key!\n";
               } else {
                  cout << "Key " << e->getKey() << " has name " << e->getName() << endl;
               }
               break;
      case 8:  cout << "Goodbye!" << endl;
               break;
      default: cout << "What???\n\n" << flush;  // should not happen, but...
      }
   }
   return 0;
}


