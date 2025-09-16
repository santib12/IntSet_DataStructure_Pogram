// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     dynamic array whose size is stored in member variable
//     capacity; the member variable data references the array.
// (2) The distinct int value with earliest membership is stored
//     in data[0], the distinct int value with the 2nd-earliest
//     membership is stored in data[1], and so on.
//     Note: No "prior membership" information is tracked; i.e.,
//           if an int value that was previously a member (but its
//           earlier membership ended due to removal) becomes a
//           member again, the timing of its membership (relative
//           to other existing members) is the same as if that int
//           value was never a member before.
//     Note: Re-introduction of an int value that is already an
//           existing member (such as through the add operation)
//           has no effect on the "membership timing" of that int
//           value.
// (4) The # of distinct int values the IntSet currently contains
//     is stored in the member variable used.
// (5) Except when the IntSet is empty (used == 0), ALL elements
//     of data from data[0] until data[used - 1] contain relevant
//     distinct int values; i.e., all relevant distinct int values
//     appear together (no "holes" among them) starting from the
//     beginning of the data array.
// (6) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1].
//     Note: This applies also when the IntSet is empry (used == 0)
//           in which case we DON'T care what is stored in any of
//           the data array elements.
//     Note: A distinct int value in the IntSet can be any of the
//           values an int can represent (from the most negative
//           through 0 to the most positive), so there is no
//           particular int value that can be used to indicate an
//           irrelevant value. But there's no need for such an
//           "indicator value" since all relevant distinct int
//           values appear together starting from the beginning of
//           the data array and used (if properly initialized and
//           maintained) should tell which elements of the data
//           array are actually relevant.
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//           Note: Recall that one of the things a constructor
//                 has to do is to make sure that the object
//                 created BEGINS to be consistent with the
//                 class invariant. Thus, resize() should not
//                 be used within constructors unless it is at
//                 a point where the class invariant has already
//                 been made to hold true.
//     Post: The capacity (size of the dynamic array) of the
//           invoking IntSet is changed to new_capacity...
//           ...EXCEPT when new_capacity would not allow the
//           invoking IntSet to preserve current contents (i.e.,
//           value for new_capacity is invalid or too low for the
//           IntSet to represent the existing collection),...
//           ...IN WHICH CASE the capacity of the invoking IntSet
//           is set to "the minimum that is needed" (which is the
//           same as "exactly what is needed") to preserve current
//           contents...
//           ...BUT if "exactly what is needed" is 0 (i.e. existing
//           collection is empty) then the capacity should be
//           further adjusted to 1 or DEFAULT_CAPACITY (since we
//           don't want to request dynamic arrays of size 0).
//           The collection represented by the invoking IntSet
//           remains unchanged.
//           If reallocation of dynamic array is unsuccessful, an
//           error message to the effect is displayed and the
//           program unconditionally terminated.

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

//private helper function chnages the capacity of the dynamic array.
void IntSet::resize(int new_capacity)
{
   if(new_capacity < 1){
      new_capacity = 1;
   }

   if(new_capacity < used){
      new_capacity = used;
   }

   if(used ==0 && new_capacity == 0){
      new_capacity = DEFAULT_CAPACITY;
   }

   if(new_capacity != capacity){
      int* new_data = new int[new_capacity];

      for(int i =0; i < used; i++){
         new_data[i] = data[i];
      }

      // free old memory
      delete[] data;
      data = new_data;
      capacity = new_capacity;
   }

}

// create an IntSet with specified intial capacity (or default if not specified)
IntSet::IntSet(int initial_capacity)
{
   if(initial_capacity < 1){
      initial_capacity = DEFAULT_CAPACITY;
   }

   capacity = initial_capacity;
   used  = 0;
   data = new int[capacity];
}

//create a deep copy of another IntSet object
IntSet::IntSet(const IntSet& src)
{
   capacity = src.capacity;
   used = src.used;

   data = new int[capacity];

   for(int i =0; i < used; i++){
      data[i] = src.data[i];
   }
   
}

//frees the dinamically allocated memory when the object is destroyed
IntSet::~IntSet()
{
   delete[] data;
}

//Assigns one IntSet to another with proper memory managment
IntSet& IntSet::operator=(const IntSet& rhs)
{
   if(this == &rhs){
      return *this;
   }

   delete[] data;

   capacity = rhs.capacity;
   used = rhs.used;

   data = new int[capacity];

   for(int i = 0; i < used; i++){
      data[i] = rhs.data[i];
   }

   return *this;
}

////Returns the number of elements in the set
int IntSet::size() const
{
   return used;
}

//Returns true if the set is empty, flase otherwise
bool IntSet::isEmpty() const
{
   return (used ==0);
}

//Checks if a given integer is in the set
bool IntSet::contains(int anInt) const
{
   for(int i = 0; i < used; i++){
      if(data[i] == anInt){
         return true;
      }
   }

   return false;
}

//checks if this set is a subset of another set
bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
   if(used == 0){
      return true;
   }

   for(int i = 0; i < used; i++){
      if(!otherIntSet.contains(data[i])){
         return false;
      }
   }

   return true;
}

void IntSet::DumpData(ostream& out) const
{  // already implemented ... DON'T change anything
   if (used > 0)
   {
      out << data[0];
      for (int i = 1; i < used; ++i)
         out << "  " << data[i];
   }
}

IntSet IntSet::unionWith(const IntSet& otherIntSet) const
{
   IntSet result = *this;
   for(int i = 0; i < otherIntSet.used; i++){
      result.add(otherIntSet.data[i]);
   }

   return result;
}

IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
  IntSet result;

  for(int i = 0; i < used; i++){
      if(otherIntSet.contains(data[i])){
         result.add(data[i]);
      }
  }

  return result;
}

IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
   IntSet result = *this;

   for(int i = 0; i < otherIntSet.used; i++){
      result.remove(otherIntSet.data[i]);
   }

   return result;
}

void IntSet::reset()
{
   used = 0;
}

bool IntSet::add(int anInt)
{
   if(contains(anInt)){
      return false;
   }

   if(used == capacity){
      int new_capacity = int(1.5 * capacity) + 1;
      resize(new_capacity);
   }

   data[used] = anInt;
   used++;

   return true;
}

bool IntSet::remove(int anInt)
{
   int index = -1;

   for(int i = 0; i < used; i++){
      if(data[i] == anInt){
         index = i;
         break;
      }
   }

   if(index == -1){
      return false;
   }

   for(int i = index; i < used -1; i++){
      data[i] = data[i + 1];
   }

   used --;
   return true;
}

bool operator==(const IntSet& is1, const IntSet& is2)
{
  // Different sizes means different sets
  if(is1.size() != is2.size()){
      return false;
  }

  // Two sets are equal if each is a subset of the other
  return (is1.isSubsetOf(is2) && is2.isSubsetOf(is1));
}
