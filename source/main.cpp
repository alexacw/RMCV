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
    //adaptive threshold tuning
    namedWindow(adpt_ts_window_name, CV_WINDOW_AUTOSIZE);
    createTrackbar(adpt_ts_trackbar_adaptiveMethod, adpt_ts_window_name, &adpt_ts_adaptiveMethod, adpt_ts_max_adaptiveMethod);
    createTrackbar(adpt_ts_trackbar_thresholdType, adpt_ts_window_name, &adpt_ts_thresholdType, adpt_ts_max_thresholdType);
    createTrackbar(adpt_ts_trackbar_blockSize, adpt_ts_window_name, &adpt_ts_blockSize, adpt_ts_max_blockSize);
    createTrackbar(adpt_ts_trackbar_subConstant, adpt_ts_window_name, &adpt_ts_subConstant, adpt_ts_max_subConstant);

    while (1)
    {
        Mat frame, grayA, binaryA, gauA, morphoA, cannyA;
        cap >> frame;
        if (frame.empty())
            break; // end of video stream

        //namedWindow(adpt_ts_window_name, CV_WINDOW_AUTOSIZE);
        cvtColor(frame, grayA, CV_BGR2GRAY);
        //imshow("gray", grayA);

        GaussianBlur(grayA, gauA, Size(17, 17), 0);
        imshow("blur", grayA);

        adaptiveThreshold(gauA, binaryA, adpt_ts_BINARY_value, adpt_ts_adaptiveMethod, adpt_ts_thresholdType, ((adpt_ts_blockSize * 2 + 1) > 1) ? (adpt_ts_blockSize * 2 + 1) : 3, adpt_ts_subConstant);
        imshow(adpt_ts_window_name, binaryA);

        //morphologyEx(binaryA, morphoA, MORPH_OPEN, Mat());
        //imshow("morph errode ", binaryA);

        if (waitKey(1) == 27)
            break; // stop capturing by pressing ESC
    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}
