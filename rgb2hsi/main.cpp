#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{

  Mat src = imread("../baboon.jpg", 1);

  if(src.empty())
    cerr << "Error: Loading image" << endl;
  Mat hsi(src.rows, src.cols, src.type());
  Mat hsv;

  // change to hsv color space using opencv's embedded function
  cvtColor(src, hsv, CV_BGR2HSV);

  float r, g, b, h, s, in/*, norm*/;

//  i = (src.at<Vec3b>(0, 0)[0] + src.at<Vec3b>(0, 0)[1] + src.at<Vec3b>(0, 0)[2]) / 3;

  cout << "blue: " << (float)src.at<Vec3b>(0, 0)[0] << endl;
  cout << "green: " << (float)src.at<Vec3b>(0, 0)[1] << endl;
  cout << "red: " << (float)src.at<Vec3b>(0, 0)[2] << endl;

  for(int i = 0; i < src.rows; i++)
    {
      for(int j = 0; j < src.cols; j++)
        {
          b = src.at<Vec3b>(i, j)[0];
          g = src.at<Vec3b>(i, j)[1];
          r = src.at<Vec3b>(i, j)[2];

//          cout << "blue: " << b << endl;
//          cout << "green: " << g << endl;
//          cout << "red: " << r << endl;

          in = (b + g + r) / (3/**255*/);

//          norm = sqrt(r*r + g*g + b*b);

//          b = b / norm;
//          g = g / norm;
//          r = r / norm;

          int min_val = 0;
          min_val = std::min(r, std::min(b,g));

//          if(r + g + b == 765)
//            {
//              s = 0;
//              h = 0;
//            }else{
              s = 1 - 3*(min_val/(b + g + r));
              if(s < 0.00001)
                {
                  s = 0;
                }else if(s > 0.99999)
                {
                  s = 1;
                }
//            }


          if(s != 0)
            {
              h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
              h = acos(h);

              if(b <= g)
                {
                  h = h;
                } else{
                  h = /*2 * 3.14159265*/ ((360 * 3.14159265) / 180.0) - h;
                }
            }



//          cout << "hue: " << abs((h * 180) / 3.14159265) << endl;
//          cout << "saturation: " << s << endl;
//          cout << "intensity: " << in * 255 << endl << endl;

          hsi.at<Vec3b>(i, j)[0] = /*h*/(h * 180) / 3.14159265;
          hsi.at<Vec3b>(i, j)[1] = s*100;
          hsi.at<Vec3b>(i, j)[2] = in /** 255*/;
        }
    }

  cout << "hue: " << (float)hsi.at<Vec3b>(0, 0)[0] << endl;
  cout << "saturation: " << (float)hsi.at<Vec3b>(0, 0)[1] << endl;
  cout << "intensity: " << (float)hsi.at<Vec3b>(0, 0)[2] << endl;

//  Mat float_src(src.rows, src.cols, CV_32FC3);
//  src.convertTo(float_src, CV_32FC3);

  namedWindow("RGB image", CV_WINDOW_AUTOSIZE);
  namedWindow("HSI image", CV_WINDOW_AUTOSIZE);
  namedWindow("HSV image", CV_WINDOW_AUTOSIZE);
  imshow("RGB image", src);
  imshow("HSI image", hsi);
  imshow("HSV image", hsv);

  waitKey(0);
  return 0;
}

