#include <stdio.h>
#include <stdlib.h>

#include "lenet_cnn_float.h"

float sumProduct( float imgPart[CONV1_DIM][CONV1_DIM], float filter[CONV1_DIM][CONV1_DIM]){
  short y,x;
  float conv_result=0;

  for(y = 0; y < CONV1_DIM; y++){
      for(x = 0; x < CONV1_DIM; x++){
        conv_result+=imgPart[y][x]*filter[y][x];
      }
  }
  return conv_result;
}

float sumProductF( short imgPart[CONV1_DIM][CONV1_DIM], short filter[CONV1_DIM][CONV1_DIM]){
  short y,x;
  int conv_result=0;

  for(y = 0; y < CONV1_DIM; y++){
      for(x = 0; x < CONV1_DIM; x++){
        conv_result+=imgPart[y][x]*filter[y][x];
      }
  }
  return conv_result >> FIXED_POINT;
}

void Conv1_28x28x1_5x5x20_1_0(  unsigned char input[IMG_DEPTH][IMG_HEIGHT][IMG_WIDTH], 	                // IN [1][28][28]
				                short kernel[CONV1_NBOUTPUT][IMG_DEPTH][CONV1_DIM][CONV1_DIM], 	// IN [20][1][5][5]
				                short bias[CONV1_NBOUTPUT],						                // IN [20]
				                short output[CONV1_NBOUTPUT][CONV1_HEIGHT][CONV1_WIDTH])        // OUT [20][24][24]
{
  unsigned short o,h,w,x,y;
  short imgPart[CONV1_DIM][CONV1_DIM];
  short conv_px;
  
  for(o = 0; o < CONV1_NBOUTPUT; o++){
    for(h = 0; h < CONV1_HEIGHT; h++){
      for(w = 0; w < CONV1_WIDTH; w++){
        //prepare 5X5 imgPart for the convolution
        for(y = 0; y < CONV1_DIM; y++){
          for(x = 0; x < CONV1_DIM; x++){
            imgPart[y][x]=input[0][h+y][w+x];
          }
        }
        conv_px=sumProductF(imgPart,kernel[o][0]);
        
        //neuron activation
        if(conv_px+bias[o]<=0){
          output[o][h][w]=0;
        }else{
          output[o][h][w]=conv_px + (float)bias[o];
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
  short f,d,o,h,w,x,y,oh,ow;
  short imgPart[CONV2_DIM][CONV2_DIM];
  short conv_px;
  
  for(f=0;f<CONV2_NBOUTPUT;f++){
    for(d=0;d<POOL1_NBOUTPUT;d++){

      for(h=0;h<CONV2_HEIGHT;h++){
        for(w=0;w<CONV2_WIDTH;w++){
          
          for(y = 0; y < CONV2_DIM; y++){ //prepare 5X5 imgPart for the convolution
            for(x = 0; x < CONV2_DIM; x++){
              imgPart[y][x]=input[d][y+h][x+w];
            }
          }

          conv_px=sumProductF(imgPart,kernel[f][d]);

          if(d==0){ //to initialize first element
            output[f][h][w] = conv_px;
          }else{
            output[f][h][w]+= conv_px;
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