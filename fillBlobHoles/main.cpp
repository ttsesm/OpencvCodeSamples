#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("../blobs.jpg"); // image should be single channel black and white image

    if(src.empty() || !src.data)
    {
        cerr << "Problem loading image!!!" << endl;
        return -1;
    }

    imshow("src",src);

    cout << "src size: " << src.size() << endl;

    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY);
//    imshow("gray", gray);

    Mat bin;
    threshold(gray, bin, 50, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("bin", bin);

    // A image with size greater than the present object is created, it is needed from floodFill()
    cv::Mat mask = cv::Mat::zeros(src.rows + 2, src.cols + 2, CV_8U);

    cout << "mask size: " << mask.size() << endl;

    cv::floodFill(bin, mask, cv::Point(0,0), 255, 0, cv::Scalar(), cv::Scalar(), 4 + (255 << 8) + cv::FLOODFILL_MASK_ONLY);
    //NOTE Since the mask is larger than the filled image, a pixel  (x, y) in image corresponds to the pixel (x+1, y+1) in the mask .

    //remove the extra lines added earlier in the initialization of the mask
    Mat dst;
    mask(Range(1, mask.rows - 1), Range(1, mask.cols-1)).copyTo(dst);

    cout << "dst size: " << dst.size() << endl;

    imshow("dst", ~dst);

        waitKey();
        return 0;
}

