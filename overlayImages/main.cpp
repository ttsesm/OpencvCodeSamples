#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// first way
void overlayImage(Mat& src, Mat& overlay, const Point& location)
{
  for (int y = max(location.y, 0); y < src.rows; ++y)
  {
    int fY = y - location.y;

    if (fY >= overlay.rows)
      break;

    for (int x = max(location.x, 0); x < src.cols; ++x)
    {
      int fX = x - location.x;

      if (fX >= overlay.cols)
        break;

      double opacity = ((double)overlay.data[fY * overlay.step + fX * overlay.channels() + 3]) / 255;

      for (int c = 0; opacity > 0 && c < src.channels(); ++c)
      {
        unsigned char overlayPx = overlay.data[fY * overlay.step + fX * overlay.channels() + c];
        unsigned char srcPx = src.data[y * src.step + x * src.channels() + c];
        src.data[y * src.step + src.channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
      }
    }
  }
}

//// same as first but with pointers
//void overlayImage(Mat* src, Mat* overlay, const Point& location)
//{
//    for (int y = max(location.y, 0); y < src->rows; ++y)
//    {
//        int fY = y - location.y;

//        if (fY >= overlay->rows)
//            break;

//        for (int x = max(location.x, 0); x < src->cols; ++x)
//        {
//            int fX = x - location.x;

//            if (fX >= overlay->cols)
//                break;

//            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

//            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
//            {
//                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
//                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
//                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
//            }
//        }
//    }
//}

// second way
//void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
//  cv::Mat &output, cv::Point2i location)
//{
//  background.copyTo(output);


//  // start at the row indicated by location, or at row 0 if location.y is negative.
//  for(int y = std::max(location.y , 0); y < background.rows; ++y)
//  {
//    int fY = y - location.y; // because of the translation

//    // we are done of we have processed all rows of the foreground image.
//    if(fY >= foreground.rows)
//      break;

//    // start at the column indicated by location,

//    // or at column 0 if location.x is negative.
//    for(int x = std::max(location.x, 0); x < background.cols; ++x)
//    {
//      int fX = x - location.x; // because of the translation.

//      // we are done with this row if the column is outside of the foreground image.
//      if(fX >= foreground.cols)
//        break;

//      // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
//      double opacity =
//        ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

//        / 255.;


//      // and now combine the background and foreground pixel, using the opacity,

//      // but only if opacity > 0.
//      for(int c = 0; opacity > 0 && c < output.channels(); ++c)
//      {
//        unsigned char foregroundPx =
//          foreground.data[fY * foreground.step + fX * foreground.channels() + c];
//        unsigned char backgroundPx =
//          background.data[y * background.step + x * background.channels() + c];
//        output.data[y*output.step + output.channels()*x + c] =
//          backgroundPx * (1.-opacity) + foregroundPx * opacity;
//      }
//    }
//  }
//}

int main()
{
    // add the second parameter "-1" as flag, to make sure the transparancy channel is read!
    cv::Mat overlay = imread("../overlay.png", IMREAD_UNCHANGED);
    cv::Mat underlay = imread("../underlay.png", IMREAD_UNCHANGED);
//    cv::Mat result;

    imshow("over", overlay);
    imshow("under", underlay);
    waitKey();

    overlayImage(underlay, overlay, cv::Point(5,5));
//    overlayImage(&underlay, &overlay, cv::Point(5,5));
    cv::imshow("result", underlay);
    waitKey();

    cout << "Hello World!" << endl;
    return 0;
}

