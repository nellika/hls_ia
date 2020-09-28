#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lenet_cnn_float.h"

void Softmax(float vector_in[FC2_NBOUTPUT], float vector_out[FC2_NBOUTPUT]){
  float vector_exp[FC2_NBOUTPUT];
  float exp_sum=0;

  for (short i = 0; i < FC2_NBOUTPUT; i++){
    vector_exp[i]=exp(vector_in[i]);
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
  short o,d,h,w;
  short fc_sum;
  short temp_sum;

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
			        float 	output[FC2_NBOUTPUT]) 			        // OUT
{
  short o,d;
  float fc_sum;

  for(o = 0; o < FC2_NBOUTPUT; o++){
    fc_sum=0;
    for(d=0;d<FC1_NBOUTPUT;d++){
      fc_sum+=(input[d]*kernel[o][d]);
    }

    //neuron activation
    if(fc_sum+bias[o]<=0){
      output[o]=0;
    }else{
      output[o]=fc_sum+bias[o];
    }
  }  
}