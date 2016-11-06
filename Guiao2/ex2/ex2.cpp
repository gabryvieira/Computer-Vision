#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
int threshold_value = 70;
int threshold_type = 2;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

const char* window_name = "Threshold Demo";
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";
void Threshold_Demo( int, void* );

int main( int, char** argv )
{
  Mat src, src_gray, dst;
  VideoCapture stream1(0);
  stream1.read(src);
  src_gray = Mat::zeros( src.size(), src.type() );
  cvtColor(src, src_gray, CV_BGR2GRAY, 1);
  namedWindow( window_name, WINDOW_AUTOSIZE );
  createTrackbar( trackbar_type,
                  window_name, &threshold_type,
                  max_type );
  createTrackbar( trackbar_value,
                  window_name, &threshold_value,
                  max_value );

  char c = 't';
  while (true) {
    stream1.read(src);
    cvtColor(src, src_gray, CV_RGB2GRAY, 1);

    switch (c) {
    case 'g': //grayscale
      dst = src_gray.clone();
      break;
    case 'a': //adaptive treshold
      adaptiveThreshold(src_gray, dst, threshold_value, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 1.5);
      break;
    case 't': //treshold
      threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );
      break;
    default: //original 
      dst = src.clone();
      break;
    }
    imshow( window_name, dst );

    char a;
    if ( (a = waitKey(30)) >= 0) {
      if (a == 'c')
	break;
      c = a;
    }
  }
}
