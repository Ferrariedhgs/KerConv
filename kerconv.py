import cv2
import numpy as np
import ctypes
import os

ckdll = ctypes.WinDLL (os.path.join(os.path.dirname(__file__), "KerConv.dll"))

ckdll.InitKernel.restype = ctypes.POINTER(ctypes.c_double)
ckdll.FreeKernel.argtypes = [ctypes.POINTER(ctypes.c_double)]
ckdll.CreateKernel.argtypes = [
    ctypes.POINTER(ctypes.c_double),  #kernel
    ctypes.c_int,                     #kernelType
    ctypes.c_int,                     #kernelVariation
    ctypes.c_int                      #x
]
ckdll.CreateKernelGaussian.argtypes = [
    ctypes.POINTER(ctypes.c_double),  #kernel
    ctypes.c_double,                  #sigma
    ctypes.c_int                      #x
]
ckdll.CreateKernel.restype = ctypes.c_bool
ckdll.ApplyKernel.argtypes = [
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

def free_kernel(kernel):
    ckdll.FreeKernel(kernel)

def create_kernel(kernel, ktype:KernelType, variation:KernelVariation, size:int):
    if not ckdll.CreateKernel(kernel, ktype, variation, size):
        raise RuntimeError("CreateKernel failed")
    
kernel=init_kernel(3)
create_kernel(kernel,KernelType.Sobel,KernelVariation.Vertical,3)

print_kernel(kernel,3)

free_kernel(kernel)
