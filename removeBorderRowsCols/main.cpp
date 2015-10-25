#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


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

int main()
{
    Mat a(150, 150, CV_8UC3, Scalar::all(255));

    imshow("a", a);

    Mat b;
    copyMakeBorder(a, a, 10, 10, 10, 10, BORDER_CONSTANT, Scalar(0, 0, 0));

    imshow("b", a);

    removeBorder(a, a, 10, 10, 10, 10);
    imshow("c", a);

    Range range(20, 50);


    Mat mat = (Mat_<int>(5, 5) << 1, 2, 3, 4, 5,
                                  7, 8, 9, 10, 11,
                                  12, 13, 14, 15, 16,
                                  17, 18, 19, 20, 21,
                                  22, 23, 24, 25, 26);

    cout << "mat: " << endl << mat << endl;

    Mat col;
    removeCol(mat, col, 3);
    cout << "removeCol: " << endl << col << endl;

    Mat cols;
    removeCols(mat, cols, cv::Range(2, 3));
    cout << "removeCols: " << endl << cols << endl;


    Mat row;
    removeRow(mat, row, 3);
    cout << "removeRow: " << endl << row << endl;

    Mat rows;
    removeRows(mat, rows, cv::Range(2, 3));
    cout << "removeRows: " << endl << rows << endl;

    waitKey();
    return 0;
}

