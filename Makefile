CC=gcc

H5PART_DIR=/home/saksham/work/cmu/hdf5/deltafs_vol/h5part
# Vol already part of HDF5
HDF5_DIR=/home/saksham/work/cmu/hdf5/deltafs_vol/hdf5/
DELTAFS_DIR=/home/saksham/work/cmu/hdf5/deltafs/src/deltafs-umbrella-build/deltafs-prefix/src/

H5PART_H_DIR=$(H5PART_DIR)/src/
HDF5_H_DIR=$(HDF5_DIR)/src/
DELTAFS_H_DIR=$(DELTAFS_DIR)/deltafs/include/deltafs

H5PART_LIB_DIR=$(H5PART_DIR)/src/.libs
HDF5_LIB_DIR=$(HDF5_DIR)/src/.libs
DELTAFS_LIB_DIR=$(DELTAFS_DIR)/deltafs-build/src/libdeltafs/

CFLAGS=-Wall -Werror -I$(HDF5_H_DIR) -I$(H5PART_H_DIR) -I$(DELTAFS_H_DIR) -g3
LDFLAGS=-L$(H5PART_LIB_DIR) -lH5Part -L$(HDF5_LIB_DIR) -lhdf5 -L$(DELTAFS_LIB_DIR) -ldeltafs -lz -lsz

DELTA_ENV=env DELTAFS_MetadataSrvAddrs="127.0.0.1:10101" DELTAFS_NumOfMetadataSrvs="1"

all: h5_write

run_vol_write: h5_write
	LD_LIBRARY_PATH=$(HDF5_LIB_DIR):$(H5PART_LIB_DIR):$(DELTAFS_LIB_DIR) $(DELTA_ENV) HDF5_DELTAFS_ENABLE=1 ./h5_write

run_write: h5_write
	LD_LIBRARY_PATH=$(HDF5_LIB_DIR):$(H5PART_LIB_DIR):$(DELTAFS_LIB_DIR) $(DELTA_ENV) ./h5_write

h5_write: h5_write.c
	$(CC) $(CFLAGS) -o h5_write h5_write.c $(LDFLAGS)

clean:
	rm -f h5_write *\.h5*
