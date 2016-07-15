//
// BinarySearchTree.cpp
//
// Binary Search Tree Program adapted from example on
// http://www.cplusplus.happycodings.com/Algorithms/code5.html
// As listed, this contains a major bug.  Can you find it?
//
#include <iostream>
#include <cstdlib>
using namespace std;
class BinarySearchTree
{
private:
   struct tree_node
   {
      tree_node* left;
      tree_node* right;
      int data;
   };
   tree_node* root;
   void destructor_helper( tree_node *root );
   // helpers for print routines
   void inorder(tree_node*);
   void postorder(tree_node*);
   void preorder(tree_node*);
public:
   BinarySearchTree();
   ~BinarySearchTree();
   bool isEmpty() const;
   void print_inorder();
   void print_preorder();
   void print_postorder();
   void insert(int);
   void remove(int);
};

// constructor - create a new empty tree
BinarySearchTree::BinarySearchTree()
{
   root = NULL;
}

// in post-order, traverse tree and delete elements after deleting subtrees
BinarySearchTree::~BinarySearchTree()
{
   cout << "In destructor" << endl;
   destructor_helper( root );
}
// helper to delete nodes in post-order
void BinarySearchTree::destructor_helper( tree_node *r )
{
   if( r != NULL )
   {
      destructor_helper( r->left );
      destructor_helper( r->right );
      cout << "deleting " << r->data << endl;
      delete r;
   }
   return;
}

// return true if the tree is empty, else false
bool BinarySearchTree::isEmpty() const
{
   return root == NULL;
}
// Smaller elements go left
// larger elements go right
void BinarySearchTree::insert(int d)
{
   tree_node* t = new tree_node;
   tree_node* parent = NULL;
   t->data = d;
   t->left = NULL;
   t->right = NULL;
   // is this a new tree?
   if(isEmpty())
   {
      root = t;
   }
   else
   {
      //Note: ALL insertions are as leaf nodes
      tree_node* curr;
      curr = root;
      // Find the Node's parent
      while(curr != NULL)
      {
         parent = curr;
         if(t->data > curr->data)
         {
            curr = curr->right;
         }
         else
         {
            curr = curr->left;
         }
      }
      if(t->data < parent->data)
      {
         parent->left = t;
      }
      else
      {
         parent->right = t;
      }
   }
   return;
}

// this remove routine has a serious bug in it - can you find it?
void BinarySearchTree::remove(int d)
{
   //Locate the element
   bool found = false;
   if(isEmpty())
   {
      cout << " This Tree is empty!" << endl;
      return;
   }
   tree_node* curr;
   tree_node* parent;
   curr = root;
   while(curr != NULL)
   {
      if(curr->data == d)
      {
         found = true;
         break;
      }
      else
      {
         parent = curr;
         if(d > curr->data)
         {
            curr = curr->right;
         }
         else
         {
            curr = curr->left;
         }
      }
   }
   if(!found)
   {
      cout << " Data not found!" << endl;
      return;
   }
   // 3 cases :
   // 1. We're removing a leaf node
   // 2. We're removing a node with a single child
   // 3. we're removing a node with 2 children

   // Node with single child
   if( (curr->left == NULL && curr->right != NULL) ||
       (curr->left != NULL && curr->right == NULL) )
   {
      // right child present, no left child
      if(curr->left == NULL && curr->right != NULL)
      {
         if(parent->left == curr)
         {
            parent->left = curr->right;
            delete curr;
         }
         else
         {
            parent->right = curr->right;
            delete curr;
         }
      }
      else // left child present, no right child
      {
         if(parent->left == curr)
         {
            parent->left = curr->left;
            delete curr;
         }
         else
         {
            parent->right = curr->left;
            delete curr;
         }
      }
      return;
   }
   //We're looking at a leaf node
   if( curr->left == NULL && curr->right == NULL)
   {
      if(parent->left == curr)
      {
         parent->left = NULL;
      }
      else
      {
         parent->right = NULL;
      }
      delete curr;
      return;
   }
   //Node with 2 children
   // replace node with smallest value in right subtree
   // using largest value in left subtree is an alternative
   if (curr->left != NULL && curr->right != NULL)
   {
      tree_node *chkr;
      chkr = curr->right;
      // right chile has no children
      if((chkr->left == NULL) && (chkr->right == NULL))
      {
         curr = chkr;  // replave node value with victim node's value
         delete chkr;
         curr->right = NULL;  // and delete victim
      }
      else // right child has children
      {
         //if the node's right child has a left child
         // Move all the way down left to locate smallest element
         if((curr->right)->left != NULL)
         {
            tree_node* lcurr;
            tree_node* lcurrp;
            lcurrp = curr->right;
            lcurr = (curr->right)->left;
            while(lcurr->left != NULL)
            {
               lcurrp = lcurr;
               lcurr = lcurr->left;
            }
            curr->data = lcurr->data;
            delete lcurr;
            lcurrp->left = NULL;
         }
         else
         {
            tree_node* tmp;
            tmp = curr->right;
            curr->data = tmp->data;
            curr->right = tmp->right;
            delete tmp;
         }
      }
      return;
   }
}

void BinarySearchTree::print_inorder()
{
   inorder(root);
   return;
}

void BinarySearchTree::inorder(tree_node* p)
{
   if(p != NULL)
   {
      if(p->left) inorder(p->left);
      cout << " " << p->data << " ";
      if(p->right) inorder(p->right);
   }
   return;
}

void BinarySearchTree::print_preorder()
{
   preorder(root);
}
void BinarySearchTree::preorder(tree_node* p)
{
   if(p != NULL)
   {
      cout << " " << p->data << " ";
      if(p->left) preorder(p->left);
      if(p->right) preorder(p->right);
   }
   return;
}
void BinarySearchTree::print_postorder()
{
   postorder(root);
}
void BinarySearchTree::postorder(tree_node* p)
{
   if(p != NULL)
   {
      if(p->left) postorder(p->left);
      if(p->right) postorder(p->right);
      cout << " " << p->data << " ";
   }
   return;
}
int main()
{
   BinarySearchTree b;
   int ch,tmp,tmp1;
   while(1)
   {
      cout << endl << endl;
      cout << " Binary Search Tree Operations " << endl;
      cout << " ----------------------------- " << endl;
      cout << " 1. Insertion/Creation " << endl;
      cout << " 2. In-Order Traversal " << endl;
      cout << " 3. Pre-Order Traversal " << endl;
      cout << " 4. Post-Order Traversal " << endl;
      cout << " 5. Removal " << endl;
      cout << " 6. Exit " << endl;
      cout << " Enter your choice : ";
      cin  >> ch;
      switch(ch)
      {
      case 1 : cout << " Enter Number to be inserted : ";
               cin  >> tmp;
               b.insert(tmp);
               break;
      case 2 : cout << "\n In-Order Traversal " << endl;
               cout <<" -------------------" << endl;
               b.print_inorder();
               break;
      case 3 : cout << "\n Pre-Order Traversal "<<endl;
               cout << " -------------------"<<endl;
               b.print_preorder();
               break;
      case 4 : cout << "\n Post-Order Traversal " << endl;
               cout << " --------------------" << endl;
               b.print_postorder();
               break;
      case 5 : cout << " Enter data to be deleted : ";
               cin  >> tmp1;
               b.remove(tmp1);
               break;
      case 6 : system("pause");
               return 0;
      }
   }
}

