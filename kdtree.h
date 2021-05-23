#ifndef KDTREE_H_
#define KDTREE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <queue>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include "maxheap.h"

using namespace std;

/*
a color image is a 2D array of pixels, where each pixel is defined by three 0-255 values.
we can consider each color of each pixel to be a dimension.
then a 64x64x3 array of chars represents each image.

we create a kdtree based on these arrays
*/

//################
//HELPER FUNCTIONS
//################

float euc_dist(unsigned char* arr1, unsigned char* arr2, int DIM){
  //distance function between vectors of size DIM
  //note: casting is important to prevent underflow
  float dist = 0;
  for(int i = 0; i < DIM; i++){
    dist += pow((float)(arr1[i])-(float)(arr2[i]), 2);
  }
  //we do not square root because it saves on computation
  return dist;
}

bool is_equal(unsigned char* arr1, unsigned char* arr2, int DIM){
  //simple equality checking between two vectors
  for(int i = 0; i < DIM; i++){
    if(arr1[i] != arr2[i])
      return false;
  }
  return true;
}

void print_arr(unsigned char* arr, int DIM){
  //convenience function
  for(int i = 0; i < DIM; i++){
    cout << (int)(arr[i]) << " ";
  }
  cout << endl;
}

//#####################
//KDTREE IMPLEMENTATION
//#####################

//class which implements a kdtree
class KDTree{
 private:
  int DIM;
  
 public:
  node* root;
  
  KDTree(int new_DIM){
    DIM = new_DIM;
    root = 0;
};

  void print(){
    //prints the structure of the tree
    //NOTE: best used only for small test cases
    queue<string> strings;
    strings.push("s");
    
    queue<node*> nodes;
    nodes.push(root);
    
    while(!nodes.empty()){
      node* cur = nodes.front();
      nodes.pop();

      string cur_string = strings.front();
      strings.pop();
      
      cout << cur_string << ": ";
      if(cur->name.compare("") == 0)
	print_arr(cur->image, DIM);
      else
	cout << cur->name << endl;

      if(cur->left){
	nodes.push(cur->left);
	strings.push(cur_string + "l");
      }
      if(cur->right){
	nodes.push(cur->right);
	strings.push(cur_string + "r");
      }
    }
  };
  
  void insert(unsigned char* image){
    //insert function as the user would call it
    //this version is for unnamed nodes
    insert(image, root, 0, "");
  };

  void insert(unsigned char* image, string name){
    //insert function as the user would call it
    //this one is for named nodes
    insert(image, root, 0, name);
  };

  bool insert(unsigned char* image, node* &cur, int cd, string name){
    //actual insert function; this is where the magic happens

    //end of the line case
    if(!cur){
      cur = new node();

      cur->left = 0;
      cur->right = 0;

      cur->image = image;
      cur->name = name;
      
      return true;
    }
    //don't allow inserting duplicates
    else if(is_equal(cur->image, image, DIM)){
      return false;
    }

    //kdtree check to progress down the tree
    else if(image[cd] < cur->image[cd])
      return insert(image, cur->left, (cd+1)%DIM, name);
    else
      return insert(image, cur->right, (cd+1)%DIM, name);
  };

  node* nearest_neighbor(unsigned char* image){
    //finds the single nearest neighbor
    //this function is for the user to call
    return nearest_neighbor(image, root, 0);
  };
  
  node* nearest_neighbor(unsigned char* image, node* &cur, int cd){
    //finds the single nearest neighbor
    
    node* cur_best = 0;
    node* alt_path = 0;
    //recurse down the path which will probably have the nearest neighbor
    if(image[cd] < cur->image[cd]){
      if(cur->left){
	cur_best = nearest_neighbor(image, cur->left, (cd+1)%DIM);
	alt_path = cur->right;
      }
    }
    else{
      if(cur->right){
	cur_best = nearest_neighbor(image, cur->right, (cd+1)%DIM);
	alt_path = cur->left;
      }
    }
    //check if the current node is a better candidate or the preferred branch is null
    if(!cur_best || euc_dist(image, cur->image, DIM) < euc_dist(image, cur_best->image, DIM)){
      cur_best = cur;
    }

    //check if there could be better points on the other branch
    //casting is important to prevent wraparound
    if(abs((float)(image[cd])-(float)(cur->image[cd])) < euc_dist(image, cur_best->image, DIM) && alt_path){
      node* alt_best = nearest_neighbor(image, alt_path, (cd+1)%DIM);
      if(euc_dist(image, alt_best->image, DIM) < euc_dist(image, cur_best->image, DIM))
	cur_best = alt_best;
  }

    return cur_best;
  };

