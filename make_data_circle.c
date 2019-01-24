#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(void){

  int i,k;
  int num_frames = 250;
  int num_classes = 2;
  int num_dim = 2;
  int num_test_data = 200;
  float data[num_dim];
  float test_data[num_dim+1];
  //float bias_x[2] = {0.5, 1.4};
  //float bias_y[2] = {0.8, 2.0};
  float r_range = 2;
  float r_min = 3;

  float r_max = 2.5;

  float r,theta;
  float x,y;
  FILE *fout;
  char *out[2] = {"class1.dat", "class2.dat"};
  char *test_file = "test.dat";

  srand(time(NULL));


  /*-----------tnput Data-----------*/
  for(k = 0; k < num_classes; k++){
    if((fout = fopen(out[k], "w")) == NULL){
      fprintf(stderr, "cannnot open file %s", out[k]);
    }

    //distribute data
    for(i=0;i<num_frames;i++){
      if(k == 0){
        r = r_range * (double)rand()/RAND_MAX + r_min ;//* (rand() % 2);
      }else{
        r = r_max * (double)rand()/RAND_MAX;
      }
      theta = 2 * M_PI * (double)rand()/RAND_MAX;
      x = r * cos(theta);
      y = r * sin(theta);

      data[0] = x;
      data[1] = y;

      //printf("%f %f\n", data[0], data[1]);
      fwrite(data, sizeof(float),2,fout);
    }
    fclose(fout);

  }

  /*-----------Test Data-----------*/
  if((fout = fopen(test_file, "w")) == NULL){
    fprintf(stderr, "cannnot open file %s", out[k]);
  }else{
      fout = freopen(test_file, "a", fout);
  }

  for(k = 0; k < num_classes; k++){
    for(i=0;i<num_test_data;i++){
      if(k == 0){
        r = r_range * (double)rand()/RAND_MAX + r_min ;//* (rand() % 2);
      }else{
        r = r_max * (double)rand()/RAND_MAX;
      }
      theta = 2 * M_PI * (double)rand()/RAND_MAX;

      x = r * cos(theta);
      y = r * sin(theta);

      test_data[0] = x;
      test_data[1] = y;
      test_data[2] = (float)k;

      //printf("%f %f %f\n", test_data[0], test_data[1], test_data[2]);
      fwrite(test_data,sizeof(float),3,fout);
    }
  }
  fclose(fout);
  return 0;
}
