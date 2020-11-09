#include <stdio.h>
#include <stdlib.h>

#include "lenet_cnn_float.h"

/*
short sumProduct( short imgPart[CONV1_DIM][CONV1_DIM], short filter[CONV1_DIM][CONV1_DIM]){
  unsigned short y,x;
  int conv_result=0;

  for(y = 0; y < CONV1_DIM; y++){
      for(x = 0; x < CONV1_DIM; x++){
        conv_result+=imgPart[y][x]*filter[y][x];
      }
  }
  return conv_result >> FIXED_POINT;
}*/

void Conv1_28x28x1_5x5x20_1_0(  unsigned char input[IMG_DEPTH][IMG_HEIGHT][IMG_WIDTH], 	                // IN [1][28][28]
				                short kernel[CONV1_NBOUTPUT][IMG_DEPTH][CONV1_DIM][CONV1_DIM], 	// IN [20][1][5][5]
				                short bias[CONV1_NBOUTPUT],						                // IN [20]
				                short output[CONV1_NBOUTPUT][CONV1_HEIGHT][CONV1_WIDTH])        // OUT [20][24][24]
{
//#pragma HLS ARRAY_PARTITION variable=input complete dim=3
#pragma HLS RESOURCE variable=bias core=RAM_1P_LUTRAM
#pragma HLS RESOURCE variable=output core=RAM_1P_LUTRAM

  unsigned short o,h,w,x,y;
  int conv_px_sum;

  for(o = 0; o < CONV1_NBOUTPUT; o++){
    for(h = 0; h < CONV1_HEIGHT; h++){
      for(w = 0; w < CONV1_WIDTH; w++){
        //prepare 5X5 imgPart for the convolution
        conv_px_sum = 0;

        for(y = 0; y < CONV1_DIM; y++){
#pragma HLS pipeline
          for(x = 0; x < CONV1_DIM; x++){
            conv_px_sum = conv_px_sum + input[0][h+y][w+x]*kernel[o][0][y][x];
          }
        }
        //conv_px=sumProduct(imgPart,kernel[o][0]);
        //neuron activation
        //<=-100 works with better accuracy, but thats not the point here...

        if(conv_px_sum+bias[o]<=0){
          output[o][h][w]=0;
        }else{
          conv_px_sum = conv_px_sum >> FIXED_POINT;
          output[o][h][w]=conv_px_sum + bias[o];
        }
      }
    }
  }
  
}

void Conv2_12x12x20_5x5x40_1_0( short input[POOL1_NBOUTPUT][POOL1_HEIGHT][POOL1_WIDTH], 	            // IN
				                short kernel[CONV2_NBOUTPUT][POOL1_NBOUTPUT][CONV2_DIM][CONV2_DIM], 	// IN
				                short bias[CONV2_NBOUTPUT], 						                    // IN
				                short output[CONV2_NBOUTPUT][CONV2_HEIGHT][CONV2_WIDTH]) 		        // OUT
{

//#pragma HLS ARRAY_PARTITION variable=input complete dim=3
//#pragma HLS RESOURCE variable=bias core=RAM_1P_LUTRAM
//#pragma HLS RESOURCE variable=output core=RAM_1P_LUTRAM

#pragma HLS INLINE region recursive
#pragma HLS UNROLL factor=4

  unsigned short f,d,h,w,x,y,oh,ow;
  int conv_px_sum;
  
  for(f=0;f<CONV2_NBOUTPUT;f++){
    for(d=0;d<POOL1_NBOUTPUT;d++){

      for(h=0;h<CONV2_HEIGHT;h++){
        for(w=0;w<CONV2_WIDTH;w++){
          conv_px_sum = 0;
#pragma HLS pipeline
          for(y = 0; y < CONV2_DIM; y++){ //prepare 5X5 imgPart for the convolution
            for(x = 0; x < CONV2_DIM; x++){
              //imgPart[y][x]=input[d][y+h][x+w];
#pragma HLS RESOURCE variable=conv_px_sum core=MulnS latency=2
              conv_px_sum = conv_px_sum + input[d][h+y][w+x]*kernel[f][d][y][x];
            }
          }

          //conv_px=sumProduct(imgPart,kernel[f][d]);
          conv_px_sum = conv_px_sum >> FIXED_POINT;

          if(d==0){ //to initialize first element
            output[f][h][w] = conv_px_sum;
          }else{
            output[f][h][w]+= conv_px_sum;
          }
        }
      } 
    }

    for(oh=0;oh<CONV2_HEIGHT;oh++){
      for(ow=0;ow<CONV2_WIDTH;ow++){
        //neuron activation
        if(output[f][oh][ow]+bias[f]<=0){
          output[f][oh][ow]=0;
        }else{
          output[f][oh][ow]=output[f][oh][ow] + bias[f];
        }
      }
    }
  }
}