  void k_nearest_neighbors(unsigned char* image, Heap<heapnode>* cur_heap, int k){
    //finds the k nearest neighbors to an image
    //this is the function the user calls
    //fills cur_heap with the k nearest neighbors
    node* cur_best = 0;
    k_nearest_neighbors(image, k, cur_heap, root, 0);
  };

  
  void k_nearest_neighbors(unsigned char* image, int k, Heap<heapnode>* cur_heap, node* &cur, int cd){
    /*fills the cur_heap with the k-nearest neighbors
      similar to nearest_neighbor but keeps updating the cur_heap
      also check alt_branch if we might find a better candidate than our worst candidate
     */
    
    //add the current node if it beats the worst node in the heap
    //then trim the heap so it only has k nodes
    float cur_dist = euc_dist(cur->image, image, DIM);
    if(cur_heap->count() < k || cur_dist < cur_heap->value(0)->key){
      heapnode* new_node = new heapnode();
      new_node->content = cur;
      new_node->key = cur_dist;
      cur_heap->insert(new_node);
      if(cur_heap->count() > k)
	cur_heap->remove_max();
    }
    //recurse down the tree getting the best candidate
    if(cur->left && (float)(cur->image[cd])-(float)(image[cd]) < cur_heap->value(0)->key){
      k_nearest_neighbors(image, k, cur_heap, cur->left, (cd+1)%DIM);
    }
    if(cur->right && (float)(image[cd])-(float)(cur->image[cd]) < cur_heap->value(0)->key){
      k_nearest_neighbors(image, k, cur_heap, cur->right, (cd+1)%DIM);
    }

  };
  
};

//#######################
//LOADING KDTREE FROM DIR
//#######################

/*
KDTree load_tree_from_csv(string filename, int DIM){
  KDTree new_tree = KDTree(DIM);
  
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
	cur_image.close();
	new_tree.insert(image, cur_filename);
	//cout << cur_filename << " loaded" << endl;
      }
      else{
	//cout << cur_filename << " failed to load" << endl;
      }
    }
    files.close();
  }
  return new_tree;
}
*/

//####################
//LOADING TEST IMAGES#
//####################

vector<node> load_vector_from_csv(string filename, int DIM){
  fstream files;
  files.open(filename, ios::in);

  vector<node> images;
  
  if(files.is_open()){
    string cur_filename;
    while(getline(files, cur_filename)){
      fstream cur_image;
      cur_image.open(cur_filename, ios::in);
      if(cur_image.is_open()){
	node cur_node;
	cur_node.name = cur_filename;
	cur_node.image = new unsigned char[DIM];
	int cur = 0;
	string cur_val;
	while(getline(cur_image, cur_val)){
	  cur_node.image[cur] = stoi(cur_val);
	  cur += 1;
	}
	cur_image.close();
	images.push_back(cur_node);
	//cout << cur_filename << " loaded" << endl;
      }
      else{
	//cout << cur_filename << " failed to load" << endl;
      }
    }
    files.close();
  }
  return images;

}

KDTree random_median_insert(int cur_dim, int DIM, vector<node> cur_images, KDTree cur_tree, int num_samples){

  //terminating case; if there are only two values, just insert them
  if(cur_images.size() <= 2){
    vector<node>::iterator it;
    for(it = cur_images.begin(); it != cur_images.end(); ++it){
      cur_tree.insert(it->image, it->name);
    }
    return cur_tree;
  }
  
  //seeding our random sampling
  srand(time(NULL));

  //selecting num_samples random samples
  vector<node> cur_samples;
  if(num_samples == 0 || num_samples >= cur_images.size()){
    cur_samples = cur_images;
  }
  else{
    for(int i = 0; i < num_samples; i++){
      cur_samples.push_back(cur_images[rand()%(cur_images.size())]);
    }
  }

  //sort the samples and find the median
  std::sort(cur_samples.begin(), cur_samples.end(), [cur_dim] (node lhs, node rhs){return (lhs.image[cur_dim] > rhs.image[cur_dim]);});
  int median_value = cur_samples[cur_samples.size()/2].image[cur_dim];
  bool median_inserted = false;
  vector<node> left_side;
  vector<node> right_side;
  vector<node>::iterator it;
  for(it = cur_images.begin(); it != cur_images.end(); ++it){
    if (it->image[cur_dim] > median_value)
      right_side.push_back(*it);
    else if (it->image[cur_dim] < median_value)
      left_side.push_back(*it);
    else if (!median_inserted){
      cur_tree.insert(it->image, it->name);
    }
    else
      right_side.push_back(*it);
  }
  cur_tree = random_median_insert((cur_dim + 1)%DIM, DIM, left_side, cur_tree, num_samples);
  cur_tree = random_median_insert((cur_dim + 1)%DIM, DIM, right_side, cur_tree, num_samples);  
  
  return cur_tree;
  
}

KDTree load_tree_from_csv(string filename, int DIM, int num_samples){

  //the eventual tree
  KDTree new_tree = KDTree(DIM);

  //the raw collection of images
  vector<node> images = load_vector_from_csv(filename, DIM);
  
  return random_median_insert(0, DIM, images, new_tree, num_samples);  
}  

KDTree load_tree_from_csv(string filename, int DIM){
  return load_tree_from_csv(filename, DIM, 0);
}


#endif
