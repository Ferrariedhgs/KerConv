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

void KERCONV_API CreateKernel(const char* type,int** kernel, const int x, const char* variation);
void KERCONV_API ConvKernel(int*** image, int** destination, const int imgx, const int imgy, const int imgz, const int** kernel, const int kerx);
void KERCONV_API MakeMono(int*** image, const int imgx, const int imgy, int** destination);
void KERCONV_API CreateKernelSobel(int** kernel, const int x, const char* variation);
void KERCONV_API CreateKernelMedian(int** kernel, const int x);
void KERCONV_API CreateKernelGaussian(int** kernel, const int x, const char* sigma);
void KERCONV_API CreateKernelShift(int** kernel, const int x, const char* direction);


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
