import os
from PIL import Image, UnidentifiedImageError
import imageio.v3 as iio

# Read all images of folder and create a video
path = 'data/images/63'

images = []
for filename in os.listdir(path):
    images.append(iio.imread(os.path.join(path, filename)))
    #Save the video
    iio.imwrite('data/images/63.mp4', images, fps=10)