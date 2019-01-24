#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>

extern void initialize_weight(float **W1, float **W2, int num_dim, int num_second_nodes, int out_nodes);
extern int read_data(float **input, float **test, float **teacher, int num_dim, int num_out_nodes, int num_frames);
extern void neural_net(float **input, float **teacher, float **test, float **W1, float **W2, int num_dim, int num_out_nodes, int num_frames, int num_second_nodes,int total_iterations, int num_test_data);


int main(argc,argv)
int argc;
char *argv[];
{
  int i;
  int num_dim, num_out_nodes, num_frames, num_second_nodes;
  int total_iterations, total_frames;
  int num_test_data = 0;
  float **input, **teacher, **test,  **W1, **W2;
  char opt;
  extern char *optarg;

  /*-----------Initialize-----------*/

  for(i=0;i<argc &&(opt=getopt(argc,argv,"a:b:c:s:t:"))
	;i++){
    switch(opt){
    case 'a': num_dim = atoi(optarg);  break;
    case 'b': num_out_nodes = atoi(optarg);   break;
    case 'c': num_frames = atoi(optarg);   break;
    case 's': num_second_nodes = atoi(optarg);   break;
    case 't': total_iterations = atoi(optarg);   break;
    }
  }

  //総データ数
  total_frames = num_frames * num_out_nodes;

  //allocate memory for W1
  if((W1=(float **) calloc(num_dim+1, sizeof(float *))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  }
  for(i=0;i<num_dim+1;i++){
    if((W1[i]=(float *) calloc(num_second_nodes+1, sizeof(float))) == NULL){
      fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
    }
  }

  //allocate memory for W2
  if((W2=(float **) calloc(num_second_nodes+1, sizeof(float *))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  }
  for(i=0;i<num_second_nodes+1;i++){
    if((W2[i]=(float *) calloc(num_out_nodes+1, sizeof(float))) == NULL){
      fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
    }
  }

  //allocate memory for input
  if((input = (float **) calloc(total_frames, sizeof(float *))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  };
  for(i = 0; i < total_frames; i++){
    if((input[i] = (float *)calloc(num_dim+1, sizeof(float))) == NULL){
      fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
    };
  }

  //allocate memory for output
  if((teacher = (float **) calloc(total_frames, sizeof(float *))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  };
  for(i = 0; i < total_frames; i++){
    if((teacher[i] = (float *) calloc(num_out_nodes+1, sizeof(float))) == NULL){
      fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
    };
  }

  //allocate memory for test data
  if((test = (float **) calloc(500, sizeof(float *))) == NULL){
    fprintf(stderr, "Cannot Allocate Memory\n"); exit(-1);
  }
  for(i = 0; i < 500; i++){
    if((test[i]= (float *) calloc(num_dim+2, sizeof(float))) == NULL){
      fprintf(stderr,"Cannot Allocate Memory.\n"); exit(-1);
    }
  }

  //Initialize Weight
  initialize_weight(W1, W2, num_dim, num_second_nodes, num_out_nodes);
  //Read Data
  num_test_data = read_data(input, teacher ,test ,num_dim, num_out_nodes, num_frames);
  //Solve
  if(num_test_data != 0){
    neural_net(input, teacher, test, W1, W2, num_dim, num_out_nodes, num_frames, num_second_nodes, total_iterations, num_test_data);
  }else{
    neural_net(input, teacher, NULL, W1, W2, num_dim, num_out_nodes, num_frames, num_second_nodes, total_iterations, 0);
  }


  for(i = 0; i < total_frames; i++){
    free(input[i]);
  }
  free(input);

  for(i = 0; i < total_frames; i++){
    free(teacher[i]);
  }
  free(teacher);

  int tmp = num_test_data / 500 + 1;
  for(i = 0; i < num_test_data * tmp; i++){
    free(test[i]);
  }
  free(test);

  for(i = 0; i < num_dim+1; i++){
    free(W1[i]);
  }
  free(W1);

  for(i = 0; i < num_second_nodes+1; i++){
    free(W2[i]);
  }
  free(W2);


  return EXIT_SUCCESS;

}
