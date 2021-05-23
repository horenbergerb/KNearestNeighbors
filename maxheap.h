#ifndef MAXHEAP_H_
#define MAXHEAP_H_

#include <iostream>

/*
maxheap code liberally borrows from HW2
i used my own completed assignment here
had to update to make it a maxheap instead of minheap
*/

using namespace std;

//node for the kdtree
typedef struct node{
  node* left;
  node* right;
  unsigned char* image;
  string name;
} node;


//node for the minheap
typedef struct heapnode{
  int position;
  node* content;
  float key;
  
} heapnode;

const int MAX_SIZE = 100000; //the maximum amount of elements our heap should have.

template <typename Object>
class Heap
{
public:
   Heap(){
      elements = 0;
   };
   void insert(Object* item){// Add the object pointer item to the heap
	   if (elements >= MAX_SIZE){
		   cout << "Heap is full; can't insert "<< endl;
		   return;
	   }
	   // FILL IN THE REST
	   //1) Add element to the bottom level at the left-most space
	   //Update array/position
	   item->position = elements;
	   array[elements] = item;
	   //2) Upheap element
	   upHeap(elements);
	   elements += 1;
   };  	
   Object* remove_max(){
      if (elements ==0){
		   cout << "empty heap error, can't delete"<<endl;
	   }
      // FILL IN THE REST
      //1) grab old_root, replace with right-most element on last level
      Object* max = array[0];
      array[0]->position = -1;
      //Update array/position of right-most element
      array[0] = array[elements-1];
      array[0]->position = 0;
      elements -= 1;
      //2) downHeap element
      downHeap(0);
      //3) return old_root
      return max;
   };       // Remove the smallest element in the heap & restructure heap
   
   void increaseKey(int pos, int val)// Increases Key in pos to val
   {
     // FILL IN THE REST
     if(pos >= elements)
       return;
     //1) Change key value of element
     array[pos]->key = val;
     //2) If key value was increased, upHeap element
     upHeap(pos);
   }; 
   

   bool IsEmpty() const {  return (elements <= 0);};
   bool IsFull() const {return (elements >=MAX_SIZE );};
   int count() const {return elements;};
   Object* value(int pos) const{ //return a pointer to an object in heap position
	   if (pos >= elements){
		   cout << "Out of range of heap " << pos << "elements " << elements << endl;
	   }
      return (array[pos]);
   };  
protected:
   Object* array[MAX_SIZE];
   int elements;       //  how many elements are in the heap
private:
   void downHeap(int pos){// starting with element in position pos, sift it down the heap 
                       // until it is in final max-heap position
      Object* item = array[pos];
      while(2*(item->position)+1 < elements){
	//Get the largest child
	Object* next_child = array[2*(item->position)+1];
	int child_ind = 2*(item->position)+1;
	if(child_ind+1 < elements && next_child->key < array[child_ind+1]->key){
	  next_child = array[2*(item->position)+2];
	  child_ind += 1;
	}
	
	//Swap with largest child if necessary
	if(item->key < next_child->key){
	  //Update array
	  array[child_ind] = item;
	  array[item->position] = next_child;
	  //Update positions
	  next_child->position = item->position;
	  item->position = child_ind;
	}
	else{break;}
      }
      // FILL THIS IN
      //swap element with its smallest child until element is smaller than its smallest child

      }; 

   void upHeap(int new_pos){// starting with element in position int, sift it up the heap
                       // until it is in final max-heap position
      Object* item = array[new_pos];
      
      //FILL IN THE REST
      //swap element with its parent until the item is smaller than its parent
      while(item->position >= 0 && (item->key > array[((item->position)-1)/2]->key)){
	//swapping item and parent
	Object* parent = array[((item->position)-1)/2];
	//Update array
	array[((item->position)-1)/2] = item;
	array[item->position] = parent;
	//Update positions
	parent->position = item->position;
	item->position = ((item->position)-1)/2;
	}
   };   
};
#endif
