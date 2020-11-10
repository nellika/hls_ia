/**
  ******************************************************************************
  * @file    fc.c
  * @author  Chaitanya Devidas Gore, Bogdan Mihai Nistor, Nelli Nyisztor, Université Côte d'Azur, France
  * @version V2.0
  * @date    07 september 2020
  * @brief   Fully connected layers for modified LeNet5
  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lenet_cnn_float.h"

/*void Softmax(short vector_in[FC2_NBOUTPUT], int vector_out[FC2_NBOUTPUT]){
  long vector_exp[FC2_NBOUTPUT];
  long exp_sum=0;
  int vector_summy[FC2_NBOUTPUT];
  short bckshifted_input;

  for (short i = 0; i < FC2_NBOUTPUT; i++){
    bckshifted_input=vector_in[i] >> FIXED_POINT;
    vector_exp[i]=exp(bckshifted_input);
    exp_sum+=vector_exp[i];
  }
  
  exp_sum = exp_sum >> 4;
  //printf("%lu\n",exp_sum);
  for(short j = 0; j < FC2_NBOUTPUT; j++){
    vector_out[j]=vector_exp[j]/exp_sum;
    //vector_out[j]=0;
  }
  //printf("\n");
}*/


void Softmax(short vector_in[FC2_NBOUTPUT], float vector_out[FC2_NBOUTPUT]){
  float vector_exp[FC2_NBOUTPUT];
  float exp_sum=0;
  short bckshifted_input;

  for (short i = 0; i < FC2_NBOUTPUT; i++){
    bckshifted_input=vector_in[i] >> FIXED_POINT;
    vector_exp[i]=exp(bckshifted_input);
    exp_sum+=vector_exp[i];
  }

  for(short j = 0; j < FC2_NBOUTPUT; j++){
    vector_out[j]=vector_exp[j]/exp_sum;
  }
}


void Fc1_40_400(    short 	input[POOL2_NBOUTPUT][POOL2_HEIGHT][POOL2_WIDTH], 			        // IN
			        short 	kernel[FC1_NBOUTPUT][POOL2_NBOUTPUT][POOL2_HEIGHT][POOL2_WIDTH],	// IN
			        short 	bias[FC1_NBOUTPUT],							                        // IN
			        short 	output[FC1_NBOUTPUT]) 							                    // OUT
{
  unsigned short o,d,h,w;
  short fc_sum;
  int temp_sum;

  for(o = 0; o < FC1_NBOUTPUT; o++){
    temp_sum=0;
    for(d=0;d<POOL2_NBOUTPUT;d++){
      for(h = 0; h < POOL2_HEIGHT; h++){
        for(w = 0; w < POOL2_WIDTH; w++){
          temp_sum+=(input[d][h][w]*kernel[o][d][h][w]);
        }
      }
    }
    fc_sum=temp_sum >> FIXED_POINT;
    //neuron activation
    if(fc_sum+bias[o]<=0){
      output[o]=0;
    }else{
      output[o]=fc_sum+bias[o];
    }
  }

}

void Fc2_400_10(	short 	input[FC1_NBOUTPUT], 			        // IN
			        short 	kernel[FC2_NBOUTPUT][FC1_NBOUTPUT],	    // IN
			        short 	bias[FC2_NBOUTPUT],			            // IN
			        short 	output[FC2_NBOUTPUT]) 			        // OUT
{
  unsigned short o,d;
  int temp_sum;
  short fc_sum;

  for(o = 0; o < FC2_NBOUTPUT; o++){
    temp_sum=0;
    for(d=0;d<FC1_NBOUTPUT;d++){
#pragma HLS PIPELINE
      temp_sum+=(input[d]*kernel[o][d]);
    }
    fc_sum=temp_sum >> FIXED_POINT;
    output[o]=fc_sum+bias[o];
    
    //neuron activation
    /*if(fc_sum+bias[o]<=0){
      output[o]=0;
    }else{
      output[o]=fc_sum+bias[o];
    }*/
  }  
}