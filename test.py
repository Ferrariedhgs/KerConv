import cv2
import numpy as np
import ctypes
import os
import time
import kerconv as kc


img = cv2.imread(os.path.join(os.path.dirname(__file__), "testimg.jpg"))


kerx=3


start=time.time()
dest_display=kc.filter_image(img,kc.KernelType.Sobel,kc.KernelVariation.Vertical,3)
end=time.time()
print(f'dll time: {end-start}\n')
#display image
cv2.imshow("Result", dest_display)
cv2.waitKey(0)
cv2.destroyAllWindows()

img = cv2.imread(os.path.join(os.path.dirname(__file__), "testimg.jpg"))

start=time.time()
img_gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
img_sobel=cv2.Sobel(img_gray,cv2.CV_64F,1,0)
end=time.time()
print(f'cv2 time: {end-start}\n')

#display image
cv2.imshow("Result", img_sobel)
cv2.waitKey(0)
cv2.destroyAllWindows()




dest_display=kc.filter_image(img,kc.KernelType.Scharr,kc.KernelVariation.Vertical,3)
cv2.imshow("Scharr", dest_display)
cv2.waitKey(0)
cv2.destroyAllWindows()

dest_display=kc.filter_image(img,kc.KernelType.Gauss,1,3)
cv2.imshow("Gaussian", dest_display)
cv2.waitKey(0)
cv2.destroyAllWindows()

dest_display=kc.filter_image(img,kc.KernelType.Median,0,3)
cv2.imshow("Median", dest_display)
cv2.waitKey(0)
cv2.destroyAllWindows()

dest_display=kc.filter_image(img,kc.KernelType.Shift,kc.KernelVariation.Up,7)
cv2.imshow("Shift", dest_display)
cv2.waitKey(0)
cv2.destroyAllWindows()
