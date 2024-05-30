/*

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

typedef void (*KernelFunction)(int**, int**);

// Integrate this into CUDAWrapperCall so it's more readable.
typedef VRAM_ptr void*;


//  NOTICE: This currently takes in integer values when ideally it takes in floats

// @TODO: Create an alternative method that's basically the same but with floats. Samw with the CUDA_INT_ARRAY structure


// Using this will require a re-structuring of the codebase so that prop attributes are all stored on one void* 
typedef struct CUDA_INT_ARRAY
{
    float* x;
    unsigned int rows;
    unsigned int columns;
};

cudaError_t CUDAWrapperCall(CUDA_INT_ARRAY input, CUDA_INT_ARRAY output, KernelFunction kernel)
{
    // set them all to nullptr

    int* dev_input[input.width];
    for (int i = 0; i < input.width; i++)
    {
        dev_input[i] = nullptr;
    }

    cudaError_t cudaStatus;


    // Shouldn't need to change this
    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output).
    // Copy IO vectors to VRAM

    for (int i = 0; i < input.width; i++)
    {
        // This populates a pointer at dev_input[i] as an address in VRAM
        cudaStatus = cudaMalloc((void**)&dev_input[i], input.height * sizeof(int));
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "cudaMalloc failed!");
            goto Error;
        }
        // Copies each array(input.x[i]) at dev_input[i]
        cudaStatus = cudaMemcpy(dev_input[i], input.x[i], input.height * sizeof(int), cudaMemcpyHostToDevice);
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "cudaMemcpy failed!");
            goto Error;
        }
    }
    


    // Launch a kernel on the GPU with one thread for each element. dev_input is a pointer within VRAM to an array of pointers (Stored in VRAM) to individual int fields (each column)
    kernel <<<1, height >>> (dev_input, output);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(a, dev_input[i], height * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    for (int i = 0; i < input.width; i++)
    {
        cudaFree(dev_input[i]);
    }
    


    return cudaStatus;
}
*/