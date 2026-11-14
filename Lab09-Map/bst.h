/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    William Barr and Connor Hobbs
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   //
   // Construct
   //

   BST() : root(nullptr), numElements(0) {}
   BST(const BST &  rhs) : root(nullptr), numElements(0) { *this = rhs; }
   BST(      BST && rhs) : root(rhs.root), numElements(rhs.numElements) { rhs.root = nullptr; rhs.numElements = 0; }
   BST(const std::initializer_list<T>& il) : root(nullptr), numElements(0) { *this = il; }
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t) const;

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements; }
   

private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree

};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : data(), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}

    BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}

    BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return (pNode->pParent->pRight == pNode); }
   bool isLeftChild( BNode * pNode) const { return (pNode->pParent->pLeft == pNode); }

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;
public:
    // constructors and assignment
    iterator(BNode* p = nullptr)
            : pNode(p)
    { }

    iterator(const iterator& rhs)
            : pNode(rhs.pNode)
    { }

    iterator & operator = (const iterator & rhs)
    {
        pNode = rhs.pNode;
        return *this;
    }

    // compare
    bool operator == (const iterator & rhs) const
    {
        return pNode == rhs.pNode;
    }

    bool operator != (const iterator & rhs) const
    {
        return pNode != rhs.pNode;
    }

    // de-reference. Cannot change because it will invalidate the BST
    const T & operator * () const
    {
        return pNode->data;
    }


    // increment and decrement
    iterator & operator ++ ();
    iterator   operator ++ (int postfix)
    {
        iterator old(*this);
        ++(*this);
        return old;
    }
    iterator & operator -- ();
    iterator   operator -- (int postfix)
    {
        iterator old(*this);
        --(*this);
        return old;
    }


    // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/


/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/


