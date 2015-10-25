// http://answers.opencv.org/question/74129/is-there-any-efficientfastbest-way-to-removedelete-a-given-rowcol-from-a-cvmat/
// http://stackoverflow.com/questions/29696805/what-is-the-best-way-to-remove-a-row-or-col-from-a-cv-mat
// https://gist.github.com/enpe/369a7a17fd9b3856b544

#include <iostream>
#include <opencv2/opencv.hpp>

#include "Timer.h"

using namespace cv;
using namespace std;

enum METHOD {CV_RECT, CV_RANGE, STD_MEMCPY};


void removeBorder(InputArray _src, OutputArray _dst, int top, int bottom, int left, int right)
{
    CV_Assert( top >= 0 && bottom >= 0 && left >= 0 && right >= 0 );

    Mat src = _src.getMat();
    int type = src.type();

    _dst.create( src.rows - top - bottom, src.cols - left - right, type );
    Mat dst = _dst.getMat();

//    if(top == 0 && left == 0 && bottom == 0 && right == 0)
//    {
//        if(src.data != dst.data || src.step != dst.step)
//            src.copyTo(dst);
//        return;
//    }

    if(src.data != dst.data || src.step != dst.step)
        src(Range(top, src.rows - bottom), Range(left, src.cols-right)).copyTo(dst);
    return;
}

void removeRow(InputOutputArray _matIn, int row, int method = METHOD::CV_RECT)
{
    CV_Assert( row >= 0 && row < _matIn.getMat().rows );

    Mat matIn = _matIn.getMat();
    cv::Size size = matIn.size();
    Mat matOut( matIn.rows - 1, matIn.cols, matIn.type());

    switch(method)
    {
        case(METHOD::STD_MEMCPY) :
        {

            int rowSizeInBytes = size.width * matIn.elemSize();

            if ( row > 0 )
            {
                int numRows  = row;
                int numBytes = rowSizeInBytes * numRows;
                std::memcpy( matOut.data, matIn.data, numBytes );
            }

            if ( row < size.height - 1 )
            {
                int matOutOffset = rowSizeInBytes * row;
                int matInOffset  = matOutOffset + rowSizeInBytes;

                int numRows  = size.height - ( row + 1 );
                int numBytes = rowSizeInBytes * numRows;
                std::memcpy( matOut.data + matOutOffset , matIn.data + matInOffset, numBytes );
            }
        }
        break;

        case(METHOD::CV_RANGE) :
        {
            if(matIn.data != matOut.data || matIn.step != matOut.step)
            {
                if(row == 0)
                {
                    matIn(Range(1, matIn.rows), Range(0, matIn.cols)).copyTo(matOut);
                } else if (row == matIn.rows - 1) {
                    matIn(Range(0, row), Range(0, matIn.cols)).copyTo(matOut);
                } else {
                    Mat a, b;
                    matIn(Range(0, row), Range(0, matIn.cols)).copyTo(a);
                    matIn(Range(row +1, matIn.rows), Range(0, matIn.cols)).copyTo(b);
        //            cout << "dst: " << a << endl;
        //            cout << "temp: " << b << endl;
                    vconcat(a, b, matOut);
                }
            }
        }
        break;

        default :

            if ( row > 0 )
            {
                cv::Rect rect( 0, 0, size.width, row );
                matIn( rect ).copyTo( matOut( rect ) );
            }

            if ( row < size.height - 1 )
            {
                cv::Rect rect1( 0, row + 1, size.width, size.height - row - 1 );
                cv::Rect rect2( 0, row, size.width, size.height - row - 1 );
                matIn( rect1 ).copyTo( matOut( rect2 ) );
            }
        break;
    }

    matOut.copyTo(_matIn);
}

