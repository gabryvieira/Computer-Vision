#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main( int argc, char* argv[] )
{


 Mat image;
 image = imread("../images/lena.jpg", CV_LOAD_IMAGE_COLOR);

 if(image.empty())
       return -1;

 Mat gray_image;
 cvtColor( image, gray_image, CV_BGR2GRAY );

 imwrite( "Gray_Image.jpg", gray_image );

 namedWindow( "Original Image", CV_WINDOW_AUTOSIZE );
 namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );

 imshow( "Original image", image );
 imshow( "Gray image", gray_image );

 waitKey(0);

 return 0;
}



