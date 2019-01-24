#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
	Included Function(constructed procedually)
		forward_propagation
		back_propagation
		weight_update
		output_result
		test
		neural_net(Core, It calls functions above)

  Reading neural_net function, You can understand this program.
**/

//AdaGradのような学習率操作（プログラム中では用いていない)
double step_decay(double initial_rho, int epoch){
  double drop = 0.99;
  int interval = 100;
  double rho = initial_rho * pow(drop, floor((1+epoch)/interval));
  return rho;
}

//forward_propagation
double forward_propagation(p, input, hidden, output, teacher ,W1, W2, num_dim, num_out_nodes, num_second_nodes)
int p;
float **input;
float *hidden;
float **output;
float **teacher;
float **W1;
float **W2;
int num_dim;
int num_out_nodes;
int num_second_nodes;
{
	int i,j;
	//誤差関数値
	double error = 0.0;
  float sum;

	hidden[0] = 1.0; //bias

	for (j = 1; j < num_second_nodes+1; j++) {
		sum = 0.0;
		for (i = 0; i < num_dim+1; i++) {
			sum += input[p][i] * W1[i][j];
		}
		hidden[j] = 1.0 / (1.0 + exp(-sum)); // sigmoid function
	}

	// Compute output-unit activations and errors.
	for (j = 1; j < num_out_nodes+1; j++) {
		sum = 0.0;
		for (i = 0; i < num_second_nodes+1; i++) {
			sum += hidden[i] * W2[i][j];
		}
		output[p][j] = 1.0 / (1.0 + exp(-sum)); // sigmoid function
		error += (1/2.0) * pow(teacher[p][j] - output[p][j],2);
	}

	return error;
}

//back_propagation
void back_propagation(p, delta_hidden, delta_output, hidden, output, teacher ,W1, W2, num_out_nodes, num_second_nodes)
int p;
float *delta_hidden;
float *delta_output;
float *hidden;
float **output;
float **teacher;
float **W1;
float **W2;
int num_out_nodes;
int num_second_nodes;
{
	int j,k;
	//中間層でのe_{jp}式中の\sum_k e_{pk}w_{jk}
	float sum_from_output;

	//Compute e_{pk}
	for (j = 1; j < num_out_nodes+1; j++){
		delta_output[j] = (output[p][j] - teacher[p][j]) * output[p][j] * (1.0 - output[p][j]);
	}

	// Compute e_{pj}
	for (j = 1; j < num_second_nodes+1; j++) {
		sum_from_output = 0.0;
		for (k = 1; k < num_out_nodes+1; k++) {
			//compute /sum_k e_{pk} * w_{jk}
			sum_from_output += delta_output[k] * W2[j][k];
		}
		delta_hidden[j] = sum_from_output * hidden[j] * (1.0 - hidden[j]);
	}

}

//Update weight
void weight_update(p, delta_hidden, delta_output, input, hidden ,W1, W2, num_dim, num_out_nodes, num_second_nodes, rho)
int p;
float *delta_hidden;
float *delta_output;
float **input;
float *hidden;
float **W1;
float **W2;
int num_dim;
int num_out_nodes;
int num_second_nodes;
float rho;
{

	int i,j;
	float delta;

	//  Update weights between input and hidden layer.
	for (j = 1; j < num_second_nodes+1; j++) {
    delta = 0.0;
		for (i = 0; i < num_dim+1; i++) {
			//Compute rho * e_{pj} * input[p][i] (更新量)
			delta = rho * delta_hidden[j] * input[p][i];
			//Update weight				　
			W1[i][j] -= delta;
		}
	}

	// Update weights between hidden and output layer
	for (j = 1; j < num_out_nodes+1; j++) {
    delta = 0.0;
		for (i = 0; i < num_second_nodes+1; i++) {
			//Compute rho * e_{pk} * hidden[p][j] (更新量)
			delta = rho * delta_output[j] * hidden[i];
			//Update weight
			W2[i][j] -= delta;
		}
	}
}

void output_result(input, output, teacher ,W1, W2 ,num_dim, num_second_nodes, num_out_nodes, num_frames)
float **input;
float **output;
float **teacher;
float **W1;
float **W2;
int num_dim;
int num_second_nodes;
int num_out_nodes;
int num_frames;

{
	int i,j,p;
	//結果出力用ファイル
	char *out = "result.dat";
	FILE *fout;

	//総データ数
	int total_frames = num_frames*num_out_nodes;

	// Print inputs and outputs.
	if((fout = fopen(out, "w")) == NULL){
		fprintf(stderr, "cannnot open file %s", out);
	}

  //output Weight
  puts("\n\nW1");
  for(i = 0; i < num_dim + 1; i++){
    for(j = 0; j < num_second_nodes + 1; j++){
      printf("%f ",W1[i][j]);
    }
    putchar('\n');
  }
  puts("W2");
  for(i = 0; i < num_second_nodes + 1; i++){
    for(j = 0; j < num_out_nodes + 1; j++){
      printf("%f ",W2[i][j]);
    }
    putchar('\n');
  }

	// Print inputs and outputs.
	printf("\n\ndata\t  ");
	fprintf(fout, "#data\t  ");
	for (i = 1; i < num_dim+1; i++) {
		printf("input%d\t  ", i);
		fprintf(fout, "input%d\t  ", i);
	}
	for (i = 1; i < num_out_nodes+1 ; i++) {
		printf("teacher%d\t  output%d  \t", i, i);
		fprintf(fout,"teacher%d\t  output%d  \t", i, i);
	}
	for (p = 0; p < total_frames; p++) {
		printf("\n%d\t  ", p+1);
		fprintf(fout,"\n%d\t  ", p+1);

		for (i = 1; i < num_dim+1; i++) {
			printf("%f\t  ", input[p][i]);
			fprintf(fout,"%f\t  ", input[p][i]);
		}
		for (i = 1; i < num_out_nodes+1; i++) {
			printf("%f\t  %f\t  ", teacher[p][i], output[p][i]);
			fprintf(fout, "%f\t  %f\t  ", teacher[p][i], output[p][i]);
		}
	}
	printf("\n\n");

}

