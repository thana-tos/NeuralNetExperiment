#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void initialize_weight(float **W1, float **W2, int num_dim, int num_second_nodes, int num_out_nodes){

  int i,j;
  float mean;

  /*-----------Set Initial Weight-----------*/
  //Allocate value to W1 & Compute mean of W1
  mean = 0.0;
  for(i=0;i<num_dim+1;i++){
    for(j=0;j<num_second_nodes+1;j++){
      W1[i][j] = 0.1*(double)rand()/RAND_MAX;
      mean += W1[i][j];
    }
  }
  mean = mean/((num_dim+1)*(num_second_nodes+1));

  //distribute W1 following Gauss Distribution
  for(i=0;i<num_dim+1;i++){
    for(j=0;j<num_second_nodes+1;j++){
      W1[i][j] = W1[i][j] - mean;
    }
  }

  //Allocate value to W2 & Compute mean of W2
  mean = 0.0;
  for(i=0;i<num_second_nodes+1;i++){
    for(j=0;j<num_out_nodes+1;j++){
      W2[i][j] = 0.1*(double)rand()/RAND_MAX;
      mean += W2[i][j];
    }
  }
  mean = mean/((num_second_nodes+1)*(num_out_nodes+1));

  //distribute W2 following Gauss Distribution
  for(i=0;i<num_second_nodes+1;i++){
    for(j=0;j<num_out_nodes+1;j++){
      W2[i][j] = W2[i][j] - mean;
    }
  }
}
