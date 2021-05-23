


knn_test: 
	g++ -o knn_test knn_test.cpp kdtree.h maxheap.h

knn_timing:
	g++ -o knn_timing knn_timing.cpp kdtree.h maxheap.h lsh.h

knn_images:
	g++ -o knn_images knn_images.cpp kdtree.h maxheap.h lsh.h

clean:
	-rm -f knn_test knn_timing knn_images *.gch a.out
