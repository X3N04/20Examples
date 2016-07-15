//
// BStree.cpp
//
// Keith Mehl
// 10/21/06
// CSCI-20 Binary Search Tree example program
// using carriers and (mostly) recursive traversals
// 2/27/14 - fixed a memory leak at insert of duplicate key
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

// a "quick and dirty" tree node class using carrier nodes
class TREENODE
{
private:
   int key;
   string data;
public:
   int GetKey() { return key; }
   string GetData() { return data; }
   TREENODE( int k, string d ) { key = k; data = d; };
   TREENODE() { key = 0; data = ""; }
   // callback function for printing in tree traversal
   void PrintCallBack( ostream &out ) { out << key << ' ' << data << endl; }
};

// The binary search tree class, only partial functionality
class BSTREE
{
private:
   // private internal class for carrier nodes (class composition)
   class CARRIER
   {
   public: // public here means members visible within enclosing class code
      TREENODE *d; // pointer to user data
      CARRIER *left, *right;  // left and right child pointers
      // default constructor with all NULL pointers
      CARRIER() : d(NULL), left(NULL), right(NULL) {}
      // normal constructor, data pointer to user data, NULL children pointers
      CARRIER( TREENODE *p ) : d(p), left(NULL), right(NULL) {}
   };

   CARRIER *root;  // root of the tree
   void InsertHelper( CARRIER **p, TREENODE *e );   // for use by Insert()
   TREENODE *RemoveHelper( CARRIER **p, int k );    // for use by Delete()
   void TraverseHelper( CARRIER *p, ostream &out ); // for use by Traverse()
   void ClearHelper( CARRIER * );
public:
   BSTREE() : root(NULL) {}       // constructor - make an empty tree
   ~BSTREE();                     // destructor - calls ClearHelper() w/root
   void Clear();                  // clean out the tree (deleting everything)
   void Insert( TREENODE *e );    // Add user element by key, if possible
   TREENODE *Remove( int key );   // Remove and return user data
   void Traverse( ostream &out ); // in-order print using callback function
};

// destructor
BSTREE::~BSTREE()
{
    ClearHelper( root );
}

// user clear the tree method
void BSTREE::Clear()
{
    ClearHelper( root );
    root = NULL;
}

void BSTREE::ClearHelper( CARRIER *c )
{
    // postorder traversal to remove all data from tree
    if( c != NULL )
    {
        cout << "clearing " << c->d->GetKey() << ' ' << c->d->GetData() << endl;
        ClearHelper( c->left );
        ClearHelper( c->right );
        delete c->d;
        delete c;
    }
    return;
}

// In-order traversal user interface method
void BSTREE::Traverse( ostream &out )
{
   TraverseHelper( root, out );  // traverse using helper from root
   return;
}

// In-order traversal private helper method
void BSTREE::TraverseHelper( CARRIER *p, ostream &out )
{
   if( p != NULL )
   {
      TraverseHelper( p->left, out );
      p->d->PrintCallBack( out );      // example of a "callback" function
      TraverseHelper( p->right, out );
   }
   return;
}

// Insert a node user public interface method
void BSTREE::Insert( TREENODE *e )
{
   InsertHelper( &root, e );  // just call the helper function
   return;
}

// Private helper method containing
// all the work of inserting the node, unless a duplicate key...
void BSTREE::InsertHelper( CARRIER **p, TREENODE *e )
{
   CARRIER *c;
   // this could set root on first insert into an empty tree
   if( (*p) == NULL )
   {
      c = new CARRIER( e ); // constructor sets child pointers to NULL
      (*p) = c;
   }
   else if( ((*p)->d)->GetKey() == e->GetKey() )  // duplicate?
   {
      cout << "Error - duplicate key " << e->GetKey() << endl;
      delete e;  // fixed a memory leak here!
      return;
   }
   else if( ((*p)->d->GetKey() ) < e->GetKey() ) // insert to right?
   {
      return InsertHelper( &((*p)->right), e );
   }
   else  // otherwise we have to insert to left (only other option)
   {
      return InsertHelper( &((*p)->left), e );
   }
   return;
}

// Remove and return a user TREENODE by key public interface method
TREENODE *BSTREE::Remove( int key )
{
   return RemoveHelper( &root, key );
}

// All the work of removing the node, private helper method
TREENODE *BSTREE::RemoveHelper(  CARRIER **p, int k )
{
   TREENODE *t;
   CARRIER *c;
   if( (*p) == NULL )  // not found?
   {
      return NULL;
   }
   else if( ((*p)->d)->GetKey() == k ) // found it?
   {
      t = (*p)->d; // save pointer to user data to return
      if( (*p)->left == NULL && (*p)->right == NULL ) // leaf node?
      {
         delete (*p); // free carrier node
         (*p) = NULL; // NULL parent's pointer (it points to deallocated RAM)
         return t;    // return user data
      }
      if( (*p)->left == NULL ) // right subtree only?
      {
         c = (*p); // link parent's pointer to this node's right subtree
         (*p) = (*p)->right;
         delete c;
         return t;
      }
      if( (*p)->right == NULL ) // left suntree only?
      {
         c = (*p); // link parent's pointer to this node's left subtree
         (*p) = (*p)->left;
         delete c;
         return t;
      }
      // Otherwise, we have two subtrees, so
      // (iteratively here, could be done recursively)
      // find our victim (here, rightmost element in left subtree)
      c = (*p)->left;
      while( c->right != NULL )
      {
         c = c->right;
      }
      // exchange data in this node and the victim (just swap pointers)
      t = c->d;
      c->d = (*p)->d;
      (*p)->d = t;
      // recursively remove victim in left subtree only
      // is now there with its new data (with swap above)
      // and return its data to user
      return RemoveHelper( &((*p)->left), k );
   }
   else if ( ((*p)->d)->GetKey() < k ) // need to look right?
   {
      return RemoveHelper( &((*p)->right), k );
   }
   else // otherwise we must look left
   {
      return RemoveHelper( &((*p)->left), k );
   }
}

// a little driver with minimal functionality to test BSTREE class
int main()
{
   BSTREE tree;
   TREENODE *p = NULL;
   int command, key;
   bool more = true;
   string str;

   while( more )
   {
      cout << "1) Enter a node\n2) Remove a node\n"
           << "3) Traverse tree\n4) Clear the tree\n"
           << "\nEnter choice (anything else quits): " << flush;
      cin >> command;
      switch( command )
      {
         case 1: cout << "Enter key : " << flush;
                 cin >> key;
                 cin.ignore();
                 cout << "Enter data : " << flush;
                 getline( cin, str );
                 p = new TREENODE( key, str );
                 tree.Insert( p );
                 break;
         case 2: cout << "Enter key : " << flush;
                 cin >> key;
                 p = tree.Remove( key );
                 if( p != NULL )
                 {
                    cout << "Removed node " << p->GetKey()
                         << ", data " << p->GetData() << ".\n" << flush;
                    delete p;
                 }
                 else
                 {
                    cout << "Key " << key << " not found!\n" << flush;
                 }
                 break;
         case 3: tree.Traverse( cout );
                 cout << endl;
                 break;
         case 4: tree.Clear();
                 cout << endl;
                 break;
         default: more = false;
      }
   }
   return 0;
}
