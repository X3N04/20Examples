//
// stack.cpp
// Keith Mehl
// 11/08/06
// a very simple stack example code
// revised 2/2/10 to add example client
// revised 2/6/14 to add const to peek() and const_cast<> example
//

#include <cstdlib>
#include <iostream>
using namespace std;

// a simple stack element (node) holding just an int
struct STACK_NODE
{
   int key;
   STACK_NODE *next;
};

// the stack itself
class STACK
{
public:
   STACK();
   ~STACK();
   STACK_NODE *pop();
   void push( STACK_NODE *E );
   const STACK_NODE * peek() const;
   void init();
   bool IsEmpty();
private:
   STACK_NODE *top;
};

// constructor
STACK::STACK()
{
   top = NULL;
}

// destructor - must free allocated RAM
STACK::~STACK()
{
   init();
}

// add a node to the top of the stack
void STACK::push( STACK_NODE *e )
{
   e->next = top;
   top = e;
   return;
}

// return a pointer to the top element without removing it, or NULL
const STACK_NODE *STACK::peek() const
{
   return top;
}

// remove and return the top element if there
STACK_NODE *STACK::pop()
{
   STACK_NODE *p = top;
   if( top != NULL )
   {
      top = top->next;
   }
   return p;
}

// clean out stack, returning all memory to system heap
void STACK::init()
{
   STACK_NODE *p = top;
   while( p )
   {
      p = p->next;
      delete top;
      top = p;
   }
   return;
}

// returns true if stack is empty, else false
bool STACK::IsEmpty()
{
   return top == NULL;
}

// a simple client to drive the stack
// one improvement may be to have the client maintain a free list of unused nodes
int main()
{
   STACK s;
   STACK_NODE *p;
   bool done = false;
   int option, value;
   while( !done )
   {
      cout << "1) push\n2) pop\n3) peek\n4) pop all\n5) init\n"
           << "6) check if empty\n7) quit\n\nEnter option : " << flush;
      cin  >> option;
      switch( option )
      {
      case 1:  cout << "Enter value to push : " << flush;
               cin  >> value;
               p = new STACK_NODE;
               p->key = value;
               s.push( p );
               break;
      case 2:  if( !s.IsEmpty() )
               {
                  p = s.pop();
                  value = p->key;
                  delete p;
                  cout << "Popped " << value << endl;
               }
               else
               {
                  cout << "Nothing to pop!" << endl;
               }
               break;
      case 3:  if( !s.IsEmpty() )
               {
                  // cast off const-ness (but DON't mess with the data!)
                  p = const_cast<STACK_NODE *>(s.peek());
                  value = p->key;
                  cout << "Peeked " << value << endl;
               }
               else
               {
                  cout << "Nothing to peek!" << endl;
               }
               break;
      case 4:  while( !s.IsEmpty() )
               {
                  p = s.pop();
                  value = p->key;
                  delete p;
                  cout << "Popped " << value << endl;
               }
               cout << "Stack is now empty" << endl;
               break;
      case 5:  cout << "Initializing..." << endl;
               s.init();
               break;
      case 6:  cout << "IsEmpty returns " << ( s.IsEmpty() ? "true" : "false" ) << endl;
               break;
      case 7:  done = true;
               break;
      default: cout << "What?" << endl;
      }
   }
   return 0;
}


