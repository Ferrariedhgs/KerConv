#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <string>

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

int* KERCONV_API InitKernel(const int x=3);
bool KERCONV_API FreeKernel(int** kernel);
bool KERCONV_API CreateKernel(const char* type, int** kernel, kernelType ktype, kernelVariation variation, const int x=3);
bool KERCONV_API ConvKernel(int*** image, int** destination, const int imgx, const int imgy, const int imgz, const int** kernel, const int kerx=3);
bool KERCONV_API MakeMono(int*** image, const int imgx, const int imgy, int** destination);
bool KERCONV_API CreateKernelSobel(int** kernel, kernelVariation variation, const int x=3);
bool KERCONV_API CreateKernelScharr(int** kernel, kernelVariation variation, const int x=3);
bool KERCONV_API CreateKernelMedian(int** kernel, const int x=3);
bool KERCONV_API CreateKernelGaussian(int** kernel, double sigma, const int x=3);
bool KERCONV_API CreateKernelShift(int** kernel, kernelVariation direction, const int x=3);


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
