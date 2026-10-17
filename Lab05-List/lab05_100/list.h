/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    William Barr and Connor Hobbs
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
#include <utility>     // for std::move and std::swap
#include <initializer_list>

class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

   /**************************************************
    * LIST
    * Just like std::list
    **************************************************/
   template <typename T>
   class list
   {
      friend class ::TestList; // give unit tests access to the privates
      friend class ::TestHash;
      friend void swap(list& lhs, list& rhs);
   public:
      // 
      // Construct
      //

      list();
      list(list <T>& rhs);
      list(list <T>&& rhs);
      list(size_t num, const T& t);
      list(size_t num);
      list(const std::initializer_list<T>& il);
      template <class Iterator>
      list(Iterator first, Iterator last);
      ~list()
      {
         clear();
      }

      // 
      // Assign
      //

      list <T>& operator = (list& rhs);
      list <T>& operator = (list&& rhs);
      list <T>& operator = (const std::initializer_list<T>& il);
      void swap(list <T>& rhs);

      //
      // Iterator
      //

      class  iterator;
      // Return iterators to the first node, last node, or past the end.
      iterator begin() { return iterator(pHead); }
      iterator rbegin() { return iterator(pTail); }
      iterator end() { return iterator(nullptr); }

      //
      // Access
      //

      T& front();
      T& back();

      //
      // Insert
      //

      void push_front(const T& data);
      void push_front(T&& data);
      void push_back(const T& data);
      void push_back(T&& data);
      iterator insert(iterator it, const T& data);
      iterator insert(iterator it, T&& data);

      //
      // Remove
      //

      void pop_back();
      void pop_front();
      void clear();
      iterator erase(const iterator& it);

      // 
      // Status
      //

      bool empty()  const { return numElements == 0; }
      size_t size() const { return numElements; }


   private:
      // nested linked list class
      class Node;

      // member variables
      size_t numElements; // though we could count, it is faster to keep a variable
      Node* pHead;    // pointer to the beginning of the list
      Node* pTail;    // pointer to the ending of the list
   };

   /*************************************************
    * NODE
    * the node class.  Since we do not validate any
    * of the setters, there is no point in making them
    * private.  This is the case because only the
    * List class can make validation decisions
    *************************************************/
   template <typename T>
   class list <T> ::Node
   {
   public:
      //
      // Construct
      //
      Node() : data(), pNext(nullptr), pPrev(nullptr) {}
      Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {}
      Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}

      //
      // Data
      //

      T data;                 // user data
      Node* pNext;       // pointer to next node
      Node* pPrev;       // pointer to previous node
   };

   /*************************************************
    * LIST ITERATOR
    * Iterate through a List, non-constant version
    ************************************************/
   template <typename T>
   class list <T> ::iterator
   {
      friend class ::TestList; // give unit tests access to the privates
      friend class ::TestHash;
      template <typename TT>
      friend class custom::list;
   public:
      // An iterator only points to a node; it does not own or create one.
      iterator() : p(nullptr) {}
      iterator(Node* p) : p(p) {}
      iterator(const iterator& rhs) : p(rhs.p) {}

      // Make this iterator point to the same node as rhs.
      iterator& operator = (const iterator& rhs)
      {
         p = rhs.p;
         return *this;
      }

      // Two iterators are equal when they point to the same node.
      bool operator == (const iterator& rhs) const { return p == rhs.p; }
      bool operator != (const iterator& rhs) const { return p != rhs.p; }

      // Get the actual data stored in the current node.
      T& operator * ()
      {
         assert(p != nullptr);
         return p->data;
      }

      // Postfix returns the old position, then moves forward.
      iterator operator ++ (int)
      {
         iterator old(*this);
         ++(*this);
         return old;
      }

      // Prefix moves forward and returns the updated iterator.
      iterator& operator ++ ()
      {
         if (p != nullptr)
            p = p->pNext;
         return *this;
      }

      // Postfix returns the old position, then moves backward.
      iterator operator -- (int)
      {
         iterator old(*this);
         --(*this);
         return old;
      }

      // Prefix moves backward to the previous node.
      iterator& operator -- ()
      {
         if (p != nullptr)
            p = p->pPrev;
         return *this;
      }

      // two friends who need to access p directly
      friend iterator list <T> ::insert(iterator it, const T& data);
      friend iterator list <T> ::insert(iterator it, T&& data);
      friend iterator list <T> ::erase(const iterator& it);

   private:

      typename list <T> ::Node* p;
   };

   /*****************************************
    * LIST :: NON-DEFAULT constructors
    * Create a list initialized to a value
    ****************************************/
   template <typename T>
   list <T> ::list(size_t num, const T& t)
   {
      numElements = 0;
      pHead = pTail = nullptr;
      for (size_t i = 0; i < num; i++)
         push_back(t);
   }

   /*****************************************
    * LIST :: ITERATOR constructors
    * Create a list initialized to a set of values
    ****************************************/
   template <typename T>
   template <class Iterator>
   list <T> ::list(Iterator first, Iterator last)
   {
      // Start empty and add each value from the given range.
      numElements = 0;
      pHead = pTail = nullptr;
      for (; first != last; ++first)
         push_back(*first);
   }

   /*****************************************
    * LIST :: INITIALIZER constructors
    * Create a list initialized to a set of values
    ****************************************/
   template <typename T>
   list <T> ::list(const std::initializer_list<T>& il)
   {
      // Copy the initializer-list values in their original order.
      numElements = 0;
      pHead = pTail = nullptr;
      for (const T& value : il)
         push_back(value);
   }

   /*****************************************
    * LIST :: NON-DEFAULT constructors
    * Create a list initialized to a value
    ****************************************/
   template <typename T>
   list <T> ::list(size_t num)
   {
      numElements = 0;
      pHead = pTail = nullptr;
      for (size_t i = 0; i < num; i++)
         push_back(T());
   }

   /*****************************************
    * LIST :: DEFAULT constructors
    ****************************************/
   template <typename T>
   list <T> ::list()
   {
      numElements = 0;
      pHead = pTail = nullptr;
   }

   /*****************************************
    * LIST :: COPY constructors
    ****************************************/
   template <typename T>
   list <T> ::list(list& rhs)
   {
      // Make our own nodes instead of sharing rhs's pointers.
      numElements = 0;
      pHead = pTail = nullptr;
      for (Node* current = rhs.pHead; current != nullptr; current = current->pNext)
         push_back(current->data);
   }

   /*****************************************
    * LIST :: MOVE constructors
    * Steal the values from the RHS
    ****************************************/
   template <typename T>
   list <T> ::list(list <T>&& rhs)
   {
      // Take ownership of rhs's nodes without copying their data.
      numElements = rhs.numElements;
      pHead = rhs.pHead;
      pTail = rhs.pTail;
      rhs.numElements = 0;
      rhs.pHead = rhs.pTail = nullptr;
   }

   /**********************************************
    * LIST :: assignment operator - MOVE
    * Copy one list onto another
    *     INPUT  : a list to be moved
    *     OUTPUT :
    *     COST   : O(n) with respect to the size of the LHS
    *********************************************/
   template <typename T>
   list <T>& list <T> :: operator = (list <T>&& rhs)
   {
      if (this != &rhs)
      {
         // Delete our old nodes, then take rhs's nodes.
         clear();
         numElements = rhs.numElements;
         pHead = rhs.pHead;
         pTail = rhs.pTail;
         rhs.numElements = 0;
         rhs.pHead = rhs.pTail = nullptr;
      }
      return *this;
   }

   /**********************************************
    * LIST :: assignment operator
    * Copy one list onto another
    *     INPUT  : a list to be copied
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T>
   list <T>& list <T> :: operator = (list <T>& rhs)
   {
      if (this != &rhs)
      {
         // Build a separate copy first, then exchange ownership.
         list<T> temp(rhs);
         swap(temp);
      }
      return *this;
   }

   /**********************************************
    * LIST :: assignment operator
    * Copy one list onto another
    *     INPUT  : a list to be copied
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T>
   list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
   {
      // Make a new list from the values, then replace our old list.
      list<T> temp(rhs);
      swap(temp);
      return *this;
   }

   /**********************************************
    * LIST :: CLEAR
    * Remove all the items currently in the linked list
    *     INPUT  :
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T>
   void list <T> ::clear()
   {
      Node* p = pHead;
      while (p != nullptr)
      {
         Node* pNext = p->pNext;
         delete p;
         p = pNext;
      }
      pHead = pTail = nullptr;
      numElements = 0;
   }

   /*********************************************
    * LIST :: PUSH BACK
    * add an item to the end of the list
    *    INPUT  : data to be added to the list
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::push_back(const T& data)
   {
      Node* n = new Node(data);
      if (numElements == 0)
         pHead = pTail = n;
      else
      {
         n->pPrev = pTail;
         pTail->pNext = n;
         pTail = n;
      }
      ++numElements;
   }

   template <typename T>
   void list <T> ::push_back(T&& data)
   {
      Node* n = new Node(std::move(data));
      if (numElements == 0)
         pHead = pTail = n;
      else
      {
         n->pPrev = pTail;
         pTail->pNext = n;
         pTail = n;
      }
      ++numElements;
   }

   /*********************************************
    * LIST :: PUSH FRONT
    * add an item to the head of the list
    *     INPUT  : data to be added to the list
    *     OUTPUT :
    *     COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::push_front(const T& data)
   {
      Node* n = new Node(data);
      if (numElements == 0)
         pHead = pTail = n;
      else
      {
         n->pNext = pHead;
         pHead->pPrev = n;
         pHead = n;
      }
      ++numElements;
   }

   template <typename T>
   void list <T> ::push_front(T&& data)
   {
      Node* n = new Node(std::move(data));
      if (numElements == 0)
         pHead = pTail = n;
      else
      {
         n->pNext = pHead;
         pHead->pPrev = n;
         pHead = n;
      }
      ++numElements;
   }


   /*********************************************
    * LIST :: POP BACK
    * remove an item from the end of the list
    *    INPUT  :
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::pop_back()
   {
      if (numElements == 0)
         return;

      Node* old = pTail;
      if (pHead == pTail)
         pHead = pTail = nullptr;
      else
      {
         pTail = pTail->pPrev;
         pTail->pNext = nullptr;
      }
      delete old;
      --numElements;
   }

   /*********************************************
    * LIST :: POP FRONT
    * remove an item from the front of the list
    *    INPUT  :
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::pop_front()
   {
      if (numElements == 0)
         return;

      Node* old = pHead;
      if (pHead == pTail)
         pHead = pTail = nullptr;
      else
      {
         pHead = pHead->pNext;
         pHead->pPrev = nullptr;
      }
      delete old;
      --numElements;
   }

   /*********************************************
    * LIST :: FRONT
    * retrieves the first element in the list
    *     INPUT  :
    *     OUTPUT : data to be displayed
    *     COST   : O(1)
    *********************************************/
   template <typename T>
   T& list <T> ::front()
   {
      if (empty())
         throw "ERROR: unable to access data from an empty list";
      return pHead->data;
   }

   /*********************************************
    * LIST :: BACK
    * retrieves the last element in the list
    *     INPUT  :
    *     OUTPUT : data to be displayed
    *     COST   : O(1)
    *********************************************/
   template <typename T>
   T& list <T> ::back()
   {
      if (empty())
         throw "ERROR: unable to access data from an empty list";
      return pTail->data;
   }

   /******************************************
    * LIST :: REMOVE
    * remove an item from the middle of the list
    *     INPUT  : an iterator to the item being removed
    *     OUTPUT : iterator to the new location
    *     COST   : O(1)
    ******************************************/
   template <typename T>
   typename list <T> ::iterator  list <T> ::erase(const list <T> ::iterator& it)
   {
      Node* current = it.p;
      if (current == nullptr)
         return end();

      // Save the following node so we can return its iterator.
      Node* next = current->pNext;

      // Reconnect the neighboring nodes around the removed node.
      if (current->pPrev != nullptr)
         current->pPrev->pNext = current->pNext;
      else
         pHead = current->pNext;

      if (current->pNext != nullptr)
         current->pNext->pPrev = current->pPrev;
      else
         pTail = current->pPrev;

      delete current;
      --numElements;
      return iterator(next);
   }

   /******************************************
    * LIST :: INSERT
    * add an item to the middle of the list
    *     INPUT  : data to be added to the list
    *              an iterator to the location where it is to be inserted
    *     OUTPUT : iterator to the new item
    *     COST   : O(1)
    ******************************************/
   template <typename T>
   typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
      const T& data)
   {
      // Make a copy of the value in a new node.
      Node* added = new Node(data);
      Node* next = it.p;

      if (next == nullptr)
      {
         // Inserting at end() appends the node.
         added->pPrev = pTail;
         if (pTail != nullptr)
            pTail->pNext = added;
         else
            pHead = added;
         pTail = added;
      }
      else
      {
         // Insert immediately before the iterator's current node.
         added->pNext = next;
         added->pPrev = next->pPrev;
         if (next->pPrev != nullptr)
            next->pPrev->pNext = added;
         else
            pHead = added;
         next->pPrev = added;
      }
      ++numElements;
      return iterator(added);
   }

   template <typename T>
   typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
      T&& data)
   {
      // Same pointer changes as copy insert, but move the data.
      Node* added = new Node(std::move(data));
      Node* next = it.p;

      if (next == nullptr)
      {
         added->pPrev = pTail;
         if (pTail != nullptr)
            pTail->pNext = added;
         else
            pHead = added;
         pTail = added;
      }
      else
      {
         added->pNext = next;
         added->pPrev = next->pPrev;
         if (next->pPrev != nullptr)
            next->pPrev->pNext = added;
         else
            pHead = added;
         next->pPrev = added;
      }
      ++numElements;
      return iterator(added);
   }

   /**********************************************
    * LIST :: assignment operator - MOVE
    * Copy one list onto another
    *     INPUT  : a list to be moved
    *     OUTPUT :
    *     COST   : O(n) with respect to the size of the LHS
    *********************************************/
   template <typename T>
   void swap(list <T>& lhs, list <T>& rhs)
   {
      // Exchange the contents without copying any nodes.
      lhs.swap(rhs);
   }

   template <typename T>
   void list<T>::swap(list <T>& rhs)
   {
      // Swap the head, tail, and count in constant time.
      using std::swap;
      swap(pHead, rhs.pHead);
      swap(pTail, rhs.pTail);
      swap(numElements, rhs.numElements);
   }

   //#endif
}; // namespace custom
