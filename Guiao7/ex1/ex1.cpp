#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;



int main(int argc, const char** argv)
{

      VideoCapture cap(0); // open the default camera
      if(!cap.isOpened())  // check if we succeeded
         return -1;

      Mat flow;

      // some faster than mat image container
      UMat  flowUmat, prevgray;

     for (;;)
     {

       Mat frame;
       cap >> frame;

       //Mat img;
       Mat original;

       // capture frame from video file
       cap.retrieve(frame, CV_CAP_OPENNI_BGR_IMAGE);
       resize(frame, frame, Size(640, 480));

       // save original for later
       frame.copyTo(original);

       // just make current frame gray
       cvtColor(frame, frame, COLOR_BGR2GRAY);

       if (prevgray.empty() == false ) {

        // calculate optical flow
        calcOpticalFlowFarneback(prevgray, frame, flowUmat, 0.4, 1, 12, 2, 8, 1.2, 0);
        // copy Umat container to standard Mat
        flowUmat.copyTo(flow);


               // By y += 5, x += 5 you can specify the grid
        for (int y = 0; y < original.rows; y += 5) {
         for (int x = 0; x < original.cols; x += 5)
         {
                  // get the flow from y, x position * 10 for better visibility
            const Point2f flowatxy = flow.at<Point2f>(y, x) * 10;
                         // draw line at flow direction
            line(original, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(0,0,255));
                                                             // draw initial point
            circle(original, Point(x, y), 1, Scalar(0, 0, 0), -1);
          }

         }
         // draw the results
        namedWindow("prew", WINDOW_AUTOSIZE);
        imshow("prew", original);

        // fill previous image again
        frame.copyTo(prevgray);

       }
       else {

        // fill previous image in case prevgray.empty() == true
        frame.copyTo(prevgray);

       }

       int key1 = waitKey(20);
     }
}


