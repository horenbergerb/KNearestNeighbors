import PIL.Image
import numpy as np

def make_tiled_images(filename, k):
    '''Designed to run on the output of kdtree_cats.cpp
    Loads a txt file with lines in the form of "CatsRaw/file1.csv CatsRaw/file2.csv ... CatsRaw/filen.csv"
    Parses the file into a nice image showing nearest neighbors '''
      
    files = []
    rows = []
    
    f = open(filename, "r")
    for line in f:
        cur_row = []
        if line != "\n":
            words = line.split(" ")
            for x in range(0, k+1):
                cur_file = "Data/Cats/" + words[x].split("/")[2].split(".")[0] + ".png"
                files.append(cur_file)
                cur_row.append(np.asarray(PIL.Image.open(cur_file).convert('RGB')))
            rows.append(np.hstack(cur_row))
        else:
            spacer = np.array(PIL.Image.open(files[0]))
            spacer.fill(255)

            for x in range(0, k+1):
                cur_row.append(spacer)
            rows.append(np.hstack(cur_row))


    final_im = PIL.Image.fromarray(np.vstack(rows))
    final_im.save("tiled.png")


    #image = PIL.IMage.open()
make_tiled_images("knn_images.out", 5)
