from PIL import Image
import numpy as np
import os

def file_to_array(filename):
    '''Converts the image file at filename to a flattened numpy array, which it returns'''
    image = Image.open(filename)
    return np.asarray(image).flatten()

def file_to_image(filename):
    '''Converts the csv file at filename to a 64x64 RGB image, which it returns'''
    image = np.loadtxt(open(filename, "rb"), dtype=np.uint8, delimiter="\n")
    image = image.reshape((64,64,3))
    image = Image.fromarray(image)
    return image

def image_to_array(cur):
    '''Converts a given image to a flattened numpy array'''
    return np.asarray(cur).flatten()

def array_to_image(cur):
    '''Converts a given numpy array to a 64x64 RGB image'''
    image = cur.reshape((64,64,3))
    image = Image.fromarray(image)
    return image

def save_array(cur, filename):
    '''Saves a given numpy array as a csv of integers separated by newlines'''
    np.savetxt(filename, cur, fmt='%i', delimiter="\n")

def save_image(cur, filename):
    '''Saves a given image'''
    cur.save(filename)

def dir_to_arrays(source_dir, dest_dir):
    '''Converts a directory of images into a directory of csvs.
    WARNING: ensure source_dir contains ONLY image files.'''
    files = os.listdir(source_dir)

    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)

    for cur_file in files:
        cur = file_to_array(source_dir+"/"+cur_file)
        save_array(cur, dest_dir+"/"+os.path.splitext(cur_file)[0]+'.csv')

def generate_filenames(cur_dir, dest_filename):
    '''Generates a newline-delimited csv of filenames in cur_dir and saves results to dest_filename'''
    files = os.listdir(cur_dir)

    if os.path.exists(dest_filename):
        os.remove(dest_filename)


    with open(dest_filename, 'w') as file:
        for cur in files:
            file.write(cur_dir+"/"+cur)
            file.write("\n")
