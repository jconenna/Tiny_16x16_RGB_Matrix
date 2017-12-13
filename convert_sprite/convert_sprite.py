from PIL import Image
from PIL import ImageOps

import numpy as np

title = input("Enter the name of the png file to convert, *without* the .png: ")

im = Image.open(title + ".png")

p = np.array(im)
p = np.rot90(p)
p = np.rot90(p)
p = np.rot90(p)
p = np.flipud(p)

f1 = open(title + ".txt", 'w')

f1.write("const uint8_t " + title + "[256] PROGMEM = {")

for i in range(0, im.size[0]):
        
    for j in range(0, im.size[1]):

         r = im.getpixel((i, j))[0] >> 5
         g = im.getpixel((i, j))[1] >> 5
         b = im.getpixel((i, j))[2] >> 6
         
         color = (r << 5) | ( g << 2) | b

         f1.write(str(color))

    

         if(not(i == (im.size[0]-1) and j == (im.size[0]-1))):
           f1.write(",")

    
f1.write("};\n")
f1.write("const uint8_t *" + title + "_ptr = " + title + ";")
f1.close()

print("\n...")
input("\nsuccess!")
