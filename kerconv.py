import cv2
import numpy as np
import ctypes
import os

ckdll = ctypes.WinDLL (os.path.join(os.path.dirname(__file__), "KerConv.dll"))

ckdll.InitKernel.argtypes = [ctypes.c_int]
ckdll.InitKernel.restype = ctypes.POINTER(ctypes.c_double)

ckdll.FreeKernel.argtypes = [ctypes.POINTER(ctypes.c_double)]
ckdll.FreeKernel.restype = ctypes.c_bool

ckdll.CreateKernel.argtypes = [
    ctypes.POINTER(ctypes.c_double),  #kernel
    ctypes.c_int,                     #kernelType
    ctypes.c_int,                     #kernelVariation
    ctypes.c_int                      #x
]
ckdll.CreateKernel.restype = ctypes.c_bool

ckdll.CreateKernelGaussian.argtypes = [
    ctypes.POINTER(ctypes.c_double),  #kernel
    ctypes.c_double,                  #sigma
    ctypes.c_int                      #x
]
ckdll.CreateKernelGaussian.restype = ctypes.c_bool

ckdll.ApplyKernel.argtypes = [
    ctypes.POINTER(ctypes.c_int), #image
    ctypes.POINTER(ctypes.c_int), #destination
    ctypes.c_int, #image x
    ctypes.c_int, #image y
    ctypes.POINTER(ctypes.c_double), #kernel
    ctypes.c_int #kernel x
]
ckdll.ApplyKernel.restype = ctypes.c_bool

ckdll.MakeMono.argtypes = [
    ctypes.POINTER(ctypes.c_int), #image
    ctypes.c_int, #image x
    ctypes.c_int, #image y
    ctypes.POINTER(ctypes.c_int) #destination
]


def print_kernel(kernelPtr, size):
    kernelValues = [kernelPtr[i] for i in range(size * size)]
    
    for row in range(size):
        line = kernelValues[row*size : (row+1)*size]
        print(" ".join(f"{v:7.3f}" for v in line))


class KernelType:
    Sobel   = 0
    Scharr  = 1
    Median  = 2
    Gauss   = 3
    Shift   = 4

class KernelVariation:
    Vertical   = 0
    Horizontal = 1
    Up         = 2
    Down       = 3
    Left       = 4
    Right      = 5


def init_kernel(size:int):
    if size%2==0:
        raise ValueError("Kernel size must be odd")
    return ckdll.InitKernel(size)

def create_kernel(kernel, ktype:KernelType, variation:KernelVariation, size:int):
    if not ckdll.CreateKernel(kernel, ktype, variation, size):
        raise RuntimeError("CreateKernel failed")

def free_kernel(kernel):
    ckdll.FreeKernel(kernel)

def make_mono(image:np.ndarray):
    image=image.astype(np.int32)
    h,w,c = image.shape
    dest = np.zeros((h, w), dtype=np.int32)
    image_ptr = image.ctypes.data_as(ctypes.POINTER(ctypes.c_int32))
    dest_ptr  = dest.ctypes.data_as(ctypes.POINTER(ctypes.c_int32))
    if not ckdll.MakeMono(image_ptr,w,h,dest_ptr):
        raise RuntimeError("Image could not be processed")
    
    dest=dest.astype(np.int32)
    return dest


    
kernel=init_kernel(3)
create_kernel(kernel,KernelType.Sobel,KernelVariation.Vertical,3)
print_kernel(kernel,3)
