#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>


int main(void) {
	int i, j;
	cl_platform_id *platforms = NULL;
	cl_device_id *devices = NULL;
	cl_uint numplatforms;
	cl_uint numdevices;
	cl_int err;
	char entity_name[1024];
	char entity_vendor[1024];


	err = clGetPlatformIDs(0, 0, &numplatforms);
	if(err) {
		printf("Error getting platforms count. %d\n", err);
		exit(1);
	}

	if(numplatforms == 0) {
		printf("No platforms here.\n");
		exit(1);
	}

	platforms = (cl_platform_id *)malloc(numplatforms * sizeof(cl_platform_id));
	if(!platforms) {
		printf("Cannot allocate platforms IDs memory.\n");
		exit(1);
	}

	err = clGetPlatformIDs(numplatforms, platforms, NULL);
	if(err) {
		printf("Failed to get platforms IDs. %d\n", err);
		free(platforms);
		exit(1);
	}

	for(i = 0; i < numplatforms; i++) {
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 1024, entity_name, NULL);
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 1024, entity_vendor, NULL);

		printf("Platform %d/%d name %s, vendor %s.\n", i, numplatforms, entity_name, entity_vendor);

		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0,  NULL, &numdevices);
		if(err) {
			printf("Faild to get devices count in  platform %d. %d\n", i, err);
			free(platforms);
			exit(1);
		}
		
		if(0 == numdevices) {
			printf("No devices found in platform %d.\n", i);
			free(platforms);
			exit(0);
		}

		devices = (cl_device_id *)malloc(numdevices * sizeof(cl_device_id));
		if(!devices) {
			printf("Cannot allocate device ids memory.\n");
			free(platforms);
			exit(1);
		}

		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, numdevices, devices, NULL);
		if(err) {
			printf("Failed to get platform %d devices ids. %d\n", i, err);
			free(platforms);
			free(devices);
			exit(1);
		}

		for(j = 0; j < numdevices; j++) {
			cl_uint cores = 0u;
			err = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 1024, entity_name, NULL);
			err = clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, 1024, entity_vendor, NULL);
			err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, 4, &cores, NULL);

			printf("Platform %d/%d device %d/%d name %s, vendor %s, cores %d.\n", i, numplatforms, j, numdevices, entity_name, entity_vendor, cores);
		} /* end of devices traversal */
	} /* end of platforms traversal */

	cl_context_properties ctx_prop;
	cl_context ctx = clCreateContext(ctx_prop, numdevices, devices, NULL, NULL, &err);

	/* we chose device 1 - epiphany */
	//cl_command_queue cmd_q0 = clCreateCommandQueue(ctx, devices[0], NULL, &err);
	cl_command_queue cmd_q1 = clCreateCommandQueue(ctx, devices[1], NULL, &err);
	
	cl_mem a_mem_buff = clCreateBuffer(ctx, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &err);
	/* clean */
	free(devices);
	free(platforms);

	return 0;
	
}

