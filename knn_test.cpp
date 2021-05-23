#include <iostream>
#include "kdtree.h"
#include "lsh.h"

using namespace std;

int main(){

  int k = 3;

  KDTree tree = KDTree(k);
  LSH lsh = LSH(1.0, 2, 3, k);
  vector<unsigned char*> brute;

  unsigned char* p1 = new unsigned char[3]{1, 1, 1};
  unsigned char* p2 = new unsigned char[3]{5, 5, 5};
  unsigned char* p3 = new unsigned char[3]{23, 3, 7};
  unsigned char* p4 = new unsigned char[3]{4, 4, 4};
  unsigned char* p5 = new unsigned char[3]{11, 83, 20};
  unsigned char* p6 = new unsigned char[3]{100, 1, 50};
  unsigned char* p7 = new unsigned char[3]{1, 2, 1};
  unsigned char* p8 = new unsigned char[3]{50, 50, 50};

  
  tree.insert(p1);
  tree.insert(p2);
  tree.insert(p3);
  tree.insert(p4);
  tree.insert(p5);
  tree.insert(p6);
  tree.insert(p7);
  tree.insert(p8);

  lsh.insert(p1);
  lsh.insert(p2);
  lsh.insert(p3);
  lsh.insert(p4);
  lsh.insert(p5);
  lsh.insert(p6);
  lsh.insert(p7);
  lsh.insert(p8);

  brute.push_back(p1);
  brute.push_back(p2);
  brute.push_back(p3);
  brute.push_back(p4);
  brute.push_back(p5);
  brute.push_back(p6);
  brute.push_back(p7);
  brute.push_back(p8);

  unsigned char* test_image = new unsigned char[3]{3, 4, 3};
  
  Heap<heapnode>* brute_heap = new Heap<heapnode>();
  vector<unsigned char*>::iterator image_it;
  //for each test image, iterate over all images adding to the KNN heap
  for(image_it = begin(brute); image_it != end(brute); ++image_it){
    float cur_dist = euc_dist(*image_it, test_image, 3);
    if(brute_heap->count() < k || cur_dist < brute_heap->value(0)->key){
      node* bong_node = new node();
      bong_node->image = *image_it;
      bong_node->name = "";
      heapnode* new_node = new heapnode();
      new_node->content = bong_node;
      new_node->key = cur_dist;
      brute_heap->insert(new_node);
      if(brute_heap->count() > k)
	brute_heap->remove_max();
    }
  }

  
  Heap<heapnode>* kd_heap = new Heap<heapnode>();
  tree.k_nearest_neighbors(test_image, kd_heap, k);

  
  //test k-nearest-neighbors from Hash
  Heap<heapnode>* hash_heap = new Heap<heapnode>();
  lsh.k_nearest_neighbors(test_image, hash_heap, k);
  
  cout << "Brute KNN Results:" << endl;
  while(!brute_heap->IsEmpty()){
    heapnode* brute_cur = brute_heap->remove_max();
    for(int i = 0; i < 3; i++){
      cout << (int)(brute_cur->content->image[i]) << " ";
    }
    cout << "Distance: " << brute_cur->key << endl;
  }
  
  cout << "    KDTree KNN Results:" << endl;
  while(!kd_heap->IsEmpty()){
    heapnode* kd_cur = kd_heap->remove_max();
    for(int i = 0; i < 3; i++){
      cout << (int)(kd_cur->content->image[i]) << " ";
    }
    cout << "Distance: " << kd_cur->key << endl;
  }
  
  cout << "    Hash KNN Results:" << endl;	
  while(!hash_heap->IsEmpty()){
    heapnode* hash_cur = hash_heap->remove_max();
    for(int i = 0; i < 3; i++){
      cout << (int)(hash_cur->content->image[i]) << " ";
    }
    cout << "Distance: " << hash_cur->key << endl;
  }
  
  return 0;
}
