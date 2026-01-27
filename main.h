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

typedef enum{
    Mono,
    RGB,
    BGR
}imageFormat;

typedef struct{
    int w; //width
    int h; //height
    unsigned int channels; //no. of channels 1/3
    int* pt; //pointer
    imageFormat format; //image format

}Image;

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
Image KERCONV_API *LoadBMP(char* file);
bool KERCONV_API FreeImage(Image *image);
bool KERCONV_API SaveBMP(Image image, char* destination);


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
