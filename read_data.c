#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int read_data(float **input, float **teacher, float **test, int num_dim, int num_out_nodes, int num_frames){
  int i,j;
  FILE *fin;
  FILE *fout;
  char *in[2] = {"class1.dat", "class2.dat"};
  char *out = "readed_data.dat";
  char *test_file = "test.dat";
  int data_count = 0;
  int num_test_data = 0;

  int total_frames = num_frames * num_out_nodes;

  //Initialize
  for(i = 0; i < total_frames; i++){
    for(j = 0; j < num_dim+1; j++){
      input[i][j] = 1.0;
    }
  }

  for(i = 0; i < total_frames; i++){
    for(j = 0; j < num_out_nodes+1; j++){
      teacher[i][j] = 0.0;
    }
  }


  //Now 1500 memory devision is allocated to test
  for(i = 0; i < 500 ;i++){
    for(j = 0; j < num_dim+2; j++){
      test[i][j] = 1.0;
    }
  }

  if((fout = fopen(out, "w")) == NULL){
    fprintf(stderr, "Cannnot Open File %s", out);
  }else{
    fout = freopen(out, "a", fout);
  }

  /*-----------Read Input Data-----------*/
  data_count = 0;
  for(i = 1; i <= num_out_nodes; i++){
    if((fin = fopen(in[i-1], "r")) == NULL){
      fprintf(stderr, "Cannnot Open File %s", in[i-1]);
    }

    while(data_count < num_frames * i){
      fread(&input[data_count][1], sizeof(float), 2, fin);
      teacher[data_count][i] = 1.0;

      fprintf(fout,"%f %f\n",input[data_count][1], input[data_count][2]);
      data_count++;
    }
    fprintf(fout,"\n");
    fclose(fin);
  }

  fclose(fout);

  /*-----------Read Test Data-----------*/
  data_count = 0;
  //If test file exists, read test data. ※If error is occered when opening file, it is not detected.
  if((fin = fopen(test_file, "r")) != NULL){ //sizeof(in)/sizeof(in[0]) - 1
    while(1){
      //Memory Reallocation
      if(data_count!=0 && data_count % 500 == 0){
        if((test = (float **)realloc(test, (data_count + 500) * sizeof(float *))) == NULL){
          fprintf(stderr, "Cannot Allocate Memory\n"); exit(-1);
        }

        for(i = 0; i < data_count; i++){
          if((test[i]=(float *)realloc(test[i], sizeof(float)*(num_dim+2))) == NULL){
            fprintf(stderr,"Cannot Allocate Memory.\n"); exit(-1);
          }
        }

        for(i = data_count; i < data_count + 500; i++){
          if((test[i]=(float *)calloc(num_dim+2,sizeof(float))) == NULL){
            fprintf(stderr,"Cannot Allocate Memory.\n"); exit(-1);
          }
        }

        for(i = data_count; i < data_count + 500;i++){
          for(j = 0; j < num_dim+2; j++){
            test[i][j] = 1.0;
          }
        }
      }

    if (fread(&test[data_count][1], sizeof(float), 3, fin) != 3) {
      break;
    }
    //For Debug
    //printf("%d %f %f %f\n", data_count, test[data_count][1], test[data_count][2], test[data_count][3]);
    data_count++;
  }

    fclose(fin);
  }

  num_test_data = data_count;
  fprintf(stdout, "%d\n", num_test_data );

  return num_test_data;

}
