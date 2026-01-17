#include "main.h"

int* KERCONV_API InitKernel(int x)
{
    if (x <= 0 || x % 2 == 0)
        return NULL;

    int* kernel = (int*)malloc((size_t)x * x * sizeof(double));
    if (!kernel)
        return NULL;

    return kernel;
}

bool KERCONV_API FreeKernel(double** kernel)
{
    if (!kernel || !*kernel)
        return 0;
    free(*kernel);
    *kernel = NULL;
    return 1;
}


bool KERCONV_API CreateKernel(const char* type, double** kernel, kernelType ktype, kernelVariation variation, const int x)
{
    if(ktype==Sobel) CreateKernelSobel(kernel,variation,x);
    else if(ktype==Scharr) CreateKernelScharr(kernel,variation,x);
    else if(ktype==Median) CreateKernelMedian(kernel,x);
    //else if(ktype==Gauss)) CreateKernelGaussian(kernel,x,variation);
    else if(ktype==Shift) CreateKernelShift(kernel,variation,x);
    else
    {
        fprintf(stderr, "Kernel type not recognised\n");
        return 0;
    }
    return 1;
}

bool KERCONV_API ConvKernel(int*** image, int** destination, const int imgx, const int imgy, const int imgz, const double** kernel, const int kerx)
{

    return 1;
}

bool KERCONV_API MakeMono(const int*** image, const int imgx, const int imgy, int** destination)
{
    //RGB ITU-R BT.601
    for(int i=0;i<imgx;i++)
        for(int j=0;j<imgy;j++)
            destination[i][j]=(int)(0.299*image[i][j][0]+0.587*image[i][j][1]+0.114*image[i][j][2]);
    return 1;
}

bool KERCONV_API CreateKernelSobel(double** kernel, kernelVariation variation, const int x)
{
    if(x%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }

    int binomial[x], derivative[x];

    //binomial coefficients
    binomial[0]=1;
    for (int i=1; i<x; i++) {
        binomial[i] = binomial[i - 1] * (x-i)/i;
    }

    //derivative
    for (int i=0; i<x; i++) {
        derivative[i] = i - x/2;
    }

    if(variation==Vertical)
    {
        for (int h=0; h<x; h++)
            for (int w=0; w<x; w++)
                kernel[h][w] = binomial[h] * derivative[w];

    }
    else if(variation==Horizontal)
    {
        for (int h=0; h<x; h++)
            for (int w=0; w<x; w++)
                kernel[h][w] = binomial[w] * derivative[h];
    }
    else
    {
        fprintf(stderr, "Sobel kernel type not recognised\n");
        return 0;
    }

    return 1;
}

bool KERCONV_API CreateKernelScharr(double** kernel, kernelVariation variation, const int x)
{
    if(x%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }

    CreateKernelSobel(kernel, variation, x);
    if(variation==Vertical)
    {
        for(int h=0; h<x; h++)
            for(int w=0; w<x; w++)
        {
            if(w==x/2) continue;

            if(h==x/2) kernel[h][w]*=5;

            else kernel[h][w]*=3;
        }
    }
    else if(variation==Horizontal)
    {
        for(int h=0; h<x; h++)
            for(int w=0; w<x; w++)
        {
            if(h==x/2) continue;

            if(w==x/2) kernel[h][w]*=5;

            else kernel[h][w]*=3;
        }
    }
    else
    {
        fprintf(stderr, "Sobel kernel type not recognised\n");
        return 0;
    }
    return 1;
}

bool KERCONV_API CreateKernelMedian(double** kernel, const int x)
{
    if(x%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }
    int med=1/(x*x);
    for(int i=0;i<x;i++)
        for(int j=0;j<x;j++)
            kernel[i][j]=med;
    return 1;
}

bool KERCONV_API CreateKernelGaussian(double** kernel, double sigma, const int x)
{
    if(x%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }

    int half = x/2;
    double sum = 0;

    for (int h=-half; h<=half; h++)
    {
        for (int w=-half; w<=half; w++)
        {
            double value = exp(-(w*w + h*h) / (2 * sigma * sigma));
            kernel[h + half][w + half] = value;
            sum += value;
        }
    }

    //normalize
    for (int h=0; h<x; h++)
        for (int w=0; w<x; w++)
            kernel[h][w] /= sum;

    return 1;
}

bool KERCONV_API CreateKernelShift(double** kernel, kernelVariation direction, const int x)
{
    if(x%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }

    for(int h=0; h<x; h++)
        for(int w=0; w<x; w++)
            kernel[h][w]=0;

    if(direction==Up)
    {
        kernel[0][x/2]=1;
    }
    else if(direction==Down)
    {
        kernel[x-1][x/2]=1;
    }
    else if(direction==Left)
    {
        kernel[x/2][0]=1;
    }
    else if(direction==Right)
    {
        kernel[x/2][x-1]=1;
    }
    else
    {
        fprintf(stderr, "Sobel kernel type not recognised\n");
        return 0;
    }

    return 1;
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