void removeRow(InputArray _src, OutputArray _dst, int row)
{
    CV_Assert( row >= 0 && row < _src.getMat().rows );

    Mat src = _src.getMat();
    int type = src.type();

    _dst.create( src.rows - 1, src.cols, type );
    Mat dst = _dst.getMat();

    if(src.data != dst.data || src.step != dst.step)
    {
        if(row == 0)
        {
            src(Range(1, src.rows), Range(0, src.cols)).copyTo(dst);
        } else if (row == src.rows - 1) {
            src(Range(0, row), Range(0, src.cols)).copyTo(dst);
        } else {
            Mat a, b;
            src(Range(0, row), Range(0, src.cols)).copyTo(a);
            src(Range(row +1, src.rows), Range(0, src.cols)).copyTo(b);
//            cout << "dst: " << a << endl;
//            cout << "temp: " << b << endl;
            vconcat(a, b, dst);
        }
    }
    return;
}

void removeRows(InputArray _src, OutputArray _dst, cv::Range range)
{
    CV_Assert( range.start >= 0 && range.end < _src.getMat().rows );

    Mat src = _src.getMat();
    int type = src.type();

    _dst.create( src.rows - range.size() - 1, src.cols, type );
    Mat dst = _dst.getMat();

    if(src.data != dst.data || src.step != dst.step)
    {
        for(int i = range.start; i <= range.end; ++i)
        {
//            cout << i << endl;
            removeRow(src, src, range.start);
//            cout << "src: " << endl << src << endl;
        }

        src.copyTo(dst);
    }
    return;
}

void removeRows(InputOutputArray _src, cv::Range range)
{
    CV_Assert( range.start >= 0 && range.end < _src.getMat().rows );

    Mat src = _src.getMat();
    int type = src.type();

//    if(src.data != dst.data || src.step != dst.step)
//    {
        for(int i = range.start; i <= range.end; ++i)
        {
            //            cout << i << endl;
            removeRow(src, src, range.start);
            //            cout << "src: " << endl << src << endl;
        }

        src.copyTo(_src);
//    }
    return;
}

void removeCol(InputOutputArray _matIn, int col, int method = METHOD::CV_RECT)
{
    CV_Assert( col >= 0 && col < _matIn.getMat().cols );

    Mat matIn = _matIn.getMat();
    cv::Size size = matIn.size();
    Mat matOut( matIn.rows, matIn.cols - 1, matIn.type());

    switch(method)
    {
        case(METHOD::STD_MEMCPY) :
        {

            int rowInInBytes  = size.width * matIn.elemSize();
            int rowOutInBytes = ( size.width - 1 ) * matIn.elemSize();

            if ( col > 0 )
            {
                int matInOffset = 0;
                int matOutOffset = 0;
                int numCols = col;
                int numBytes = numCols * matIn.elemSize();

                for ( int y = 0; y < size.height; ++y )
                {
                    std::memcpy( matOut.data + matOutOffset, matIn.data + matInOffset, numBytes );

                    matInOffset  += rowInInBytes;
                    matOutOffset += rowOutInBytes;
                }
            }

            if ( col < size.width - 1 )
            {
                int matInOffset = ( col + 1 ) * matIn.elemSize();
                int matOutOffset = col * matIn.elemSize();
                int numCols = size.width - ( col + 1 );
                int numBytes = numCols * matIn.elemSize();

                for ( int y = 0; y < size.height; ++y )
                {
                    std::memcpy( matOut.data + matOutOffset, matIn.data + matInOffset, numBytes );

                    matInOffset  += rowInInBytes;
                    matOutOffset += rowOutInBytes;
                }
            }
        }
        break;

        case(METHOD::CV_RANGE) :
        {
            if(matIn.data != matOut.data || matIn.step != matOut.step)
            {
                if(col == 0)
                {
                    matIn(Range(0, matIn.rows), Range(1, matIn.cols)).copyTo(matOut);
                } else if (col == matIn.cols - 1) {
                    matIn(Range(0, matIn.rows), Range(0, col)).copyTo(matOut);
                } else {
                    Mat a, b;
                    matIn(Range(0, matIn.rows), Range(0, col)).copyTo(a);
                    matIn(Range(0, matIn.rows), Range(col + 1, matIn.cols)).copyTo(b);
                    //            cout << "dst: " << a << endl;
                    //            cout << "temp: " << b << endl;
                    hconcat(a, b, matOut);
                }
            }
        }
        break;

        default :

            if ( col > 0 )
            {
                cv::Rect rect( 0, 0, col, size.height );
                matIn( rect ).copyTo( matOut( rect ) );
            }

            if ( col < size.width - 1 )
            {
                cv::Rect rect1( col + 1, 0, size.width - col - 1, size.height );
                cv::Rect rect2( col,     0, size.width - col - 1, size.height );
                matIn( rect1 ).copyTo( matOut( rect2 ) );
            }
        break;
    }

    matOut.copyTo(_matIn);
}

