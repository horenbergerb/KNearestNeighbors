#ifndef LSH_H_
#define LSH_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <random>

#include "maxheap.h"
#include "kdtree.h"

using namespace std;



class LSH{
 private:
  //random vectors
  float** w;
  //random scalars
  float* b;
  //bin size
  float r;
  //bin quantity
  int P;
  int DIM;

  int num_hashes;

  vector<node>* buckets;
  
 public:
  LSH(float new_r, int new_P, int new_num_hashes, int new_DIM){
    // random device class instance, source of 'true' randomness for initializing random seed
    std::random_device rd; 
    // Mersenne twister PRNG, initialized with seed from previous random device instance
    std::mt19937 gen(rd());

    num_hashes = new_num_hashes;
    
    default_random_engine generator();
    normal_distribution<float> distribution(1.0, 0.0);
    w = new float*[num_hashes];
    b = new float[num_hashes];
    for(int i = 0; i < num_hashes; i++){
      w[i] = new float[new_DIM];
      b[i] = new_r*((float)rand()/RAND_MAX);
    }
    for(int i = 0; i < num_hashes; i++){
      for(int j = 0; j < new_DIM; j++){
	w[i][j] = distribution(gen);
      }
    }
    
    r = new_r;
    P = new_P;
    buckets = new vector<node>[(int)pow(P, num_hashes)];

    DIM = new_DIM;
  };

  int index_buckets(int* indices){
    //converts a multi-dimensional index to a vector index
    //buckets has length P^num_hashes
    //there are P values along each dimension, and a selection in dim i has P^(num_hashes-i) values "under" it
    //For example, if you specify (i,j,k) for a num_hashes=3 case
    //You really want to access index i + P*(j + P*k)= i + Pj + P^2*k
    int index = 0;
    for(int i = 0; i < num_hashes; i++){
      index += indices[i]*(int)pow(P, i);
    }
    return index;
  };
  
  void print_bucket_counts(){
    for(int i = 0; i < (int)pow(P, num_hashes); i++){
      cout << "Bucket " << i << " has " << buckets[i].size() << endl;
    }
  };
  
  int hash(unsigned char* x, int index){
    /*x is the input to be hashed
      w is from a gaussian distribution
      b is a random number from 0 to r
    */
        
    float result = 0;
    for(int i = 0; i < DIM; i++){
      result += x[i]*(float)(w[index][i]);
    }
    result += b[index];
    result = result/r;
    
    return ((int)result)%P;
  };

  void insert(unsigned char* image){
    insert(image, "");
  };

  void insert(unsigned char* image, string name){
    node new_node;
    new_node.image = image;
    new_node.name = name;
    new_node.left = 0;
    new_node.right = 0;
    //cout << "Binning " << name << " in bin " << hash(image) << endl;
    int indices[num_hashes];
    for(int i = 0; i < num_hashes; i++){
      indices[i] = hash(image, i);
    }
    buckets[index_buckets(indices)].push_back(new_node);
  };

  void k_nearest_neighbors(unsigned char* image,  Heap<heapnode>* cur_heap, int k){
    int indices[num_hashes];
    for(int i = 0; i < num_hashes; i++){
      indices[i] = hash(image, i);
    }

    int cur_bucket = index_buckets(indices);

    vector<node>::iterator it;
    for(it = begin(buckets[cur_bucket]); it != end(buckets[cur_bucket]); ++it){
      float cur_dist = euc_dist(image, it->image, DIM);
      if(cur_heap->count() < k || cur_dist < cur_heap->value(0)->key){
	heapnode* new_node = new heapnode();
	new_node->content = &*it;
	new_node->key = cur_dist;
	cur_heap->insert(new_node);
	if(cur_heap->count() > k)
	  cur_heap->remove_max();
      }
    }
  };

};

LSH load_hash_from_csv(string filename, float r, int p, int num_hashes, int DIM){
  LSH new_hash = LSH(r, p, num_hashes, DIM);
  fstream files;
  files.open(filename, ios::in);
  if(files.is_open()){
    string cur_filename;
    while(getline(files, cur_filename)){
      fstream cur_image;
      cur_image.open(cur_filename, ios::in);
      if(cur_image.is_open()){
	unsigned char* image = new unsigned char[DIM];
	int cur = 0;
	string cur_val;
	while(getline(cur_image, cur_val)){
	  image[cur] = stoi(cur_val);
	  cur += 1;
	}
	if(cur_image.is_open())
	  cur_image.close();
	new_hash.insert(image, cur_filename);
	//cout << cur_filename << " loaded" << endl;
      }
      else{
	//cout << cur_filename << " failed to load" << endl;
      }
    }
    if(files.is_open())
      files.close();
  }
  return new_hash;
}

#endif
