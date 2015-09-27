#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Function that subdivides an image into blocks. The rowDivisor and colDivisor varialbles specify the number of blocks in rows and cols respectively
int subdivide(const cv::Mat &img, const int rowDivisor, const int colDivisor, std::vector<cv::Mat> &blocks)
{
    /* Some initialization */
    cv::Mat maskImg;

    /* Checking if the image was passed correctly */
    if(!img.data || img.empty())
        std::cerr << "Problem Loading Image" << std::endl;

    /* Cloning the image to another for visualization later */
    maskImg = img.clone();
    /* Checking if the clone image was cloned correctly */
    if(!maskImg.data || maskImg.empty())
        std::cerr << "Problem Loading Image" << std::endl;

    // check if divisors fit to image dimensions
    if(img.cols % colDivisor == 0 && img.rows % rowDivisor == 0)
    {
        for(int y = 0; y < img.cols; y += img.cols / colDivisor)
        {
            for(int x = 0; x < img.rows; x += img.rows / rowDivisor)
            {
                blocks.push_back(img(cv::Rect(y, x, (img.cols / colDivisor), (img.rows / rowDivisor))).clone());
                rectangle(maskImg, Point(y, x), Point(y + (maskImg.cols / colDivisor) - 1, x + (maskImg.rows / rowDivisor) - 1), CV_RGB(255, 0, 0), 1);

                imshow("Image", maskImg);
                waitKey(0);
            }
        }
    }else if(img.cols % colDivisor != 0)
    {
        cerr << "Error: Please use another divisor for the column split." << endl;
        exit(1);
    }else if(img.rows % rowDivisor != 0)
    {
        cerr << "Error: Please use another divisor for the row split." << endl;
        exit(1);
    }

    return EXIT_SUCCESS;
}

int main()
{
    Mat img = imread("../street.png");

    // check if image is loaded correctly
    if(img.empty() || !img.data)
    {
        cerr << "Problem loading image!" << endl;
        return -1;
    }

//    Mat rsz;
//    resize(img, rsz, Size(600, 400));

    // show image
    imshow("image", img);

    vector<Mat> blocks;

    subdivide(img, 8, 8, blocks);

    return 0;
}

