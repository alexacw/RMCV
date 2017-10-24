#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    // this is to store the result
    int resultDigit = -1;
    int cum_count = 0;
    int last_seen = -1;

    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if (!cap.open(0))
        return 0;

    frameWidth = cap.get(3);
    frameHeight = cap.get(4);

    //create transform image matrix for straighten digit
    Point2f srcTri[3], dstTri[3];

    Mat warp_mat(2, 3, CV_32FC1);

    srcTri[0] = Point2f(0, 0);
    srcTri[1] = Point2f(frameWidth - 1, 0);
    srcTri[2] = Point2f(0, frameWidth - 1);

    dstTri[0] = Point2f(0, 0);
    dstTri[1] = Point2f(frameWidth - 1, 0);
    dstTri[2] = Point2f(frameWidth * frameShiftMultiplier, frameWidth - 1);

    warp_mat = getAffineTransform(srcTri, dstTri);

    Rect myROI(frameWidth * (frameShiftMultiplier + cropPercent), frameHeight * (frameShiftMultiplier + cropPercent), frameWidth * (1.0 - 2 * (frameShiftMultiplier + cropPercent)), frameHeight * (1.0 - 2 * (frameShiftMultiplier + cropPercent)));
#ifdef tuningMode
    //windows initializations
    //Range threshold
    namedWindow("Color Range", CV_WINDOW_AUTOSIZE);
    createTrackbar("Low R", "Color Range", &low_r, 255);
    createTrackbar("High R", "Color Range", &high_r, 255);
    createTrackbar("Low G", "Color Range", &low_g, 255);
    createTrackbar("High G", "Color Range", &high_g, 255);
    createTrackbar("Low B", "Color Range", &low_b, 255);
    createTrackbar("High B", "Color Range", &high_b, 255);

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
    createTrackbar(mplg_trackbar_iterations, mplg_window_name, &mplg_iterations, mplg_max_iterations);
