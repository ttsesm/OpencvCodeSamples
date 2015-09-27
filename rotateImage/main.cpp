#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
  Mat src = imread("../baboon.jpg", 0);

  if(src.empty())
    cerr << "Error: Loading image" << endl;

  int r0, c0; // point to rotate image against
  int r1, c1, r2, c2; // tranformed point
  int rows, cols; // original image rows and columns
  rows = src.rows;
  cols = src.cols;

  int theta = 45; // angle in degrees
  float rads = (theta * 3.14159265) / 180.0; // angle in rads


  Mat forward = Mat::ones(src.rows * sin(((90 * 3.14159265)/180) - rads) + src.cols * sin(rads),
                      src.cols * cos(rads) + src.rows * cos(((90 * 3.14159265)/180) - rads),
                      src.type()); // create output image to be the best fit to the source

  Mat backward = Mat::ones(src.rows * sin(((90 * 3.14159265)/180) - rads) + src.cols * sin(rads),
                      src.cols * cos(rads) + src.rows * cos(((90 * 3.14159265)/180) - rads),
                      src.type()); // create output image to be the best fit to the source

  r0 = rows / 2; // rotate according to the center of the image
  c0 = cols / 2;

  r2 = (forward.rows - src.rows) / 2; // find the mapping point in the output image
  c2 = (forward.cols - src.cols) / 2;

/*********************************************************************************************/
  /* forward mapping */
  for(int r = 0; r < src.rows; r++) // loop through the image
    {
      for(int c = 0; c < src.cols; c++)
        {

          r1 = (r0 + ((r - r0) * cos(rads)) - ((c - c0) * sin(rads))) + r2; // map old point to new
          c1 = (c0 + ((r - r0) * sin(rads)) + ((c - c0) * cos(rads))) + c2;

// Use this if you want to rotate from (0, 0) instead
//          r1 = r * cos(rads) - c * sin(rads);
//          c1 = r * sin(rads) + c * cos(rads);

          if(r1 >= 0 && r1 < forward.rows && c1 >= 0 && c1 < forward.cols) // check if the point is withing the boundaries
            {
              forward.at<uchar>(r1, c1) = src.at<uchar>(r, c); // set value
            }
        }
    }
/*********************************************************************************************/
  /* backward mapping */

  for(int r = 0; r < backward.rows; r++) // loop through the image
    {
      for(int c = 0; c < backward.cols; c++)
        {

          r1 = (r0 + ((r - r0 - r2) * cos(rads)) + ((c - c0 - c2) * sin(rads))); // map old point to new
          c1 = (c0 - ((r - r0 - r2) * sin(rads)) + ((c - c0 - c2) * cos(rads)));

// Use this if you want to rotate from (0, 0) instead
//          r1 = r * cos(rads) - c * sin(rads);
//          c1 = r * sin(rads) + c * cos(rads);

          if(r1 >= 0 && r1 < src.rows && c1 >= 0 && c1 < src.cols) // check if the point is withing the boundaries
            {
              backward.at<uchar>(r, c) = src.at<uchar>(r1, c1); // set value
            }
        }
    }
/*********************************************************************************************/

  namedWindow("Source image", CV_WINDOW_AUTOSIZE);
  namedWindow("Forward rotated image", CV_WINDOW_AUTOSIZE);
  namedWindow("Backward rotated image", CV_WINDOW_AUTOSIZE);
  imshow("Source image", src);
  imshow("Forward rotated image", forward);
  imshow("Backward rotated image", backward);

  waitKey(0);

  return 0;
}

