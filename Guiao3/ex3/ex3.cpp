#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void CannyThreshold(Mat* src, Mat* dst, int* lowThreshold)
{
  Mat detected_edges, src_gray;;
  int ratio = 3;
  int kernel_size = 3;

  cvtColor( *src, src_gray, CV_RGB2GRAY );
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges,
	 detected_edges,
	 *lowThreshold,
	 (*lowThreshold) * ratio,
	 kernel_size );

  /// Using Canny's output as a mask, we display our result
  *dst = Scalar::all(0);
  src->copyTo( *dst, detected_edges);
}

int main( int argc, char** argv )
{
  Mat src, dst;
  int const max_lowThreshold = 100;
  int lowThreshold = 0;
  VideoCapture stream1(0);

  //Setting up window
  namedWindow( "Canny", CV_WINDOW_AUTOSIZE );
  createTrackbar( "Min Threshold:",
		  "Canny",
		  &lowThreshold,
		  max_lowThreshold );

  while (true) {
    stream1.read( src );
    CannyThreshold( &src, &dst, &lowThreshold );
    imshow( "Camera output", src );
    imshow( "Canny", dst );

    if (waitKey(30) >= 0) {
      break;
    }
  }
}
