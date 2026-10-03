/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    Willam Barr and Connor Hobbs
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
//#include "vector.h"
#include <vector>
#include <utility>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   // 
   // Construct
   // 

   // create an empty stack
   stack()                                                             {  }

   // create a copy of another stack
   stack(const stack <T> &  rhs) : container(rhs.container)            {  }

   // move the contents of another stack into this stack
   stack(stack <T> && rhs) : container(std::move(rhs.container))       {  }

   // create a stack by copying the contents of a vector
   stack(const std::vector<T> &  rhs) : container(rhs)                 {  }

   // create a stack by moving the contents of a vector
   stack(std::vector<T> && rhs) : container(std::move(rhs))            {  }
   ~stack()                                                            {  }

   //
   // Assign
   //

   // copy the contents of rhs into this stack
   stack <T> & operator = (const stack <T> & rhs)
   {
      container = rhs.container;
      return *this;
   }

   // move the contents of rhs into this stack
   stack <T>& operator = (stack <T> && rhs)
   {
      container = std::move(rhs.container);
      return *this;
   }

   // swap the contents of the two stacks
   void swap(stack <T>& rhs)
   {
      container.swap(rhs.container);
   }

   // 
   // Access
   //

         T& top()       { return *(new T); }
   const T& top() const { return *(new T); }

   // 
   // Insert
   // 

   void push(const T&  t) {  }
   void push(      T&& t) {  }

   //
   // Remove
   //

   void pop() 
   { 
      
   }

   //
   // Status
   //
   size_t  size () const { return 99;  }
   bool empty   () const { return true; }
   
private:
   
  std::vector<T> container;  // underlying container
};



} // custom namespace