//Test constructed model by accuracy rate
void test_model(test, hidden, output, teacher ,W1, W2, num_dim, num_out_nodes, num_second_nodes, num_test_data)
float **test;
float *hidden;
float **output;
float **teacher;
float **W1;
float **W2;
int num_dim;
int num_out_nodes;
int num_second_nodes;
int num_test_data;
{
	int p;
	int correct_class;
	int wrong_class;
	int correct_count = 0;
	for (p = 0; p < num_test_data; p++) {

		forward_propagation(p, test, hidden, output, teacher ,W1, W2, num_dim, num_out_nodes, num_second_nodes);
		correct_class = lroundf(test[p][num_dim+1]) + 1;
		wrong_class = (correct_class == 1) ? 2 : 1;
		//For Debug
		//printf("%d %d\n", correct_class, wrong_class);

		if(output[p][correct_class] > output[p][wrong_class]){
			correct_count++;
		}

		printf("%d | correct_class : %f wrong_class out2 : %f \n", p, output[p][correct_class], output[p][wrong_class]);
	}

	printf("Number of test data : %d\n", num_test_data);
	printf("Accuracy Rate : %lf\n\n", correct_count / (double)num_test_data);

}

//Main Solver
void neural_net(input, teacher, test ,W1, W2, num_dim, num_out_nodes, num_frames, num_second_nodes, total_iterations, num_test_data)
float **input;
float **teacher;
float **test;
float **W1;
float **W2;
int num_dim;
int num_out_nodes;
int num_frames;
int num_second_nodes;
int total_iterations;
int num_test_data;
{
	int p, i, iteration;

	/*Error推移出力用ファイル*/
	char *out = "result_error.dat";
	FILE *fout;

	/*隠れ層における出力値*/
	float *hidden;
	/*出力層における出力値*/
	float **output;

	//出力層における, e_{pk}
	float *delta_output;
	//隠れ層における, e_{pj}
	float *delta_hidden;

	//学習率
	float initial_rho = 0.1;
	float rho = initial_rho;

	//微小な値(fabs(total_error - total_error_old) < epsilon)となったら収束したとみなす
	double epsilon = 1e-8;

	//総データ数
	int total_frames = num_frames*num_out_nodes;
  //誤差
	double total_error = 0.0, total_error_old = 999;

	if((fout = fopen(out, "w")) == NULL){
		fprintf(stderr, "cannnot open file %s", out);
	}

	/*-----------Memory Allocation-----------*/
	//allocate memory for hidden
  if((hidden=(float *) calloc(num_second_nodes+1, sizeof(float))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  }

	//allocate memory for output
	if((output=(float **) calloc(total_frames, sizeof(float *))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  }
  for(i=0;i<total_frames;i++){
    if((output[i]=(float *) calloc(num_out_nodes+1, sizeof(float))) == NULL){
      fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
    }
  }

	//allocate memory for delta_hidden
	if((delta_hidden=(float *) calloc(num_second_nodes+1, sizeof(float))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  }

	//allocate memory for delta_output
	if((delta_output=(float *) calloc(num_out_nodes+1, sizeof(float))) == NULL){
    fprintf(stderr,"cannot allocate memory.\n");    exit(-1);
  }

  // Iterate weight updates.
	/*注意 : バイアスニューロンを考えるときはイテレーションの初期値を0, 最終は常に+1*/
	for (iteration = 0; iteration < total_iterations; iteration++) {

		for (p = 0; p < total_frames; p++) {

			/*-----------Transmit Signal Forward-----------*/
			// Compute hidden-unit activations.
			total_error += forward_propagation(p, input, hidden, output, teacher ,W1, W2, num_dim, num_out_nodes, num_second_nodes);

			/*-----------Back Propagation----------*/
      back_propagation(p, delta_hidden, delta_output, hidden, output, teacher ,W1, W2, num_out_nodes, num_second_nodes);

			/*--------------Update weight--------------*/
			weight_update(p, delta_hidden, delta_output, input, hidden ,W1, W2, num_dim, num_out_nodes, num_second_nodes, rho);

			//rho = step_decay(initial_rho, iteration);

		}

		if (iteration % 100 == 0){
			printf("\niteration = %d : total_error = %lf rho = %f", iteration, total_error, rho);
			//output error
			fprintf(fout, "%d %lf\n", iteration, total_error);
		}
		if (fabs(total_error_old - total_error) < epsilon){
		  puts("\nConverged!\n");

			printf("\n\niteration: %d",iteration);
			break;
		}

		total_error_old = total_error;
		total_error = 0.0;
	}

	fclose(fout);

	output_result(input, output, teacher, W1, W2, num_dim, num_second_nodes, num_out_nodes, num_frames);
	if(test != NULL){
	  test_model(test, hidden, output, teacher ,W1, W2, num_dim, num_out_nodes, num_second_nodes, num_test_data);
	}

	free(delta_output);
	free(delta_hidden);
	for(i = 0; i < total_frames; i++){
		free(output[i]);
	}
	free(output);
	free(hidden);

}
