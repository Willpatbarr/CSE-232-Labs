/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    Connor Hobbs and William Barr
 ************************************************************************/

#pragma once

#include <stdexcept>  // for std::out_of_range
#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map() 
   {
   }

   map(const map &  rhs) 
   { 
      bst = rhs.bst;
   }

   map(map && rhs) 
   { 
      bst = std::move(rhs.bst);
   }

   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
      while (first != last)
      {
         insert(*first);
         ++first;
      }
   }

   map(const std::initializer_list <Pairs>& il) 
   {
      for (const auto& item : il) 
      {
         insert(item);
      }
   }

  ~map()         
   {
   }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
      bst = rhs.bst;
      return *this;
   }

   map & operator = (map && rhs)
   {
      bst = std::move(rhs.bst);
      return *this;
   }

   map & operator = (const std::initializer_list <Pairs> & il)
   {
      clear();

      for (const auto& item : il) 
      {
         insert(item);
      }
      return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(bst.begin());
   }

   iterator end() 
   { 
      return iterator(bst.end());    
   }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
   iterator    find(const K & k)
   {
      return iterator(bst.find(Pairs(k)));
   }

   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
      auto result = bst.insert(std::move(rhs), true); // keys stay unique
      return make_pair(iterator(result.first), result.second);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
      auto result = bst.insert(rhs, true); // keys stay unique
      return make_pair(iterator(result.first), result.second);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      for (auto it = first; it != last; ++it)
         insert(*it);
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
      for (const auto & p : il)
         insert(p);
   }

   //
   // Remove
   //
   void clear() noexcept
   {
      bst.clear();
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();; 
   }

   size_t size() const noexcept 
   { 
      return bst.size();
   }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //
   iterator()
   {
   }

   iterator(const typename BST < pair <K, V> > :: iterator & rhs)
   { 
      it = rhs;
   }

   iterator(const iterator & rhs) 
   { 
      it = rhs.it;
   }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
      it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }
   bool operator != (const iterator & rhs) const 
   { 
      return it != rhs.it;
   }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
      return *it;
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
      it++;
      return *this;
   }

   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++it;
      return temp;
   }
   iterator & operator -- ()
   {
      --it;
      return *this;
   }
   iterator  operator -- (int postfix)
   {
      iterator temp = *this;
      --it;
      return temp;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
   // if the key is already there, insert just hands back that node
   auto result = bst.insert(Pairs(key), true);
   return result.first.pNode->data.second;
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> :: operator [] (const K& key) const
{
   // can't insert into a const map, so this works the same as at()
   return at(key);
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
   auto it = bst.find(Pairs(key));
   if (it == bst.end())
      throw std::out_of_range("invalid map<K, T> key");
   return it.pNode->data.second;
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
   // BST::find is not const, so reuse the non-const version
   return const_cast<map <K, V> *>(this)->at(key);
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
   lhs.bst.swap(rhs.bst);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
   auto it = bst.find(Pairs(k));
   if (it == bst.end())
      return 0;

   bst.erase(it);
   return 1;
}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   while (first != last)
      first = erase(first);

   return last;
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   return iterator(bst.erase(it.it));
}

}; //  namespace custom

