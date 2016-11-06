#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;


double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

int main( int argc, char** argv )
{
  /* Read image given by user */
  Mat image = imread("../images/lena.jpg");
  Mat new_image = Mat::zeros( image.size(), image.type() );

  int nRows = image.rows;
  int nChan = image.channels();
  int nCols = image.cols * nChan;

  uchar lut[256];

  std::cout<<" Basic Linear Transforms "<<std::endl;

  std::cout<<"* Enter the alpha value [1.0-3.0]: ";std::cin>>alpha;
  std::cout<<"* Enter the beta value [0-100]: "; std::cin>>beta;

  /* Itinialize Clock*/
  double t = (double)getTickCount();

  /* Initializing Lookup Table */
  for (int i = 0; i < 256; i++)
    lut[i] = saturate_cast<uchar>( (alpha * i) + beta );

  uchar *p, *pNew;
  for (int row = 0; row < nRows; row++)
    {
      p = image.ptr<uchar>(row);
      pNew = new_image.ptr<uchar>(row);
      for (int col = 0; col < nCols; col++)
	{
	  pNew[col] = lut[ p[col] ];
	}
    }


  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout << "Times passed in seconds: " << t << std::endl;

  namedWindow("Original Image", 1);
  namedWindow("New Image", 1);

  imshow("Original Image", image);
  imshow("New Image", new_image);

  waitKey();
  return 0;
}
