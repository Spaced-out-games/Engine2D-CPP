/*
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <time.h>
#include <stdio.h>

unsigned int lcg_seed = 0;
//1664525
#define RANDOM_A 1664525632
#define RANDOM_C 1013904223

typedef void (*KernelFunction3)(const int*, const int*, int*);
typedef void (*KernelFunction1)(int*, long long);
cudaError_t runCUDAOperation3(const int *a, const int* b, int* c, unsigned int size, KernelFunction3 kernel);
cudaError_t runCUDAOperation1(int* a, unsigned int size, KernelFunction1 kernel);




__global__ void CUDA_Add(const int *a, const int *b, int* c)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}


__global__ void CUDA_RAND(int* array, long long seed)
{
    int i = threadIdx.x;
    //array[i] = (RANDOM_A* (seed * i)) == -RANDOM_C;
    array[i] = (RANDOM_A * (seed) + i*RANDOM_C) & 0x7FFFFFFF;

}



int main()
{
    // Initialize the vectors
    const int arraySize =128;
    const int a[arraySize] = { 1, 2, 3, 4, 5 };
    const int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 1 };

    // Add vectors in parallel. addWithCuda returns a CUDA error code that's passed to the CPU for handling by conventional means
    cudaError_t cudaStatus = runCUDAOperation1(c,arraySize, &CUDA_RAND);

    // Check if CUDA was successful.
    if (cudaStatus != cudaSuccess) {
        // Alert the user that CUDA failed to run
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    // Display the vectors and their sums
//    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
//        c[0], c[1], c[2], c[3], c[4]);
    // Display the vectors and their sums
    for (int i = 0; i < arraySize; i++)
    {
        printf("%d, \n", c[i]);
    }

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

// Helper function for using CUDA to operate with three vectors - two input, one output
cudaError_t runCUDAOperation3(const int *a, const int *b, int *c, unsigned int size, KernelFunction3 kernel)
{
    // set them all to nullptr
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }



    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    kernel<<<1, size>>>(dev_a, dev_b, dev_c);

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
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
    
    return cudaStatus;
}











cudaError_t runCUDAOperation1(int *a , unsigned int size, KernelFunction1 kernel)
{
    // set them all to nullptr
    int* dev_a = 0;
    time_t now;
    time(&now);

    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .


    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }




    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }


    // Launch a kernel on the GPU with one thread for each element.
    kernel << <1, size >> > (dev_a, now);

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
    cudaStatus = cudaMemcpy(a, dev_a, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_a);


    return cudaStatus;
}

*/
#pragma once
#define SDL_MAIN_HANDLED



//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
#include "Engine.h"
#include "shader.h"

//#include "Drawable.h"

int main()
{


    // Get the game instance
    static Engine& game = Engine::getInstance();

    Geometry triangle = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
    Component component (&triangle);
    Prop p;
    p.addComponent(&component);
    game.addProp(&p);


    // Check if the game instance is valid
    if (&game != nullptr)
    {
        std::cout << "Game initialized successfully." << std::endl;

         //Run the game loop
        
        game.mainLoop();
    }
    else
    {
        std::cout << "Failed to initialize game." << std::endl;
    }
    

    return 0;
}




/*
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>

bool initSDL(SDL_Window*& window, SDL_GLContext& context, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return false;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cerr << "Warning: Unable to set VSync! SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cerr << "Error initializing GLEW! " << glewGetErrorString(glewError) << std::endl;
        return false;
    }

    return true;
}

void closeSDL(SDL_Window* window, SDL_GLContext context) {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_GLContext context;

    const int width = 800;
    const int height = 600;

    if (!initSDL(window, context, width, height)) {
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the screen with a specific color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // black
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap the window buffers to display the clear color
        SDL_GL_SwapWindow(window);
    }

    closeSDL(window, context);

    return 0;
}
*/