#include <stdlib.h>
#include <stdio.h>

#include "../CLManager.h"

#define BUFFER_SIZE     8

#define EXIT_PLATFORM   -1
#define EXIT_DEVICE     -2
#define EXIT_SUM        -3

int main(int argc, char * argv[])
{
    argc--;
    argv++;

    char buffer[BUFFER_SIZE] = {0};

    CLPlatform platform = NULL;
    CLDevice device;

    CLInt argPlatform = -1;
    CLInt argDevice   = -1;
    
    size_t argi = 0;
    if (argc > 0) argPlatform = (CLInt)strtol(argv[argi++], NULL, 10);
    if (argc > 1) argDevice   = (CLInt)strtol(argv[argi++], NULL, 10);

    while (argPlatform < 0) {
        CLPrintPlatforms();
        printf("Select Platform: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        argPlatform = (CLInt)strtol(buffer, NULL, 10);
    }

    if ((platform = CLSelectPlatform(argPlatform)) == NULL) {
        fprintf(stderr, "Error in CLSelectPlatform()\n");
        exit(EXIT_PLATFORM);
    }

    while (argDevice < 0) {
        CLPrintDevices(platform);
        printf("Select Device: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        argDevice = (CLInt)strtol(buffer, NULL, 10);
    }

    if ((device = CLSelectDevice(platform, argDevice)) == NULL) {
        fprintf(stderr, "Error in CLSelectDevice()\n");
        exit(EXIT_DEVICE);
    }
    
    CLContext context = CLCreateContext(platform, device);
    CLQueue queue = CLCreateQueue(context, device);
    CLProgram program = CLCreateProgram(context, device, "kernels.ocl");
    CLKernel kernel_vecsum = CLCreateKernel(program, "vecsum");
    CLEvent event_vecsum;

    // Allocate host arrays
    size_t elements = 1024;
    int * A = (int *)calloc(elements, sizeof(int));
    int * B = (int *)calloc(elements, sizeof(int));
    int * C = (int *)calloc(elements, sizeof(int));

    // Initialize A and B host arrays
    for (size_t i = 0; i < elements; ++i) {
        A[i] = i;
        B[i] = elements - i - 1;
    }

    // Allocate device arrays
    CLMem A_d = CLCreateBufferHostVar(context, CL_MEM_READ_ONLY,  elements * sizeof(*A), A, "A_d");
    CLMem B_d = CLCreateBufferHostVar(context, CL_MEM_READ_ONLY,  elements * sizeof(*B), B, "B_d");
    CLMem C_d = CLCreateBuffer(context, CL_MEM_WRITE_ONLY, elements * sizeof(*C), "C_d");

    // Set kernel arguments
    argi = 0;
    CLSetKernelArg(kernel_vecsum, argi++, sizeof(A_d),    &A_d,      "A_d");
    CLSetKernelArg(kernel_vecsum, argi++, sizeof(B_d),    &B_d,      "B_d");
    CLSetKernelArg(kernel_vecsum, argi++, sizeof(C_d),    &C_d,      "C_d");
    CLSetKernelArg(kernel_vecsum, argi++, sizeof(size_t), &elements, "elements");

    //Enqueue kernel execution
    size_t gws = elements;
    CLEnqueueNDRangeKernel(queue, kernel_vecsum, 1, 0, &gws, NULL, 0, NULL, &event_vecsum, "kernel_vecsum");

    // Await for kernel execution and get back the results from device
    clEnqueueReadBuffer(queue, C_d,CL_TRUE, 0, elements * sizeof(*C), C, 1, &event_vecsum, NULL);

    // Await for completion of the command queue
    CLFinish(queue);



    for (size_t i = 0; i < elements; ++i) {
        if (C[i] != A[i] + B[i]) {
            printf("%zu) %d != %d + %d\n", i, C[i], A[i], B[i]);
            exit(EXIT_SUM);
        }
    }

    // Everything is ok
    printf("Successful!\n");

    // Free memory
    CLReleaseMemObject(A_d, "A_d");
    CLReleaseMemObject(B_d, "B_d");
    CLReleaseMemObject(C_d, "C_d");
    free(A);
    free(B);
    free(C);

    // Release all OpenCL data structures
    CLReleaseEvent(event_vecsum, "event_vecsum");
    CLReleaseKernel(kernel_vecsum, "kernel_vecsum");
    CLReleaseProgram(program, "program");
    CLReleaseQueue(queue, "queue");
    CLReleaseContext(context, "context");
    CLReleaseDevice(device, "device");

    return 0;
}
