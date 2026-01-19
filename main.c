#include "main.h"

double* KERCONV_API InitKernel(int x)
{
    if (x <= 0 || x % 2 == 0)
        return NULL;

    double* kernel = (double*) malloc((size_t)(x * x) * sizeof(double));
    if (!kernel)
        return NULL;

    return kernel;
}

bool KERCONV_API FreeKernel(double* kernel)
{
    if (!kernel || !*kernel)
        return 0;

    free(kernel);

    //*kernel = NULL;
    return 1;
}


bool KERCONV_API CreateKernel(double* kernel, kernelType ktype, kernelVariation variation, const int x)
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

bool KERCONV_API ApplyKernel(int* image, int* destination, const int imgx, const int imgy, const double* kernel, const int kerx)
{
    if(kerx%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }

    int padding=kerx/2;
    for(int h=padding; h<imgy-padding; h++)
        for(int w=padding; w<imgx-padding; w++)
        {
            double sum=0;
            for(int y=-padding; y<=padding; y++)
                for(int x=-padding; x<=padding; x++)
                    sum+=kernel[(y+padding)*kerx + x+padding]*image[(y+h)*imgx + (x+w)];
            if(sum>255) sum=255;
            if(sum<0) sum=0;
            destination[h*imgx + w]=sum;
        }

    return 1;
}

bool KERCONV_API MakeMono(int* image, const int imgx, const int imgy, int* destination)
{
    //RGB ITU-R BT.601
    for(int i=0;i<imgy;i++)
        for(int j=0;j<imgx;j++)
            destination[i*imgx + j]=(int)(0.299*image[(i*imgx+j)*3] + 0.587*image[(i*imgx+j)*3+1] + 0.114*image[(i*imgx+j)*3+2] + 0.5);
    return 1;
}

bool KERCONV_API CreateKernelSobel(double* kernel, kernelVariation variation, const int x)
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
        binomial[i] = binomial[i-1] * (x-i)/i;
    }

    //derivative
    for (int i=0; i<x; i++) {
        derivative[i] = i - x/2;
    }

    if(variation==Vertical)
    {
        for (int h=0; h<x; h++)
            for (int w=0; w<x; w++)
                kernel[h*x + w] = binomial[h] * derivative[w];

    }
    else if(variation==Horizontal)
    {
        for (int h=0; h<x; h++)
            for (int w=0; w<x; w++)
                kernel[h*x + w] = binomial[w] * derivative[h];
    }
    else
    {
        fprintf(stderr, "Sobel kernel type not recognised\n");
        return 0;
    }

    return 1;
}

bool KERCONV_API CreateKernelScharr(double* kernel, kernelVariation variation, const int x)
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

            if(h==x/2) kernel[h*x + w]*=5;

            else kernel[h*x + w]*=3;
        }
    }
    else if(variation==Horizontal)
    {
        for(int h=0; h<x; h++)
            for(int w=0; w<x; w++)
        {
            if(h==x/2) continue;

            if(w==x/2) kernel[h*x + w]*=5;

            else kernel[h*x + w]*=3;
        }
    }
    else
    {
        fprintf(stderr, "Sobel kernel type not recognised\n");
        return 0;
    }
    return 1;
}

bool KERCONV_API CreateKernelMedian(double* kernel, const int x)
{
    if(x%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }
    double med=1.0/(x*x);
    for(int i=0;i<x;i++)
        for(int j=0;j<x;j++)
            kernel[i*x + j]=med;
    return 1;
}

bool KERCONV_API CreateKernelGaussian(double* kernel, double sigma, const int x)
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
            kernel[(h+half)*x + (w+half)]=value;
            sum += value;
        }
    }

    //normalize
    for (int h=0; h<x; h++)
        for (int w=0; w<x; w++)
            kernel[h*x + w] /= sum;

    return 1;
}

bool KERCONV_API CreateKernelShift(double* kernel, kernelVariation direction, const int x)
{
    if(x%2==0)
    {
        fprintf(stderr, "Kernel size must be odd\n");
        return 0;
    }

    for(int h=0; h<x; h++)
        for(int w=0; w<x; w++)
            kernel[h*x + w]=0;

    if(direction==Up)
    {
        kernel[x/2]=1;
    }
    else if(direction==Down)
    {
        kernel[(x-1)*x + x/2]=1;
    }
    else if(direction==Left)
    {
        kernel[(x/2)*x]=1;
    }
    else if(direction==Right)
    {
        kernel[(x/2)*x + (x-1)]=1;
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