void removeCol(InputArray _src, OutputArray _dst, int col)
{
    CV_Assert( col >= 0 && col < _src.getMat().cols );

    Mat src = _src.getMat();
    int type = src.type();

    _dst.create( src.rows, src.cols - 1, type );
    Mat dst = _dst.getMat();

    if(src.data != dst.data || src.step != dst.step)
    {
        if(col == 0)
        {
            src(Range(0, src.rows), Range(1, src.cols)).copyTo(dst);
        } else if (col == src.cols - 1) {
            src(Range(0, src.rows), Range(0, col)).copyTo(dst);
        } else {
            Mat a, b;
            src(Range(0, src.rows), Range(0, col)).copyTo(a);
            src(Range(0, src.rows), Range(col + 1, src.cols)).copyTo(b);
//            cout << "dst: " << a << endl;
//            cout << "temp: " << b << endl;
            hconcat(a, b, dst);
        }
    }
    return;
}

void removeCols(InputArray _src, OutputArray _dst, cv::Range range)
{
    CV_Assert( range.start >= 0 && range.end < _src.getMat().cols );

    Mat src = _src.getMat();
    int type = src.type();

    _dst.create( src.rows, src.cols - range.size() - 1, type );
    Mat dst = _dst.getMat();

    if(src.data != dst.data || src.step != dst.step)
    {
        for(int i = range.start; i <= range.end; ++i)
        {
//            cout << i << endl;
            removeCol(src, src, range.start);
//            cout << "src: " << endl << src << endl;
        }

        src.copyTo(dst);
    }
    return;
}

void removeCols(InputOutputArray _src, cv::Range range)
{
    CV_Assert( range.start >= 0 && range.end < _src.getMat().cols );

    Mat src = _src.getMat();
    int type = src.type();

//    Mat dst( src.rows, src.cols - range.size() - 1, type );

//    if(src.data != dst.data || src.step != dst.step)
//    {
        for(int i = range.start; i <= range.end; ++i)
        {
            //            cout << i << endl;
            removeCol(src, /*src, */range.start);
            //            cout << "src: " << endl << src << endl;
        }

        src.copyTo(_src);
//    }
    return;
}

int main()
{
    Mat a(150, 150, CV_8UC3, Scalar::all(255));

    imshow("a", a);

//    Mat b;
    copyMakeBorder(a, a, 1, 0, 1, 0, BORDER_CONSTANT, Scalar(0, 0, 0));

    imshow("b", a);

//    removeBorder(a, a, 10, 10, 10, 10);
//    imshow("c", a);

//    Range range(20, 50);


    Mat mat = (Mat_<int>(5, 5) << 1, 2, 3, 4, 5,
                                  7, 8, 9, 10, 11,
                                  12, 13, 14, 15, 16,
                                  17, 18, 19, 20, 21,
                                  22, 23, 24, 25, 26);

    cout << "mat: " << endl << mat << endl;

//    Mat col;
//    removeCol(mat, col, 3);
//    cout << "removeCol: " << endl << col << endl;

    Mat cols;
    removeCols(mat, /*cols, */cv::Range(2, 3));
    cout << "removeCols: " << endl << mat << endl;


    Mat row;
//    removeRow(mat, row, 3);
    removeRow(a, 0, METHOD::STD_MEMCPY);
    removeCol(a, 0/*, METHOD::STD_MEMCPY*/);
//    cout << "removeRow: " << endl << row << endl;
//    cout << "removeRow: " << endl << mat << endl;
    imshow("c", a);

//    Mat rows;
//    removeRows(mat, rows, cv::Range(2, 3));
//    cout << "removeRows: " << endl << rows << endl;

    waitKey();
    return 0;
}

