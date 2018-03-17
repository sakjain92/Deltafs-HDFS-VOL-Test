/* Copied from: http://vis.lbl.gov/Research/H5Part/sample.h5part.c */
#include "H5Part.h"
#include "H5Block.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

H5PartFile* file;
int numparticles = 1000;
int x_dim = 64;
int y_dim = 64; 
int z_dim = 64;

static inline double uniform_random_number() {
  return (((double)rand())/((double)(RAND_MAX)));
}

void create_synthetic_h5part_data() {
  printf("Writing out sample particle data");
  double *x, *y, *z;
  int i;

  x=(double*)malloc(numparticles*sizeof(double));
  y=(double*)malloc(numparticles*sizeof(double));
  z=(double*)malloc(numparticles*sizeof(double));
  
  h5part_int64_t *id;
  id=(h5part_int64_t*)malloc(numparticles*sizeof(h5part_int64_t));

  for (i=0; i<numparticles; i++) {
    id[i] = i;
    x[i] = uniform_random_number()*x_dim;
    y[i] = uniform_random_number()*y_dim;
    z[i] = ((double)i/numparticles)*z_dim;    
  }


  for (i = 0; i < 3; i++) {
	H5PartSetStep(file, i);
	H5PartSetNumParticles(file, numparticles);

  	H5PartWriteDataFloat64(file,"x",x); 
  	H5PartWriteDataFloat64(file,"y",y);
  	H5PartWriteDataFloat64(file,"z",z);
  
  	H5PartWriteDataFloat64(file,"px",z); 
  	H5PartWriteDataFloat64(file,"py",z);
  	H5PartWriteDataFloat64(file,"pz",z);
  
  	H5PartWriteDataInt64(file,"id",id);
  }

  free(x); free(y); free(z);
  free(id);
  printf("....\n");
}

void create_synthetic_paraview_data() {
  printf("Writing out sample particle data");
  double *x, *y, *z;
  int i;

  x=(double*)malloc(numparticles*sizeof(double));
  y=(double*)malloc(numparticles*sizeof(double));
  z=(double*)malloc(numparticles*sizeof(double));
  
  h5part_int64_t *id;
  id=(h5part_int64_t*)malloc(numparticles*sizeof(h5part_int64_t));

  for (i=0; i<numparticles; i++) {
    id[i] = i;
    x[i] = uniform_random_number();
    y[i] = uniform_random_number();
    z[i] = ((double)i/numparticles); 
  }


  for (i = 0; i < 3; i++) {
	H5PartSetStep(file, i);
    H5PartSetNumParticles(file, numparticles);
  	H5PartWriteDataFloat64(file,"Coords_0",x); 
    H5PartWriteDataFloat64(file,"Coords_1",y);
    H5PartWriteDataFloat64(file,"Coords_2",z);
  	H5PartWriteDataInt64(file,"tag",id);
  }

  free(x); free(y); free(z);
  free(id);
  printf("....\n");
}


int main(int argc, char* argv[]) {
 
  file = H5PartOpenFile("sample2.h5part", H5PART_READ);
   while(1);
  //H5PartWriteFileAttribString(file, "Origin", "Created by LBNL Visualization group");
  
  //create_synthetic_paraview_data();

  H5PartCloseFile(file);

  printf("Ending now\n");

  return 0;
}
