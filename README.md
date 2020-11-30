# Embedded AI on FPGA using High Level Synthesis #
> GSE5 Embedded Systems Project 2020-2021

The goal of the project is the full development of a Convolutional Neural Network (CNN) accelerated on a CPU / FPGA system using High Level Synthesis (HLS).
- Board: Zynq, ZedBoard
- SW: Vivado (for power/energy consumption), Vivado HLS (for hls), SDSoC (for creating image + executables with and without acceleration)

## Tasks performed ##
- [x] Phase1 - Implement LeNet CNN (C)
- [x] Phase2 - Create fixed point implementation
- [x] Phase3 - Removal of hdf5 dependencies
- [x] Phase5 - Add pragmas, decrease latency
- [x] Phase6 - Test solution, validate measurements

## Final measurements ##

**Error: 201 / 10000** <br />
**Success rate: 97.98998%**

### Execution time ###

Version      | Global time  | Local min (clk) | Local max (clk) | Local avg (clk)
| :--- | ---: | ---: | ---: | ---:
SW           | 389s         | 7 413 302       | 16 111 340      |           7 435
HW           | 908s         | 43 946 334      | 48 133 712      |           4 396
HW + pragma  | 310s         | 1 854 234       | 5 928 854       |           1 872

### Power consumption ###

TBD

## Project files and directories ##
**FIXED\_POINT\_NO\_HDF5\_PRAGMA**
* mnist _containing image files_
* weights\_exported _txt files containing exported weights and biases from lenet_weights.hdf5_
  * conv.c _conv1 and conv2 functions_
  * pool.c _pool1 and pool2 functions_
  * fc.c _fc1 and fc2 functions_
  * lenet_cnn_float.c _main lenet_cnn function_
  * lenet_cnn_float.h
  * utils.c _util functions used mainly in lenet_cnn_float.c
  * weights.h _merged weights and biases for global usage - extracted from lenet_weights.hdf5_
  * Makefile
  
**FLOAT**
* mnist _containing image files_
  * conv.c _conv1 and conv2 functions_
  * pool.c _pool1 and pool2 functions_
  * fc.c _fc1 and fc2 functions_
  * lenet_cnn_float.c _main lenet_cnn function_
  * lenet_cnn_float.h
  * lenet_weights.hdf5 _weights and biases in hdf5 format_
  * utils.c _util functions used mainly in lenet_cnn_float.c
  * Makefile _for compilation_

**synthesis_results**
  * synth\_without\_pragma.html _exported synthesis results from vivado hls without pragma usage_
  * synth\_with\_pragma.html _exported synthesis results from vivado hls with pragmas_

**lenet\_keras\_20\_40.py** _python function to train and test the model, obtain weights and biases_


