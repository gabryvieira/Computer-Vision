#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
using namespace cv;


int main( int argc, char** argv )
{
  VideoCapture stream1(0);
  char c = 'n';
  while (true) {
    Mat cameraFrame;
    stream1.read(cameraFrame);
    Mat cameraTransformed = Mat::zeros( cameraFrame.size(), cameraFrame.type() );
    switch (c) {
    case 'g':
      stream1.read(cameraFrame);
      cvtColor(cameraFrame, cameraTransformed, CV_RGB2GRAY, 1);
      break;
    case 'b':
      stream1.read(cameraFrame);
      cvtColor(cameraFrame, cameraTransformed, CV_RGB2GRAY, 1);
      adaptiveThreshold(cameraTransformed, cameraTransformed, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 1.5);
      break;
    default:
      stream1.read(cameraTransformed);
      break;
    }

    imshow("Camera output", cameraTransformed);
    char a;
    if ( (a = waitKey(30)) >= 0) {
      if (a == 'c')
	break;
      c = a;
    }
  }
}
