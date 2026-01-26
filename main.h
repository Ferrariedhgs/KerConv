#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef KERCONV_EXPORT
#define KERCONV_API __declspec(dllexport)
#else
#define KERCONV_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    Sobel,
    Scharr,
    Median,
    Gauss,
    Shift
}kernelType;

typedef enum
{
    Vertical,
    Horizontal,
    Up,
    Down,
    Left,
    Right
}kernelVariation;

double* KERCONV_API InitKernel(const int x);
bool KERCONV_API FreeKernel(double* kernel);
bool KERCONV_API CreateKernel(double* kernel, kernelType ktype, kernelVariation variation, const int x);
bool KERCONV_API ApplyKernel(int* image, int* destination, const int imgx, const int imgy, const double* kernel, const int kerx);
bool KERCONV_API MakeMono(int* image, const int imgx, const int imgy, int* destination);
bool KERCONV_API CreateKernelSobel(double* kernel, kernelVariation variation, const int x);
bool KERCONV_API CreateKernelScharr(double* kernel, kernelVariation variation, const int x);
bool KERCONV_API CreateKernelMedian(double* kernel, const int x);
bool KERCONV_API CreateKernelGaussian(double* kernel, double sigma, const int x);
bool KERCONV_API CreateKernelShift(double* kernel, kernelVariation direction, const int x);
bool KERCONV_API LoadBMP(char* file, int* destination);
bool KERCONV_API SaveBMP(int* image, char* destination);


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
