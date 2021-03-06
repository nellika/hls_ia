/**
  ******************************************************************************
  * @file    pool.c
  * @author  Chaitanya Devidas Gore, Bogdan Mihai Nistor, Nelli Nyisztor, Université Côte d'Azur, France
  * @version V3.0
  * @date    07 september 2020
  * @brief   Pooling layers for modified LeNet5
  */

#include <stdio.h>
#include <stdlib.h>

#include "lenet_cnn_float.h"

short maxPooling(short poolArray[]){
    short max=poolArray[0];
    unsigned short p;

    for (p = 1; p < POOL1_DIM*POOL1_DIM; p++){
        if(poolArray[p] > max){
          max = poolArray[p];
        }
    }
    return max;
}

void Pool1_24x24x20_2x2x20_2_0( short input[CONV1_NBOUTPUT][CONV1_HEIGHT][CONV1_WIDTH],   // IN
                                short output[POOL1_NBOUTPUT][POOL1_HEIGHT][POOL1_WIDTH])  // OUT
{
    unsigned short i,h,w,out_h,out_w;
    short maxPool;

    for (i = 0; i < CONV1_NBOUTPUT; i++){   // 20
      out_h=0;
      for (h = 0; h < CONV1_HEIGHT; h+=2){  // 20*12 > 240
        out_w=0;
        for (w = 0; w < CONV1_WIDTH; w+=2){ // 20*12*12 > 2880 iterations
            // select max from 2x2 matrix
            maxPool=maxPooling((short []){input[i][h][w],input[i][h+1][w],input[i][h][w+1],input[i][h+1][w+1]});
            output[i][out_h][out_w]=maxPool;
            out_w+=1;
        }
        out_h+=1;
      }
    }
}

void Pool2_8x8x40_2x2x40_2_0( short input[CONV2_NBOUTPUT][CONV2_HEIGHT][CONV2_WIDTH],   // IN
                              short output[POOL2_NBOUTPUT][POOL2_HEIGHT][POOL2_WIDTH])  // OUT
{   
    unsigned short j,h,w,out_w;
    short out_h;
    short maxPool;
    out_h=0;
    out_w=0;

    for (j = 0; j < CONV2_NBOUTPUT; j++){   // 40
      out_h=0;
      for (h = 0; h < CONV2_HEIGHT; h+=2){  // 40*4 > 160
        out_w=0;
        for (w = 0; w < CONV2_WIDTH; w+=2){ // 40*4*4 > 640 iterations
            // select max from 2x2 matrix
            maxPool=maxPooling((short []){input[j][h][w],input[j][h+1][w],input[j][h][w+1],input[j][h+1][w+1]});
            output[j][out_h][out_w]=maxPool;

            out_w+=1;
        }
        out_h+=1;
      }
    }
}