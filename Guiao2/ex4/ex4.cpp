#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char *argv[])
{

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    Mat matBlur;
    namedWindow("Blur",1);

    Mat matGaussianBlur;
    namedWindow("Gaussian Blur", 1);

    Mat matMedianBlur;
    namedWindow("Median Blur ",1);

    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        //cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(frame, matGaussianBlur, Size(7,7), 0, 0);
        medianBlur(frame, matMedianBlur, 7);
        blur(frame, matBlur, Size(7,7), Point(-1, -1), BORDER_DEFAULT);
        imshow("Original", frame);
        imshow("Median Blur", matMedianBlur);
        imshow("Gaussian Blur", matGaussianBlur);
        imshow("Blur", matBlur);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}



