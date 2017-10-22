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

//Gaussian Blur values
int Gau_blur_size = 8;

int const Gau_blur_max_size = 50;

char *const Gau_blur_window_name = "Gaussian Blur";

char *const Gau_blur_size_tackbar = "Gaussian Blur Size (2n+1)";

//adaptive threshold values
int const adpt_ts_BINARY_value = 255;

int adpt_ts_adaptiveMethod = 1;
int adpt_ts_thresholdType = 1;
int adpt_ts_blockSize = 1; //here blocksize means half length of diagonal
int adpt_ts_subConstant = 0;

int const adpt_ts_max_adaptiveMethod = 1;
int const adpt_ts_max_thresholdType = 1;
int const adpt_ts_max_blockSize = 50; //change if necessary, sampling size to neigherhoods to determine threshold, calculation needed as the blocksize must be 1+2n where n is a positive integer
int const adpt_ts_max_subConstant = 255;

char *const adpt_ts_window_name = "Tune adaptive threshold filter";

char *const adpt_ts_trackbar_adaptiveMethod = "adaptiveMethod: \n 0: Mean \n 1: Gaussian";
char *const adpt_ts_trackbar_thresholdType = "thresholdType: \n 0: Binary \n 1: Binary Inverse";
char *const adpt_ts_trackbar_blockSize = "blockSize:";
char *const adpt_ts_trackbar_subConstant = "Subtraction Constant:";

//Morphological
char *const mplg_window_name = "Morphological transform values";

//Morphological-kernel
int mplg_shape = 0;
int mplg_size_witdth = 1; //these size will be distance from center to edge
int mplg_size_height = 1;

int const mplg_max_shape = 2;        // rect, cross, ellipse
int const mplg_max_size_witdth = 50; //these size will be distance from center to edge
int const mplg_max_size_height = 50;

char *const mplg_trackbar_shape = "kernel shape:\n 0:rectangle\n1:cross\n2:ellipse";
char *const mplg_trackbar_witdth = "kernel width (= value*2 + 1)";
char *const mplg_trackbar_height = "kernel height (= value*2 + 1)";

//Morphological-values
int mplg_opertaion = 0;

int const mplg_max_opertaion = 6;

char *const mplg_trackbar_opertaion = "0: Erode \n1: Dilate \n2: Open \n3: Close \n4: Gradient \n5: Tophat \n6: Blackhat";
