#include <iostream>
#include <ctime>
#include <string>
#include "kdtree.h"
#include "maxheap.h"
#include "lsh.h"

using namespace std;

int main(){

  int k = 5;
  clock_t start;

  
  cout << "Loading test images..." << endl;
  vector<node> test_images = load_vector_from_csv("./Data/testnames.csv", 64*64*3);


  start = clock();
  cout << "Loading images into KDTree..." << endl;
  KDTree big_tree = load_tree_from_csv("./Data/filenames.csv", 64*64*3);
  cout << "Loaded in " << ( clock() - start ) / (double) CLOCKS_PER_SEC << " seconds" << endl;

  start = clock();
  cout << "Loading images into LSH..." << endl;
  LSH big_hash = load_hash_from_csv("./Data/filenames.csv", 5.0, 3, 8, 64*64*3);
  cout << "Loaded in " << ( clock() - start ) / (double) CLOCKS_PER_SEC << " seconds" << endl;
  
  start = clock();
  cout << "Loading images into vector..." << endl;
  vector<node> image_vector = load_vector_from_csv("./Data/filenames.csv", 64*64*3);
  cout << "Loaded in " << ( clock() - start ) / (double) CLOCKS_PER_SEC << " seconds" << endl;

  cout << "Images loaded: " << image_vector.size() << endl;

  //test k-nearest-neighbors with brute force search
  vector<node>::iterator it;
  for(it = begin(test_images); it != end(test_images); ++it){
    cout << "Finding " << k << " nearest neighbors for " << it->name << endl;

    
    start = clock();
    cout << "    Brute force KNN search..." << endl;
    Heap<heapnode>* brute_heap = new Heap<heapnode>();
    vector<node>::iterator image_it;
    //for each test image, iterate over all images adding to the KNN heap
    for(image_it = begin(image_vector); image_it != end(image_vector); ++image_it){
      float cur_dist = euc_dist(image_it->image, it->image, 64*64*3);
      if(brute_heap->count() < 5 || cur_dist < brute_heap->value(0)->key){
	heapnode* new_node = new heapnode();
	new_node->content = &*image_it;
	new_node->key = cur_dist;
	brute_heap->insert(new_node);
	if(brute_heap->count() > 5)
	  brute_heap->remove_max();
      }
    }
    cout << "    Brute force search completed in " << ( clock() - start ) / (double) CLOCKS_PER_SEC << " seconds" << endl;

    cout << endl;
    
  //test k-nearest-neighbors from KDTree
    start = clock();
    cout << "    KDTree KNN search..." << endl;
    Heap<heapnode>* kd_heap = new Heap<heapnode>();
    big_tree.k_nearest_neighbors(it->image, kd_heap, k);
    cout << "    KDTree search completed in " << ( clock() - start ) / (double) CLOCKS_PER_SEC << " seconds" << endl;

    cout << endl;
    
    //test k-nearest-neighbors from Hash
    start = clock();
    cout << "    Hash KNN search..." << endl;
    Heap<heapnode>* hash_heap = new Heap<heapnode>();
    big_hash.k_nearest_neighbors(it->image, hash_heap, k);
    cout << "    Hash search completed in " << ( clock() - start ) / (double) CLOCKS_PER_SEC << " seconds" << endl;

    cout << endl;

    cout << "    Brute KNN Results:" << endl;
    while(!brute_heap->IsEmpty()){
      heapnode* brute_cur = brute_heap->remove_max();
      cout << "        " << brute_cur->content->name << " " << brute_cur->key << endl;
    }

    cout << "    KDTree KNN Results:" << endl;
    while(!kd_heap->IsEmpty()){
      heapnode* kd_cur = kd_heap->remove_max();
      cout << "        " << kd_cur->content->name << " " << kd_cur->key << endl;
    }
 
    cout << "    Hash KNN Results:" << endl;	
    while(!hash_heap->IsEmpty()){
      heapnode* hash_cur = hash_heap->remove_max();
      cout << "        " << hash_cur->content->name << " " << hash_cur->key << endl;
    }

    cout << endl;

  }

  return 0;
}
  
