#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 * @brief This method applies some gaussian noise on a given image
 * @param img --> Input image (handles bothe rgb/grayscale images)
 * @param dst --> Output image
 * @param average --> corresponds to the mean value
 * @param sigma --> corresponds to the standard deviation value
 */
void addGaussianNoise(const Mat &img, Mat & dst, double average = 0.0, double sigma = 20.0)
{
    Mat noise_image;
    // We need to work with signed images (as noise can be
    // negative as well as positive). We use 16 bit signed
    // images as otherwise we would lose precision.
    if(img.channels() == 3)
    {
        /*Mat */noise_image.create(img.size(), CV_16SC3);
        randn(noise_image, Scalar::all(average), Scalar::all(sigma));
        img.convertTo(dst,CV_16SC3);
        addWeighted(dst, 1.0, noise_image, 1.0, 0.0, dst);
        dst.convertTo(dst, img.type());
    }else if(img.channels() == 1){
        noise_image.create(img.size(), CV_16SC1);
        randn(noise_image, Scalar::all(average), Scalar::all(sigma));
        img.convertTo(dst, CV_16SC1);
        addWeighted(dst, 1.0, noise_image, 1.0, 0.0, dst);
        dst.convertTo(dst, img.type());
    }

}


int main()
{
    cout << "start program" << endl;
    // load image
    Mat img = imread("../office.png", IMREAD_GRAYSCALE);

    // check if image is loaded correctly
    if(img.empty() || !img.data)
    {
        cerr << "Problem loading image!" << endl;
        return -1;
    }

    // show image
    imshow("image", img);

    Mat dst;
    addGaussianNoise(img, dst);

    cv::imshow("output",dst);
    waitKey();

    cout << "exit program" << endl;
    return 0;
}
