// HKUST RoboMaster Tutorial
// Author: Donald Gao, Jerry Xu

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

RotatedRect adjustRotatedRect(const RotatedRect &rect);
void show(const Mat& image);

int main()
{
    Mat a;
    a = imread("a.png");
    show(a);

    Mat grayA, binaryA;
    cvtColor(a, grayA, CV_BGR2GRAY);
    GaussianBlur(grayA, grayA, Size(13, 13), 0);
    adaptiveThreshold(grayA, binaryA, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 0);
    show(binaryA);

    vector<vector<Point2i>> contours;
    findContours(binaryA, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    float target_width = 101.6;
    float target_height = 56.8;
    vector<RotatedRect> results;
    for (int i = 0; i < contours.size(); i++)
    {
        RotatedRect rect = minAreaRect(contours[i]);
        rect = adjustRotatedRect(rect);
        if (rect.size.width > 0.8 * target_width && rect.size.height < 1.2 * target_width &&
            rect.size.height > 0.8 * target_height && rect.size.height < 1.2 * target_height &&
            rect.angle >= -15 && rect.angle <= 15)
        {
            results.push_back(rect);
        }
    }

    cout << "number of detected rectangles:" << results.size() << endl;
    Mat result_image;
    for (int i = 0; i < results.size(); i++)
    {
        a(results[i].boundingRect()).copyTo(result_image);
        show(result_image);
    }

    //Advanced part: draw all rectangles that we found
    //use other ways to do similar tasks
    //Note that sometimes approxPolyDP performs better, but it may use more memory space as well.
    for (auto c : contours)
    {
        vector<Point> curDigitContoursPoly;
        approxPolyDP(c, curDigitContoursPoly, 3, true);
        Rect rect = boundingRect(curDigitContoursPoly);
        rectangle(a, rect, Scalar(rand() % 255, rand() % 255, rand() % 255), 3);
    }
    show(a);
    return 0;
}

void show(const Mat& image)
{
    imshow("CV tutorial", image);
    waitKey(0);
}

RotatedRect adjustRotatedRect(const RotatedRect& rect)
{
	const Size2f &s = rect.size;
	if (s.width > s.height)
		return rect;
	return RotatedRect(rect.center, Size2f(s.height, s.width), rect.angle + 90.0);
}
