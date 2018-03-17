/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *   This example reads hyperslab from the SDS.h5 file
 *   created by h5_write.c program into two-dimensional
 *   plane of the three-dimensional array.
 *   Information about dataset in the SDS.h5 file is obtained.
 */


#include "hdf5.h"
#include "../vol_posix.h"
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

#define H5FILE_NAME     "SDS.h5"
#define DATASETNAME	"IntArray"

//#define NX_SUB  3           /* hyperslab dimensions */
//#define NY_SUB  4

#define RANK         2
#define RANK_OUT     2

int
main (int argc, char **argv)
{

	 hid_t vol_id, acc_tpl;
	 const H5VL_class_t *posix_vol_plugin;
	 acc_tpl = H5P_DEFAULT;
	 
#if 1

	 posix_vol_plugin = H5VL_posix_init();
	 vol_id = H5VLregister(posix_vol_plugin);
	 acc_tpl = H5Pcreate(H5P_FILE_ACCESS);
	 H5Pset_vol(acc_tpl, vol_id, NULL);
#endif

	hid_t       file, dataset;	/* handles */
	hid_t       datatype, dataspace;
	H5T_class_t t_class;		/* data type class */
	H5T_order_t order;		/* data order */
	size_t      size;		/* size of the data element stored in file */
	hsize_t     dimsm[2];		/* memory space dimensions */
	hsize_t     dims_out[2];	/* dataset dimensions */
	herr_t      status;

	// int         data_out[NX][NY][NZ ]; /* output buffer */

	hsize_t      count[2];              /* size of the hyperslab in the file */
	hsize_t      offset[2];             /* hyperslab offset in the file */
	hsize_t      count_out[2];          /* size of the hyperslab in memory */
	hsize_t      offset_out[2];         /* hyperslab offset in memory */
	int          i, j, k, status_n, rank;
	int	     *data_out;
	int 	     NX, NY;
	int 	     NX_SUB, NY_SUB;
	
	NX = atoi(argv[1]);
	NY = atoi(argv[2]);
	
	data_out = (int *) malloc(sizeof(int) * NX * NY);
	for (j = 0; j < NX; j++) {
		for (i = 0; i < NY; i++) {
			data_out[j*NY + i] = 0;
		}
    	}

	struct  timeval    tv;
	struct  timezone   tz;
	gettimeofday(&tv,&tz);
	
	/*
	 * Open the file and the dataset.
	 */
	file = H5Fopen(H5FILE_NAME, H5F_ACC_RDONLY, acc_tpl);
	printf("Done opening file\n");
	dataset = H5Dopen2(file, DATASETNAME, H5Pcreate(H5P_DATASET_ACCESS));
	printf("Done opening dataset\n");

	/*
	 * Get datatype and dataspace handles and then query
	 * dataset class, order, size, rank and dimensions.
	 */
	datatype  = H5Dget_type(dataset);     /* datatype handle */
	t_class     = H5Tget_class(datatype);
	if (t_class == H5T_INTEGER) printf("Data set has INTEGER type \n");
	order     = H5Tget_order(datatype);
	if (order == H5T_ORDER_LE) printf("Little endian order \n");

	size  = H5Tget_size(datatype);
	printf(" Data size is %d \n", (int)size);

	dataspace = H5Dget_space(dataset);    /* dataspace handle */
	rank      = H5Sget_simple_extent_ndims(dataspace);
	status_n  = H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
	printf("rank %d, dimensions %lu x %lu \n", rank,
	   (unsigned long)(dims_out[0]), (unsigned long)(dims_out[1]));

	/*
	 * Read data from hyperslab in the file into the hyperslab in
	 * memory and display.
	 */
	status = H5Dread(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
		     H5P_DEFAULT, data_out);

	for (j = 0; j < NX; j++) {
		for (i = 0; i < NY; i++) {
			printf("%d ", data_out[j*NY + i]);
		}
		printf("\n");
    	}

    	/*
     	 * Close/release resources.
     	 */
	H5Tclose(datatype);
	H5Dclose(dataset);
	H5Sclose(dataspace);
	H5Fclose(file);

    
	struct  timeval    tv2;
	struct  timezone   tz2;
	gettimeofday(&tv2,&tz2);
	
	printf("Time: %ld\n", (tv2.tv_sec - tv.tv_sec)*1000 + (tv2.tv_usec - tv.tv_usec));
	
	return 0;
}