/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST<T>::~BST()
{
    clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST<T>& BST<T>::operator=(const BST<T>& rhs)
{
    if (this == &rhs) return *this;

    if (!rhs.root) {            // rhs empty → result empty
        clear();
        return *this;
    }

    // helpers
    auto clone = [&](auto&& self, BNode* src, BNode* parent) -> BNode*
    {
        if (!src) return nullptr;
        BNode* n = new BNode(src->data);   // copy-construct T
        n->pParent = parent;
        n->pLeft   = self(self, src->pLeft,  n);
        n->pRight  = self(self, src->pRight, n);
        return n;
    };

    auto assignShape = [&](auto&& self, BNode* dst, BNode* src) -> void
    {
        if (!dst || !src) return;
        dst->data = src->data;             // Spy::numAssign++
        self(self, dst->pLeft,  src->pLeft);
        self(self, dst->pRight, src->pRight);
    };

    auto count = [&](auto&& self, BNode* n) -> size_t
    {
        if (!n) return 0;
        return 1 + self(self, n->pLeft) + self(self, n->pRight);
    };

    auto destroy = [&](auto&& self, BNode* n) -> void
    {
        if (!n) return;
        self(self, n->pLeft);
        self(self, n->pRight);
        delete n;                           // Spy delete/destructor++
    };

    const size_t rhsCount = count(count, rhs.root);

    // Case 1: rhs has exactly one node → reuse dest root, delete its subtrees
    if (rhsCount == 1)
    {
        if (!root) {
            root = new BNode(rhs.root->data);   // single copy/alloc if dest was empty
            numElements = 1;
            return *this;
        }
        // assign root once, drop both subtrees, no new allocs/copies
        root->data = rhs.root->data;
        destroy(destroy, root->pLeft);
        destroy(destroy, root->pRight);
        root->pLeft = root->pRight = nullptr;
        numElements = 1;
        return *this;
    }

    // Case 2: dest has exactly one node → assign root, then clone rhs children
    if (root && !root->pLeft && !root->pRight)
    {
        root->data  = rhs.root->data;                    // one assign
        root->pLeft  = clone(clone, rhs.root->pLeft,  root);
        root->pRight = clone(clone, rhs.root->pRight, root);
        numElements  = count(count, root);
        return *this;
    }

    // Case 3: shapes match (e.g., standard → standard) → assign in place
    if (root && numElements == rhsCount)
    {
        assignShape(assignShape, root, rhs.root);
        return *this;
    }

    // Fallback: different shapes → clear then clone
    clear();
    root = clone(clone, rhs.root, nullptr);
    numElements = rhsCount;
    return *this;
}

//keep

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();                    // wipe current contents
   for (const auto& x : il)    // insert each element
      insert(x);
   return *this;               // <-- required
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    clear();
    swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
    BNode* tempRoot = rhs.root;
    rhs.root = root;
    root = tempRoot;

    size_t tempElements = rhs.numElements;
    rhs.numElements = numElements;
    numElements = tempElements;
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
/*****************************************************
 * BST :: INSERT  (lvalue)
 ****************************************************/
template <typename T>
std::pair<typename BST<T>::iterator, bool>
BST<T>::insert(const T& t, bool keepUnique)
{
    if (!root)
    {
        root = new BNode(t);
        ++numElements;
        return { iterator(root), true };
    }

    BNode* cur = root;
    BNode* parent = nullptr;
    bool wentLeft = false;

    while (cur)
    {
        parent = cur;
       if (keepUnique && (t == cur->data))
           return { iterator(cur), false };

        if (t < cur->data)
        {
            cur = cur->pLeft;
            wentLeft = true;
        }
        else
        {
            cur = cur->pRight;   // equal keys go right (multiset-style)
            wentLeft = false;
        }
    }

    BNode* n = new BNode(t);
    if (wentLeft) parent->addLeft(n);
    else          parent->addRight(n);

    ++numElements;
    return { iterator(n), true };
}


template <typename T>
std::pair<typename BST<T>::iterator, bool>
BST<T>::insert(T&& t, bool keepUnique)
{
    if (!root)
    {
        root = new BNode(std::move(t));
        ++numElements;
        return { iterator(root), true };
    }

    BNode* cur = root;
    BNode* parent = nullptr;
    bool wentLeft = false;

    while (cur)
    {
        parent = cur;
       if (keepUnique && (t == cur->data))
           return { iterator(cur), false };

        if (t < cur->data)
        {
            cur = cur->pLeft;
            wentLeft = true;
        }
        else
        {
            cur = cur->pRight;   // equal keys go right (multiset-style)
            wentLeft = false;
        }
    }

    BNode* n = new BNode(std::move(t));
    if (wentLeft) parent->addLeft(n);
    else          parent->addRight(n);

    ++numElements;
    return { iterator(n), true };
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator& it)
{
    BNode* z = it.pNode;
    if (!z) return end();

    // successor BEFORE mutating the tree
    iterator next = it;
    ++next;

    auto transplant = [&](BNode* u, BNode* v)
    {
        // replace subtree rooted at u with subtree rooted at v
        if (!u->pParent)
            root = v;
        else if (u->pParent->pLeft == u)
            u->pParent->pLeft = v;
        else
            u->pParent->pRight = v;

        if (v)
            v->pParent = u->pParent;
    };

    if (!z->pLeft)                      // 0 or 1 child (right only)
    {
        transplant(z, z->pRight);
        delete z;
        --numElements;
        return next;
    }
    else if (!z->pRight)               // 1 child (left only)
    {
        transplant(z, z->pLeft);
        delete z;
        --numElements;
        return next;
    }
    else                               // 2 children
    {
        // successor = leftmost node of right subtree
        BNode* s = z->pRight;
        while (s->pLeft) s = s->pLeft;

        if (s->pParent != z)
        {
            // move s's right child up
            transplant(s, s->pRight);
            // put z's right subtree under s
            s->pRight = z->pRight;
            if (s->pRight) s->pRight->pParent = s;
        }

        // replace z with s
        transplant(z, s);
        s->pLeft = z->pLeft;
        if (s->pLeft) s->pLeft->pParent = s;

        delete z;
        --numElements;
        return next;
    }
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST<T>::clear() noexcept
{
    std::function<void(BNode*)> recurse = [&](BNode* p)
    {
        if (!p) return;
        recurse(p->pLeft);
        recurse(p->pRight);
        delete p;
    };

    recurse(root);
    root = nullptr;
    numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
    if (empty())
        return end();
    BNode* p = root;
    while (p->pLeft)
        p = p->pLeft;
   return iterator(p);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t) const
{

    BNode* p = root;
    while (p)
    {
        if (p->data == t)
            return iterator(p);
        else if (t < p->data)
            p = p->pLeft;
        else
            p = p->pRight;
    }
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
    // if homeboy does then make pLeft pAdd
    this->pLeft = pNode;
    if (pNode) {
        //if pAdd also exists then make it's parent pNode
        pNode->pParent = this;
    }
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    // if homeboy does then make pLeft pAdd
    this->pRight = pNode;
    if (pNode) {
        //if pAdd also exists then make it's parent pNode
        pNode->pParent = this;
    }
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    // copy the node
    BNode* pNew = new BNode(t);
    // hook the nodes up to each other
    this->pLeft = pNew;
    pNew->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    // move the node instead of copying it
    BNode* pNew = new BNode(std::move(t));
    // hook the nodes up to each other
    this->pLeft = pNew;
    pNew->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    // copy the node
    BNode* pNew = new BNode(t);
    // hook the nodes up to each other
    this->pRight = pNew;
    pNew->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    // move the node instead of copying it
    BNode* pNew = new BNode(std::move(t));
    // hook the nodes up to each other
    this->pRight = pNew;
    pNew->pParent = this;
}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
    if (!pNode)
        return *this;

    // Case 1: has right child -> go right, then all the way left
    if (pNode->pRight)
    {
        pNode = pNode->pRight;
        while (pNode->pLeft)
            pNode = pNode->pLeft;
    }
        // Case 2: no right child -> go up until we come from a left child
    else
    {
        BNode* up  = pNode->pParent;
        BNode* cur = pNode;
        while (up && cur == up->pRight)
        {
            cur = up;
            up  = up->pParent;
        }
        pNode = up;  // may become nullptr (end)
    }

    return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST<T>::iterator& BST<T>::iterator::operator--()
{
   // If we're already at end(), stay there
   if (pNode == nullptr)
      return *this;

   // Case 1: there is a left subtree -> go to its rightmost (predecessor)
   if (pNode->pLeft)
   {
      pNode = pNode->pLeft;
      while (pNode->pRight)
         pNode = pNode->pRight;
      return *this;
   }

   // Case 2: climb up until we come from a right child
   auto *cur = pNode;
   auto *up  = pNode->pParent;
   while (up && cur == up->pLeft)
   {
      cur = up;
      up  = up->pParent;
   }
   pNode = up;   // may become nullptr, which equals end()
   return *this;
}

} // namespace custom


