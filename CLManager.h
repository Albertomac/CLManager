//
//  CLManager.h
//
//  Created by Albertomac on 21/02/18.
//  Copyright © 2018 Albertomac. All rights reserved.
//

#ifndef CLManager_h
#define CLManager_h

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#pragma mark Types

#define CLPlatform		cl_platform_id
#define CLDevice		cl_device_id
#define CLContext		cl_context
#define CLQueue			cl_command_queue
#define CLProgram		cl_program
#define CLKernel		cl_kernel
#define CLEvent			cl_event
#define CLMem			cl_mem
#define CLMemFlags		cl_mem_flags
#define CLDeviceType	cl_device_type

#define CLBool			cl_int
#define CLChar			cl_char
#define CLUChar			cl_uchar
#define CLShort			cl_short
#define CLUShort		cl_ushort
#define CLInt			cl_int
#define CLUInt			cl_uint
#define CLLong			cl_long
#define CLULong			cl_ulong
#define CLHalf			cl_half
#define CLFloat			cl_float
#define CLDouble		cl_double

#define CLString		char *
#define CLStringConst	const CLString
#define CLSize			size_t
#define CLSSize			ssize_t

#define CLTrue 			CL_TRUE
#define CLFalse 		CL_FALSE

#define CHECK_EXIT		CLTrue
#define CHECK_NOT_EXIT	CLFalse


#pragma mark Print

void CLPrintPlatforms();

void CLPrintDevices(CLPlatform platform);

void CLPrintStats(CLEvent start,
				  CLEvent finish,
				  CLUInt operations,
				  CLStringConst name);

CLDouble timeBetweenEventsNS(CLEvent start,
							 CLEvent finish);

CLDouble timeBetweenEventsMS(CLEvent start,
							 CLEvent finish);

CLDouble timeBetweenEventsS(CLEvent start,
							CLEvent finish);


#pragma mark Select

CLPlatform CLSelectPlatform(CLInt platformIndex);

CLDevice CLSelectDevice(CLPlatform platform,
						CLInt deviceIndex);


#pragma mark Create

CLContext CLCreateContext(CLPlatform platform,
						  CLDevice device);

CLQueue CLCreateQueue(CLContext context,
					  CLDevice device);

CLProgram CLCreateProgram(CLContext context,
						  CLDevice device,
						  CLStringConst fileName);

CLProgram CLCreateProgramWithMacro(CLContext context,
								   CLDevice device,
								   CLStringConst fileName,
								   CLStringConst macro);

CLKernel CLCreateKernel(CLProgram program,
						CLStringConst name);

CLMem CLCreateBufferHostVar(CLContext context,
							CLMemFlags flags,
							CLSize size,
							void * hostVar,
							CLStringConst name);

CLMem CLCreateBuffer(CLContext context,
					 CLMemFlags flags,
					 CLSize size,
					 CLStringConst name);

CLMem CLCreateSubBuffer(CLMem mem,
						CLMemFlags flags,
						CLSize offset,
						CLSize size,
						CLStringConst name);


#pragma mark Kernel

void CLSetKernelArg(CLKernel kernel,
					CLUInt index,
					CLSize size,
					const void * arg,
					CLStringConst name);

void CLEnqueueNDRangeKernel(CLQueue queue,
							CLKernel kernel,
							const CLInt workDim,
							const CLSize * globalWorkOffset,
							const CLSize * globalWorkSize,
							const CLSize * localWorkSize,
							CLUInt numberOfEventsWaitList,
							const CLEvent * eventsWaitList,
							CLEvent * event,
							CLStringConst name);

CLSize CLGetPreferredWorkGroupSizeMultiple(CLKernel kernel,
										   CLDevice device,
										   CLStringConst name);

CLSize CLGetOptimalGlobalWorkItemsSize(CLSize numberOfElements,
									   CLSize localWorkSize);


#pragma mark Queue

void CLWaitForEvent(CLEvent * event,
					CLStringConst name);

void * CLEnqueueReadBufferWithEvent(CLQueue queue,
									CLMem mem,
									CLSize size,
									CLEvent * event,
									CLStringConst name);

void * CLEnqueueReadBuffer(CLQueue queue,
						   CLMem mem,
						   CLSize size,
						   CLStringConst name);


#pragma mark Finish and Relases

void CLFinish(CLQueue queue);

void CLReleaseDevice(CLDevice device,
					 CLStringConst name);

void CLReleaseContext(CLContext context,
					  CLStringConst name);

void CLReleaseQueue(CLQueue queue,
					CLStringConst name);

void CLReleaseProgram(CLProgram program,
					  CLStringConst name);

void CLReleaseKernel(CLKernel kernel,
					 CLStringConst name);

void CLReleaseMemObject(CLMem var,
						CLStringConst name);

void CLReleaseEvent(CLEvent event,
					CLStringConst name);

void CLReleaseEvents(CLUInt numberOfEvents,
					 CLEvent * events);


#pragma mark Stuff

CLSize roundUpSize(CLSize elements,
				   CLSize multiple);

CLUInt roundUpUInt(CLUInt elements,
				   CLUInt multiple);

CLSize divUpSize(CLSize elements,
				 CLSize divider);

CLUInt divUpUInt(CLUInt elements,
				 CLUInt divider);

CLDeviceType CLGetDeviceType(CLDevice device);


#pragma mark Error

void CLErrorCheck(CLInt error,
				  CLStringConst function,
				  CLStringConst message,
				  CLInt needExit);

CLString getErrorName(CLInt errorCode);

#endif
