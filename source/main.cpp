#include <opencv2/opencv.hpp>
#include "main.h"

using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if (!cap.open(0))
        return 0;

    //windows initializations
    //window- Gaussian Blur
    namedWindow(Gau_blur_window_name, CV_WINDOW_AUTOSIZE);
    createTrackbar(Gau_blur_size_tackbar, Gau_blur_window_name, &Gau_blur_size, Gau_blur_max_size);

    //window-adaptive threshold tuning
    namedWindow(adpt_ts_window_name, CV_WINDOW_AUTOSIZE);
    createTrackbar(adpt_ts_trackbar_adaptiveMethod, adpt_ts_window_name, &adpt_ts_adaptiveMethod, adpt_ts_max_adaptiveMethod);
    createTrackbar(adpt_ts_trackbar_thresholdType, adpt_ts_window_name, &adpt_ts_thresholdType, adpt_ts_max_thresholdType);
    createTrackbar(adpt_ts_trackbar_blockSize, adpt_ts_window_name, &adpt_ts_blockSize, adpt_ts_max_blockSize);
    createTrackbar(adpt_ts_trackbar_subConstant, adpt_ts_window_name, &adpt_ts_subConstant, adpt_ts_max_subConstant);

    //window-morphologytuning
    namedWindow(mplg_window_name, CV_WINDOW_AUTOSIZE);
    createTrackbar(mplg_trackbar_shape, mplg_window_name, &mplg_shape, mplg_max_shape);
    createTrackbar(mplg_trackbar_witdth, mplg_window_name, &mplg_size_witdth, mplg_max_size_witdth);
    createTrackbar(mplg_trackbar_height, mplg_window_name, &mplg_size_height, mplg_max_size_height);
    createTrackbar(mplg_trackbar_opertaion, mplg_window_name, &mplg_opertaion, mplg_max_opertaion);

    while (1)
    {
        Mat frame, grayA, binaryA, gauA, morphoA, cannyA;
        cap >> frame;
        if (frame.empty())
            break; // end of video stream

        //convert ot B&W
        cvtColor(frame, grayA, CV_BGR2GRAY);

        //Blur
        GaussianBlur(grayA, gauA, Size(17, 17), 0);
        imshow(Gau_blur_window_name, gauA);
        
        //highlight foreground
        adaptiveThreshold(gauA, binaryA, adpt_ts_BINARY_value, adpt_ts_adaptiveMethod, adpt_ts_thresholdType, ((adpt_ts_blockSize * 2 + 1) > 1) ? (adpt_ts_blockSize * 2 + 1) : 3, adpt_ts_subConstant);
        imshow(adpt_ts_window_name, binaryA);

        //make segment clear by morpho
        Mat morphoKernel = getStructuringElement(MORPH_ELLIPSE, Size(1, 5));

        morphologyEx(binaryA, morphoA, MORPH_OPEN, morphoKernel);
        imshow(mplg_window_name, morphoA);

        if (waitKey(1) == 27)
            break; // stop capturing by pressing ESC
    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}
