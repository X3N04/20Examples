//
// circlularList.cpp
//
// Keith Mehl
// 1/26/10
// CSCI-20 Example program
// Illustrate logic for circular linked list with a header node.
// This is a very "quick and dirty" example, but the logic is right...
//

#include <iostream>
#include <string>
using namespace std;
#include <climits>  // for INT_MAX, which will be my "infinity" key

// options for menu for client code
#define INIT         0
#define INSERTFRONT  1
#define INSERTREAR   2
#define INSERTSORTED 3
#define REMOVEFRONT  4
#define REMOVEREAR   5
#define REMOVEBYKEY  6
#define PRINTALL     7
#define HOWMANY      8
#define QUIT         9

// very simple NODE for list element
// no separation into carrier nodes and data nodes for this example
struct NODE
{
   int key;
   string data;
   NODE *prior, *next;
};

// the largest possible key
#define INFINITY INT_MAX

// a simple LIST class for a circular list with a header-trailer node
// doubly linked
class LIST
{
private:
   NODE *head;    // points to header-trailer node, NOT part of list
   int numNodes;  // how many elements are in the list
public:
   LIST();
   ~LIST();
   NODE *getNode( int key ); // remove & return node by key
   NODE *getFirst();         // remove and return first node
   NODE *getLast();          // remove and return last node
   void insertFront( NODE *e ); // insert node in front of list
   void insertSorted( NODE *e );// insert by key (assumes sorted already!)
   void insertEnd( NODE *e );   // insert at end of list
   void printAll();
   int getCount();
   void init();
};

// constructor
LIST::LIST()
{
   NODE *p = new NODE;
   p->next = p->prior = p; // all list pointers to self
   p->key = INFINITY;      // maximum possible key value is infinity
   p->data = "";           // empty data (this is not really needed)
   head = p;               // point to the list,
   numNodes = 0;           // which is originally empty
}

// destructor
LIST::~LIST()
{
   init();        // clear to empty list
   delete head;   // and delete its header node, too
}

// remove & return node by key
NODE *LIST::getNode( int key )
{
   NODE *p = head->next;
   // while not to end of list and not found
   while( p->key != INFINITY && p->key != key )
   {
      p = p->next;  // advance to next
   }
   if( p->key == key ) // found it?
   {
      // unlink and return element
      // link next element's prior around this element
      p->next->prior = p->prior;
      // link prior element's next around this element
      p->prior->next = p->next;
      // remove all links into data from this element
      p->next = p->prior = NULL;
      // one fewer element in the list
      numNodes--;
      // give element back to caller
      return p;
   }
   return NULL; // otherwise not there
}

// remove & return first node in list
NODE *LIST::getFirst()
{
   NODE *p = head->next;      // point at front element
   if( p->key == INFINITY )   // if empty list, no first element
   {
      return NULL;
   }
   // unlink element from list and return it
   p->next->prior = p->prior;
   p->prior->next = p->next;
   p->next = p->prior = NULL;
   numNodes--;
   return p;
}

// remove & return last node in list
NODE *LIST::getLast()
{
   NODE *p = head->prior;     // point at last element
   if( p->key == INFINITY )   // if empty list, no last element
   {
      return NULL;
   }
   // unlink element from list
   p->next->prior = p->prior;
   p->prior->next = p->next;
   p->next = p->prior = NULL;
   numNodes--;
   return p;
}

// insert a node at the front of the list
void LIST::insertFront( NODE *e )
{
   e->next = head->next;
   e->prior = head;
   head->next->prior = e;
   head->next = e;
   numNodes++;
   return;
}

// insert a node into the list sorted by key
void LIST::insertSorted( NODE *e )
{
   NODE *p = head->next;
   // find node to insert element e before
   while( p->key < e->key )  // must stop because of infinity key
   {
      p = p->next;
   }
   // be careful, as usual, about the order in which you do these!
   e->prior = p->prior;
   e->next = p;
   p->prior->next = e;
   p->prior = e;
   numNodes++;
   return;
}

// insert a node at the end of the list
void LIST::insertEnd( NODE *e )
{
   e->prior = head->prior;
   e->next = head;
   head->prior->next = e;
   head->prior = e;
   numNodes++;
   return;
}

// clean out list
void LIST::init()
{
   NODE *p, *q;
   p = head->next;  // point to first actual element, if there
   // remove all elements in list
   while( p->key != INFINITY  )
   {
      q = p;
      p = p->next;
      delete q;
   }
   // set up empty list
   p->next = p->prior = p;
   numNodes = 0;
   return;
}

// loop through list printing nodes
void LIST::printAll()
{
   NODE *p = head->next;
   while( p->key != INFINITY )
   {
      cout << p->key << " \t" << p->data << endl;
      p = p->next;
   }
   return;
}

// get count of nodes in the list
int LIST::getCount()
{
   return numNodes;
}

// and now a simple client to drive it all
int main()
{
   int choice = 0;   // user selection from menu, initialized to get into loop
   LIST l;           // the linked list itself
   NODE *e;          // for creating and removing nodes from the list
   int key;          // for access by key

   // until user says to quit...
   while( choice != QUIT )
   {
      // print the menu
      cout << "\n0)\tinitialize list\n1)\tInsert front\n2)\tInsert rear\n"
           << "3)\tInsert by key\n4)\tRemove front\n5)\tRemove rear\n"
           << "6)\tRemove by key\n7)\tPrint list\n"
           << "8)\tCount of elements\n9)\tQuit\n\nEnter choice : " << flush;
      cin  >> choice;
      switch( choice )
      {
         case INIT:        l.init();   // initialize an empty list
                           break;

         case INSERTFRONT: e = new NODE;  // insert at front
                           cout << "Enter key and data : " << flush;
                           cin  >> e->key >> e->data;
                           l.insertFront( e );
                           break;

         case INSERTREAR:  e = new NODE;  // insert at rear
                           cout << "Enter key and data : " << flush;
                           cin  >> e->key >> e->data;
                           l.insertEnd( e );
                           break;

         case INSERTSORTED:e = new NODE;  // insert sorted by key
                           cout << "Enter key and data : " << flush;
                           cin  >> e->key >> e->data;
                           l.insertSorted( e );
                           break;

         case REMOVEFRONT: e = l.getFirst(); // remove front element
                           if( e == NULL )   // unless list is empty...
                           {
                              cout << "Not found" << endl;
                           }
                           else
                           {
                              cout << "key " << e->key << ", data " << e->data << endl;
                              delete e;
                           }
                           break;

         case REMOVEREAR:  e = l.getLast();  // remove last element
                           if( e == NULL )
                           {
                              cout << "Not found" << endl;
                           }
                           else
                           {
                              cout << "key " << e->key << ", data " << e->data << endl;
                              delete e;
                           }
                           break;

         case REMOVEBYKEY: cout << "Enter key : " << flush; // remove an element by key
                           cin  >> key;
                           e = l.getNode( key );
                           if( e == NULL )
                           {
                              cout << "Not found" << endl;
                           }
                           else
                           {
                              cout << "key " << e->key << ", data " << e->data << endl;
                              delete e;
                           }
                           break;

         case PRINTALL:    cout << "-------------------------" << endl;  // print the list
                           l.printAll();
                           cout << "-------------------------" << endl;
                           break;

         case HOWMANY:     cout << "LIST has " << l.getCount() << " elements\n"; // count
         case QUIT:        break;

         default:          cout << "I don't understand, sorry...\n" << flush;
      }
   }
   return 0;
}

