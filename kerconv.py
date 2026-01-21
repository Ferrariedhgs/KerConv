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

def apply_kernel(gray: np.ndarray, kernel, ksize: int) -> np.ndarray:
    if gray.ndim != 2:
        raise ValueError("apply_kernel expects a grayscale image")

    gray = gray.astype(np.int32, copy=False)
    out = np.zeros_like(gray, dtype=np.int32)

    ckdll.ApplyKernel(
        gray.ctypes.data_as(ctypes.POINTER(ctypes.c_int)),
        out.ctypes.data_as(ctypes.POINTER(ctypes.c_int)),
        gray.shape[1],
        gray.shape[0],
        kernel,
        ksize
    )

    return out

def make_mono(rgb: np.ndarray) -> np.ndarray:
    if rgb.ndim != 3 or rgb.shape[2] != 3:
        raise ValueError("make_mono expects an RGB image")

    rgb = rgb.astype(np.int32, copy=False)
    out = np.zeros((rgb.shape[0], rgb.shape[1]), dtype=np.int32)

    ckdll.MakeMono(
        rgb.ctypes.data_as(ctypes.POINTER(ctypes.c_int)),
        rgb.shape[1],
        rgb.shape[0],
        out.ctypes.data_as(ctypes.POINTER(ctypes.c_int))
    )

    return out
    
kernel=init_kernel(3)
create_kernel(kernel,KernelType.Sobel,KernelVariation.Vertical,3)

print_kernel(kernel,3)

free_kernel(kernel)

