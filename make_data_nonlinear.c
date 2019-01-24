#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(void){

  int i,j,k;
  int num_frames = 250;
  int num_classes = 2;
  int num_dim = 2;
  int num_test_data = 200;
  float data[num_dim];
  float test_data[num_dim+1];
  float bias_x[2] = {0.5, 1.4};
  float bias_y[2] = {0.8, 2.0};
  float r_max = 0.44;

  float r,theta;
  float x,y;
  FILE *fout;
  char *out[2] = {"class1.dat", "class2.dat"};
  char *test_file = "test.dat";

  srand(time(NULL));

  if(num_frames % 2 != 0){
    num_frames++;
  }

  if(num_test_data % 2 != 0){
    num_test_data++;
  }


  /*-----------input Data-----------*/
  for(k = 0; k < num_classes; k++){
    if((fout = fopen(out[k], "w")) == NULL){
      fprintf(stderr, "cannnot open file %s", out[k]);
    }

    //distribute data
    for(i=0;i<2;i++){
      for(j=0;j<(num_frames/2);j++){
        r = r_max * (double)rand()/RAND_MAX ;//* (rand() % 3);
        theta = 2 * M_PI * (double)rand()/RAND_MAX;
        if(k == 0){
          x = r * cos(theta) + bias_x[i];
          y = r * sin(theta) + bias_y[i];
        }else{
          if(i == 0){
            x = r * cos(theta) + bias_x[0];
            y = r * sin(theta) + bias_y[1];
          }else{
            x = r * cos(theta) + bias_x[1];
            y = r * sin(theta) + bias_y[0];
          }
        }
        data[0] = x;
        data[1] = y;

        //printf("%f %f\n", data[0], data[1]);
        fwrite(data, sizeof(float),2,fout);
      }
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
    for(i=0;i<2;i++){
      for(j=0;j<(num_test_data/2);j++){
        r = r_max * (double)rand()/RAND_MAX ;//* (rand() % 3);
        theta = 2 * M_PI * (double)rand()/RAND_MAX;
        if(k == 0){
          x = r * cos(theta) + bias_x[i];
          y = r * sin(theta) + bias_y[i];
        }else{
          if(i == 0){
            x = r * cos(theta) + bias_x[0];
            y = r * sin(theta) + bias_y[1];
          }else{
            x = r * cos(theta) + bias_x[1];
            y = r * sin(theta) + bias_y[0];
          }
        }
        test_data[0] = x;
        test_data[1] = y;
        test_data[2] = k;

        //printf("%f %f %f\n", test_data[0], test_data[1], test_data[2]);
        fwrite(test_data, sizeof(float),3,fout);
      }
    }
  }
  fclose(fout);
  return 0;
}
