/*
Define seven-segment display segment regions:
       ------- 
      |   1   |      
 ---   -------   ---
|   |           |   |
| 0 |           | 2 |
|   |           |   |
 ---   -------   ---
      |   3   |      
 ---   -------   ---  
|   |           |   |
| 4 |           | 5 |
|   |           |   |
 ---   -------   ---  
      |   6   |
       -------
"""
*/

bool number_segment[10][7] = {
    // first index is number, second is the segment it have
    {1, 1, 1, 0, 1, 1, 1},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 0, 1},
    {0, 1, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 0, 1, 0},
    {1, 1, 0, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1},
    {0, 1, 1, 0, 0, 1, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1}};

//adaptive threshold values
int const adpt_ts_BINARY_value = 255;

int adpt_ts_adaptiveMethod = 0;
int adpt_ts_thresholdType = 0;
int adpt_ts_blockSize = 1; //here blocksize means half length of diagonal
int adpt_ts_subConstant = 0;

int adpt_ts_max_adaptiveMethod = 1;
int adpt_ts_max_thresholdType = 1;
int adpt_ts_max_blockSize = 50; //change if necessary, sampling size to neigherhoods to determine threshold, calculation needed as the blocksize must be 1+2n where n is a positive integer
int adpt_ts_max_subConstant = 255;

char *adpt_ts_window_name = "Tune adaptive threshold filter";

char *adpt_ts_trackbar_adaptiveMethod = "adaptiveMethod: \n 0: Mean \n 1: Gaussian";
char *adpt_ts_trackbar_thresholdType = "thresholdType: \n 0: Binary \n 1: Binary Inverse";
char *adpt_ts_trackbar_blockSize = "blockSize:";
char *adpt_ts_trackbar_subConstant = "Subtraction Constant:";