#endif

    Mat frame, grayA, binaryA, gauA, morphoA, cannyA;

    //for each frame
    while (1)
    {
        int frameResult = -1;
        Point PrevCenter = Point(0, 0);
        int ClosetestDistance = 0;
        //retrieve frame from camera
        cap >> frame;
        if (frame.empty())
            break; // end of video stream

        //transform frame to make digit straight
        warpAffine(frame, frame, warp_mat, frame.size());
        frame = frame(myROI);

        //Apply Color Threshold
        inRange(frame, Scalar(low_b, low_g, low_r), Scalar(high_b, high_g, high_r), grayA);

        //convert to B&W, Blur it, and apply threshold
        //cvtColor(redA, grayA, CV_BGR2GRAY);
        GaussianBlur(grayA, gauA, Size(Gau_blur_size * 2 + 1, Gau_blur_size * 2 + 1), 0);
        adaptiveThreshold(gauA, binaryA, adpt_ts_BINARY_value, adpt_ts_adaptiveMethod, adpt_ts_thresholdType, ((adpt_ts_blockSize * 2 + 1) > 1) ? (adpt_ts_blockSize * 2 + 1) : 3, adpt_ts_subConstant);

        //joint segments together by morpho
        Mat morphoKernel = getStructuringElement(mplg_shape, Size(mplg_size_witdth > 0 ? mplg_size_witdth : 1, mplg_size_height > 0 ? mplg_size_height : 1));
        morphologyEx(binaryA, morphoA, mplg_opertaion, morphoKernel, Point(-1, -1), mplg_iterations);

        //find contours
        vector<vector<Point>> contours;
        Rect boundRect;
        vector<Vec4i> hierarchy;
        Mat ContourSrc;
        morphoA.copyTo(ContourSrc);

        findContours(ContourSrc, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        //for each contours
        for (int i = 0; i < contours.size(); i++)
        {
            int contour_result = -1;

            boundRect = boundingRect(Mat(contours[i]));
            //calculate teh Width/Height ratio of the Rect
            float ratio = (float)boundRect.width / boundRect.height;

            //Filter out useless contours by ratio, size //TODO: tuning interface
            if ((ratio > 0.45 && ratio < 0.7 && boundRect.area() > 2000) || (ratio > 0.09 && ratio < 0.25 && boundRect.area() > 400))
            {
                //modify rect to check for 1
                if (ratio < 0.25)
                {
                    boundRect.x -= boundRect.height / 2;
                    boundRect.width += boundRect.height / 2;
                    if (boundRect.x < 0 || boundRect.x + boundRect.width >= morphoA.size().width)
                    {
                        break; // break the for loop if extended rect is out of bound
                    }
                }

                //check segments

                Mat digitA = morphoA(boundRect);
                // compute the rect of segments
                int xWidth = digitA.size().width / (2 + LWratio);
                int xLength = digitA.size().width * LWratio / (2 + LWratio);
                int yWidth = digitA.size().height / (3 + 2 * LWratio);
                int yLength = digitA.size().height * LWratio / (3 + 2 * LWratio);
                Rect segmentRect[7];
                segmentRect[0] = Rect(0, yWidth, xWidth, yLength);
                segmentRect[1] = Rect(xWidth, 0, xLength, yWidth);
                segmentRect[2] = Rect(xWidth + xLength, yWidth, xWidth, yLength);
                segmentRect[3] = Rect(xWidth, yWidth + yLength, xLength, yWidth);
                segmentRect[4] = Rect(0, yWidth * 2 + yLength, xWidth, yLength);
                segmentRect[5] = Rect(xWidth + xLength, yWidth * 2 + yLength, xWidth, yLength);
                segmentRect[6] = Rect(xWidth, yWidth * 2 + yLength * 2, xLength, yWidth);

                //determine segment is on or off
                bool segOn[7];
                for (int i = 0; i < 7; i++)
                {
                    segOn[i] = ((float)countNonZero(digitA(segmentRect[i])) / segmentRect[i].area()) > min_segment_match_percent;
                }

                //find matching digits
                for (int number = 0; number < 10; number++)
                {
                    bool match = true;
                    for (int segIndex = 0; segIndex < 7; segIndex++)
                        if (number_segment[number][segIndex] != segOn[segIndex])
                        {
                            match = false;
                            break;
                        }

                    //all segment's state match this number
                    if (match == true)
                        contour_result = number;
                }

                //Contour result pirority (closer to last position, others >7 >1)

                Point thisCenter = (boundRect.tl() - boundRect.br()) * (1.0 / 2.0);
                int distanceFromLast = norm(PrevCenter - thisCenter);
                if ((frameResult < 0) ||
                    (frameResult == 1 && contour_result != 1) ||
                    (frameResult == 7 && contour_result != 7 && contour_result != 1) ||
                    (ClosetestDistance > distanceFromLast))
                {
                    ClosetestDistance = distanceFromLast;
                    PrevCenter = thisCenter;
                    frameResult = contour_result;
                }

#ifdef tuningMode
// for (int i = 0; i < 7; i++)
// {
//     rectangle(digitA, segmentRect[i], Scalar(255, 0, 0));
// }
// imshow("frame", digitA);
// cout << ratio << "  ";frameResult
#endif
            }
        } //end contour loop

#ifdef tuningMode
        //for tuning purpose
        imshow(Gau_blur_window_name, gauA);
        imshow(adpt_ts_window_name, binaryA);
        imshow(mplg_window_name, morphoA);

        for (int i = 0; i < contours.size(); i++)
        {
            drawContours(ContourSrc, contours, i, Scalar(255, 0, 0));
        }
        imshow("Contours", ContourSrc);
#endif
        //store and update results
        if (resultDigit == frameResult)
            cum_count++;
        else
        {
            cout << frameResult<< endl;
            cum_count = 0;
        }
        resultDigit = frameResult;

        //confirmed digit, output
        if (cum_count == min_count_to_confirm && resultDigit != -1 && last_seen != resultDigit)
        {
            //TODO: Bluetooth
            cout << "Confiremed: " << resultDigit << endl;
            last_seen = resultDigit;
        }

        // stop program by pressing ESC
        if (waitKey(1) == 27)
            break;
    }

    // the camera will be closed automatically upon exit no cap.close needed
    return 0;
}
