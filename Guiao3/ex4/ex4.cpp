#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat image_gray;
    Mat dst, dst_norm, dst_norm_scaled;

    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    int thresh = 200;
    int max_thresh = 255;

    for(;;)
    {
        Mat frame;
        cap >> frame;
        dst = Mat::zeros( frame.size(), CV_32FC1 ); /// coloca a matriz a 0 num canal, do tipo float de 32 bits (pode se usar mais canais para uma maior precisao

        /// Convert it to gray
        cvtColor( frame, image_gray, CV_BGR2GRAY);
        /// Detecting corners
        cornerHarris( image_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
        /// Normalizing
        normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
        convertScaleAbs( dst_norm, dst_norm_scaled );

        /// Drawing a circle around corners
        for( int j = 0; j < dst_norm.rows ; j++ )
        {
            for( int i = 0; i < dst_norm.cols; i++ )
            {
                if( (int) dst_norm.at<float>(j,i) > thresh )
                {
                    circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 ); /// circulo --> explicar depois
                }
            }
        }

        /// Showing the result
        imshow( "corners window", dst_norm_scaled );

        //namedWindow("Original image", CV_WINDOW_AUTOSIZE);
        imshow("Original", frame);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}



