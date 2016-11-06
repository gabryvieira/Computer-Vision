#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void DetectCircles(VideoCapture);
void DetectLines(VideoCapture);
/// global variables
Mat dst, gray_dst;
Mat image_gray;




void DetectCircles(VideoCapture cap){

    for(;;){
        Mat frame;
        cap >> frame;

        /// Convert it to gray
        cvtColor(frame, image_gray, CV_BGR2GRAY );

        /// Reduce the noise so we avoid false circle detection
        //GaussianBlur( image_gray, image_gray, Size(9, 9), 2, 2 );

        vector<Vec3f> circles;

        /// Apply the Hough Transform to find the circles
        HoughCircles( image_gray, circles, CV_HOUGH_GRADIENT, 1, image_gray.rows/8, 200, 100, 0, 0 );

        /// Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

        imshow("Circles detected", frame);
        if(waitKey(30) >= 0) break;
    }


}

void DetectLines(VideoCapture cap){
    for(;;)
    {
        Mat frame;
        cap >> frame;
        Canny(frame, dst, 50, 200, 3); /// Detect the edges of the image by using a Canny detector
        cvtColor(dst, gray_dst, CV_GRAY2BGR); /// conversao para tons de cinza

        vector<Vec2f> lines;
        HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 ); /// explicar parametros

        /// display result by drawing lines
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( gray_dst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }

        imshow("Detected line window", gray_dst);
        if(waitKey(30) >= 0) break;
    }



    //namedWindow("Original image", CV_WINDOW_AUTOSIZE);*/
}

int main(int argc, char *argv[])
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    int op;
    cout << "Choose if you want to detect lines or circles in image\n";
    cout << "1 - Lines\n";
    cout << "2 - Circles\n";
    cout << "Option --> ";
    cin >> op;

    switch(op){
        case 1:
        DetectLines(cap);
        break;

        case 2:
        DetectCircles(cap);
        break;
    }

    return 0;

}





