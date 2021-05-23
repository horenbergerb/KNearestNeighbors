# Introduction

This project is an attempt to implement three K-nearest neighbor search algorithms on a set of images. The first algorithm is the brute-force search. The next is using KDtrees. The final algorithm uses locality-sensitive hashing.

The algorithms are implemented in C++, but there is also Python code for prepping the image dataset.

# Preparing the Images

We assume you have a dataset of uniformly-sized images, all of which are stored in a single directory. You will need to use the functions in image_conversion.py to convert a directory of images into a directory of csv files. Additionally, you will need to generate a csv of filenames.

For example, lets say we have a directory "KNN/Cats" which contains 29,843 unique 64x64 images of cats. Then we might prepare this data by opening a Python3 terminal in the "KNN" directory and running the following:

```
from image_conversion import *
dir_to_arrays("Cats", "CatsRaw")
generate_filenames("CatsRaw", "arraynames.csv")
```

You will also want to take a handful of lines from arraynames.csv and move them into a new file, testnames.csv. These will be the images we find nearest-neighbors for, so we don't want them inserted into the data structures.

Then the C++ code is ready to compute using CatsRaw, arraynames.csv, and testnames.csv.

To see more details about the implemented file structure (or make changes), check out the filenames referenced via load_from_csv commands in the cpp files.

# Running Nearest-Neighbor Algorithms

There are three main programs you can run. The first is a simple test case. The second program is designed for comparing the speed and accuracy of the three methods on a real dataset. The third generates an image of the k nearest neighbors for visual inspection.

## Test Case

To run the simple test case, you must first call

```
make knn_test
```

Then, you can run the program and dump the output to a file as follows

```
./knn_test >> knn_test.out
```

This output details the performance of each method finding 5 nearest neighbors in the dataset for 5 test images.

## Performance Test

To run the test on the real dataset, you must first call

```
make knn_timing
```

Then, you can run the program and dump the output to a file as follows

```
./knn_timing >> knn_timing.out
```

This output details the performance of each method finding 5 nearest neighbors in the dataset for 5 test images.

## Image Generation

The other program which can be run is designed to create an output which can then be converted into an image. First you must call
```
make knn_images
```

Then, you can run the program and dump the output,

```
./knn_images >> knn_images.out
```

To convert this into an image, you can execute the program

```
python make_tiled_images.py
```