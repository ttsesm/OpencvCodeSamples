#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void matwrite(const string& filename, InputArray _mat)
{
    ofstream fs(filename, fstream::binary);

    Mat mat = _mat.getMat();

    // Header
    int type = mat.type();
    int channels = mat.channels();
    fs.write((char*)&mat.rows, sizeof(int));    // rows
    fs.write((char*)&mat.cols, sizeof(int));    // cols
    fs.write((char*)&type, sizeof(int));        // type
    fs.write((char*)&channels, sizeof(int));    // channels

    // Data
    if (mat.isContinuous())
    {
        fs.write(mat.ptr<char>(0), (mat.dataend - mat.datastart));
    }
    else
    {
        int rowsz = CV_ELEM_SIZE(type) * mat.cols;
        for (int r = 0; r < mat.rows; ++r)
        {
            fs.write(mat.ptr<char>(r), rowsz);
        }
    }

    fs.close();
}

Mat matread(const string& filename)
{
    ifstream fs(filename, fstream::binary);

    // Header
    int rows, cols, type, channels;
    fs.read((char*)&rows, sizeof(int));         // rows
    fs.read((char*)&cols, sizeof(int));         // cols
    fs.read((char*)&type, sizeof(int));         // type
    fs.read((char*)&channels, sizeof(int));     // channels

    // Data
    Mat mat(rows, cols, type);
    fs.read((char*)mat.data, CV_ELEM_SIZE(type) * rows * cols);

    return mat;
}

void saveMatTo(InputArray _matrix, string filename, int fmt)
{
    Mat matrix = _matrix.getMat();
    ofstream outputFile(filename);
    outputFile << format(matrix, fmt) << endl;
    outputFile.close();
}

// save on file
void writeVectorOfVector(FileStorage &fs, string name, vector<vector<int>> &data)
{
    fs << name;
    fs << "{";
    for (int i = 0; i < data.size(); i++)
    {
        fs << name + "_" + to_string(i);
        vector<int> tmp = data[i];
        fs << tmp;
    }
    fs << "}";
}

// read from file
void readVectorOfVector(FileStorage &fns, string name, vector<vector<int>> &data)
{
    data.clear();
    FileNode fn = fns[name];
    if (fn.empty()){
        return;
    }

    FileNodeIterator current = fn.begin(), it_end = fn.end();
    for (; current != it_end; ++current)
    {
        vector<int> tmp;
        FileNode item = *current;
        item >> tmp;
        data.push_back(tmp);
    }
}

int main()
{
    Mat m(50, 50, CV_8UC1);
    randu(m, 0, 1000);

    vector<int> x(4, 5);

    // Save the random generated data from 2d vector
    {
        // Transform 2d vector to mat
        int size = 5;
        vector<vector<int> > w(size);
        for(int i = 0; i < size; ++i)
        {
            w[i].resize(size);
            for(int j = 0; j < size; ++j)
                w[i][j] = (i+1) * 10 + j;
        }

        Mat mat(size, size, CV_64F);
        for(int i = 0; i < w.size(); ++i)
            mat.row(i) = Mat(w[i]).t();

        cout << mat << endl;

        // Method 1 filestorage
        FileStorage fs("test.yml", FileStorage::WRITE);
        writeVectorOfVector(fs, "values", w);
        fs.release();
    }

    // Save the random generated data from Matrix or 1d vector
    {
        // Method 1: using FileStorage
        FileStorage fs("fs.yml", FileStorage::WRITE);
        fs << "m" << x;
        fs.release();

        // Method 2: usign raw binary data
        matwrite("raw.bin", m);

        // Method 3: usign cv::Formatter
        // FMT_DEFAULT
        // FMT_MATLAB
        // FMT_CSV
        // FMT_PYTHON
        // FMT_NUMPY
        // FMT_C
        saveMatTo(/*m*/x,"data.csv", cv::Formatter::FMT_CSV);
    }

    // Load the saved data from a 2d vector
    {
        vector<vector<int> > tset;

        // Method 1 filestorage
        FileStorage fs2("test.yml", FileStorage::READ);
        readVectorOfVector(fs2, "values", tset);
        fs2.release();

        for(size_t i = 0; i < tset.size(); ++i)
        {
            for(size_t j = 0; j < tset[i].size(); ++j)
                cout << tset[i][j] << " ";

            cout << endl << endl;
        }
    }

    // Load the saved matrix

    {
        // Method 1: using FileStorage
        double tic = double(getTickCount());

        FileStorage fs("fs.yml", FileStorage::READ);
//        Mat m1;
        vector<int> m1;
        fs["m"] >> m1;
        fs.release();
        cout << m1[0] << m1[1] << endl;

        double toc = (double(getTickCount()) - tic) * 1000. / getTickFrequency();
        cout << "Using FileStorage: " << toc << endl;
    }

    {
        // Method 2: usign raw binary data
        double tic = double(getTickCount());

        Mat m2 = matread("raw.bin");

        double toc = (double(getTickCount()) - tic) * 1000. / getTickFrequency();
        cout << "Using Raw: " << toc << endl;
    }

    {
        // Method 3 using TrainData
        cv::Ptr<cv::ml::TrainData> raw_data = cv::ml::TrainData::loadFromCSV("test.csv", 0, -2, 0);
        cv::Mat data = raw_data->getSamples();
        // optional if you have a color image and not just raw data
        data.convertTo(data, CV_8UC3);
        data = data.reshape(3); //set number of channels
    }

    return 0;
}

