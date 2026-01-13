#include "main.h"

// a sample exported function
void KERCONV_API CreateKernel(const char* type, int** kernel, const int x, const char* variation)
{
    if(strcmp(type,"sobel")) CreateKernelSobel(kernel,x,variation);
    else if(strcmp(type,"median")) CreateKernelMedian(kernel,x);
    else if(strcmp(type,"gaussian")) CreateKernelGaussian(kernel,x,variation);
    else if(strcmp(type,"shift")) CreateKernelShift(kernel,x,variation);
}

void KERCONV_API ConvKernel(int*** image, int** destination, const int imgx, const int imgy, const int imgz, const int** kernel, const int kerx)
{

}

void KERCONV_API MakeMono(const int*** image, const int imgx, const int imgy, int** destination)
{
    //RGB ITU-R BT.601
    for(int i=0;i<imgx;i++)
        for(int j=0;j<imgy;j++)
            destination[i][j]=(int)(0.299*image[i][j][0]+0.587*image[i][j][1]+0.114*image[i][j][2]);
}

void KERCONV_API CreateKernelSobel(int** kernel, const int x, const char* variation)
{
    if(strcmp(variation,"vertical"))
    {

    }
    else if(strcmp(variation,"horizontal"))
    {

    }
    else perror("Sobel kernel type not recognized");
}

void KERCONV_API CreateKernelMedian(int** kernel, const int x)
{
    int med=1/(x*x);
    for(int i=0;i<x;i++)
        for(int j=0;j<x;j++)
            kernel[i][j]=med;
}

void KERCONV_API CreateKernelGaussian(int** kernel, const int x, const char* sigma)
{

}

void KERCONV_API CreateKernelShift(int** kernel, const int x, const char* direction)
{

}
/*
BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
*/
