import cv2
import numpy as np
import ctypes
import os


img = cv2.imread(os.path.join(os.path.dirname(__file__), "testimg.jpg"))

KerConv = ctypes.WinDLL (os.path.join(os.path.dirname(__file__), "KerConv.dll"))

KerConv.InitKernel.restype = ctypes.POINTER(ctypes.c_double)
KerConv.FreeKernel.argtypes = [ctypes.POINTER(ctypes.c_double)]
KerConv.CreateKernel.argtypes = [
    ctypes.POINTER(ctypes.c_double),  #kernel
    ctypes.c_int,                     #kernelType
    ctypes.c_int,                     #kernelVariation
    ctypes.c_int                      #x
]
KerConv.CreateKernelGaussian.argtypes = [
    ctypes.POINTER(ctypes.c_double),  #kernel
    ctypes.c_double,                  #sigma
    ctypes.c_int                      #x
]
KerConv.CreateKernel.restype = ctypes.c_bool
KerConv.ApplyKernel.argtypes = [
    ctypes.POINTER(ctypes.c_int), #image
    ctypes.POINTER(ctypes.c_int), #destination
    ctypes.c_int, #image x
    ctypes.c_int, #image y
    ctypes.POINTER(ctypes.c_double), #kernel
    ctypes.c_int #kernel x
]


def print_kernel(kernelPtr, size):
    kernelValues = [kernelPtr[i] for i in range(size * size)]
    
    for row in range(size):
        line = kernelValues[row*size : (row+1)*size]
        print(" ".join(f"{v:7.3f}" for v in line))


#enums from the dll
Sobel=0
Scharr=1
Median=2
Gauss=3
Shift=4

Vertical=0
Horizontal=1
Up=2
Down=3
Left=4
Right=5

kerx=3

#reserve memory for the kernel
kernel=KerConv.InitKernel(kerx)

#create the kernel
KerConv.CreateKernel(kernel,Sobel,Vertical,kerx)
#KerConv.CreateKernelGaussian(kernel,1,kerx)

#print kernel
print_kernel(kernel,kerx)

#make image mono
gray= cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray = gray.astype(np.int32)  
dest = np.zeros_like(gray, dtype=np.int32)

#get pointers
gray_ptr = gray.ctypes.data_as(ctypes.POINTER(ctypes.c_int))
dest_ptr = dest.ctypes.data_as(ctypes.POINTER(ctypes.c_int))

#apply the kernel to the image
KerConv.ApplyKernel(gray_ptr,dest_ptr,393,312,kernel,kerx)

dest_display = np.clip(dest, 0, 255).astype(np.uint8)

#display image
cv2.imshow("Result", dest_display)
cv2.waitKey(0)
cv2.destroyAllWindows()

#free kernel
KerConv.FreeKernel(kernel)