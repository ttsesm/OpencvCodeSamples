#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// functions applies steganography in spatial domain
void steganograph(Mat &aFrontImage, Mat &aHiddenImage)
{
    // check if two images are the same type and size
    CV_Assert(aFrontImage.type() == aHiddenImage.type() && aFrontImage.size() == aHiddenImage.size());

    // create our stego image, where we are gonna store the final result
    Mat aStegedImage(aFrontImage.rows, aFrontImage.cols, aFrontImage.type());
    // create some temp images that we are gonna need later for the process
    Mat tFront_image, tHidden_image;

    if(aFrontImage.channels() == 3) // check if we are dealing with color images
    {
        // populate mask matrices with the value 0xF0 or 11110000 in binary
        Mat front_mask(aFrontImage.rows, aFrontImage.cols, aFrontImage.type(), Scalar(0xF0, 0xF0, 0xF0));
        Mat hidden_mask(aHiddenImage.rows, aHiddenImage.cols, aHiddenImage.type(), Scalar(0xF0, 0xF0, 0xF0));

        // perform bitwise ANDing of two matrices and store the result in a third matrix.
        // What we achieved with this operation? Well, now the resulting tFront_image and
        // tHidden_image matrices contains only the first four important bits of each pixel
        // in aFrontImage. The remaining four bits are zero padded
        bitwise_and(aFrontImage, front_mask, tFront_image);
        bitwise_and(aHiddenImage, hidden_mask, tHidden_image);

        for(int j = 0; j < aHiddenImage.rows; j++)
            for(int i = 0; i < aHiddenImage.cols; i++)
            {
                // right-shift the pixel components of the tHidden_image matrix by 4 bits,
                // and hence the first four bits are zero padded.
                tHidden_image.at<Vec3b>(j,i)[0] = tHidden_image.at<Vec3b>(j,i)[0] >> 4;
                tHidden_image.at<Vec3b>(j,i)[1] = tHidden_image.at<Vec3b>(j,i)[1] >> 4;
                tHidden_image.at<Vec3b>(j,i)[2] = tHidden_image.at<Vec3b>(j,i)[2] >> 4;
            }
    }else if(aFrontImage.channels() == 1){ // check if we are dealing with grayscale images
        Mat front_mask(aFrontImage.rows, aFrontImage.cols, aFrontImage.type(), Scalar(0xF0));
        Mat hidden_mask(aHiddenImage.rows, aHiddenImage.cols, aHiddenImage.type(), Scalar(0xF0));

        bitwise_and(aFrontImage, front_mask, tFront_image);
        bitwise_and(aHiddenImage, hidden_mask, tHidden_image);

        for(int j = 0; j < aHiddenImage.rows; j++)
            for(int i = 0; i < aHiddenImage.cols; i++)
            {
                tHidden_image.at<uchar>(j,i) = tHidden_image.at<uchar>(j,i) >> 4;
            }
    }

    // Finally, perform the bitwise addition of the tFront_image and tHidden_image matrices
    // to obtain aStegedImage, which is our steganograph image
    bitwise_or(tFront_image, tHidden_image, aStegedImage);

    // save and show the stego image
    imwrite("stegedImg.png", aStegedImage);
    imshow("aStegedImg", aStegedImage);
}

// function to desteganograph an image
void deSteganograph(Mat &aStegedImage)
{
    // create matrices to store the results
    Mat aFrontImage(aStegedImage.rows, aStegedImage.cols, aStegedImage.type());
    Mat aHiddenImage(aStegedImage.rows, aStegedImage.cols, aStegedImage.type());

//    Mat tFront_image, tHidden_image;

    if(aFrontImage.channels() == 3) // check if we are dealing with color images
    {
        // populate again the mask matrices with the values 0xF0 or 11110000 in binary
        // and 0x0F or 00001111 in binary depending which image we want to retrieve
        Mat front_mask(aStegedImage.rows, aStegedImage.cols, aStegedImage.type(), Scalar(0xF0, 0xF0, 0xF0));
        Mat hidden_mask(aStegedImage.rows, aStegedImage.cols, aStegedImage.type(), Scalar(0x0F, 0x0F, 0x0F));

        // apply again bitwise_ANDing to retrieve the images
        bitwise_and(aStegedImage, front_mask, aFrontImage);
        bitwise_and(aStegedImage, hidden_mask, aHiddenImage);

        for(int j = 0; j < aHiddenImage.rows; j++)
            for(int i = 0; i < aHiddenImage.cols; i++)
            {
                // left-shift the pixel components of aHidden_image by 4 bits, because
                // the first four bits are zero padded and the actual information is stored
                // in the last 4 bits
                aHiddenImage.at<Vec3b>(j,i)[0] = aHiddenImage.at<Vec3b>(j,i)[0] << 4;
                aHiddenImage.at<Vec3b>(j,i)[1] = aHiddenImage.at<Vec3b>(j,i)[1] << 4;
                aHiddenImage.at<Vec3b>(j,i)[2] = aHiddenImage.at<Vec3b>(j,i)[2] << 4;
            }
    }else if(aFrontImage.channels() == 1){ // check if we are dealing with grayscale images
        Mat front_mask(aStegedImage.rows, aStegedImage.cols, aStegedImage.type(), Scalar(0xF0));
        Mat hidden_mask(aStegedImage.rows, aStegedImage.cols, aStegedImage.type(), Scalar(0x0F));

        bitwise_and(aStegedImage, front_mask, aFrontImage);
        bitwise_and(aStegedImage, hidden_mask, aHiddenImage);

        for(int j = 0; j < aHiddenImage.rows; j++)
            for(int i = 0; i < aHiddenImage.cols; i++)
            {
                aHiddenImage.at<uchar>(j,i) = aHiddenImage.at<uchar>(j,i) << 4;
            }
    }

    // final images
    imshow("front", aFrontImage);
    imshow("hidden", aHiddenImage);
}

int main()
{
    Mat img1 = imread("../lena.png");
    Mat img2 = imread("../baboon.png");

    if(img1.empty() || !img1.data)
    {
        cerr << "Problem loading first image!" << endl;
        return -1;
    }
    if(img2.empty() || !img2.data)
    {
        cerr << "Problem loading second image!" << endl;
        return -1;
    }

    steganograph(img1, img2);

    Mat img3 = imread("stegedImg.png");
    deSteganograph(img3);

    imshow("image1", img1);
    imshow("image2", img2);
    waitKey();
    return 0;
}
