#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;


double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

int main( int argc, char** argv )
{
  /// Read image given by user
  Mat image = imread("../images/lena.jpg");
  Mat new_image = Mat::zeros( image.size(), image.type() ); // valores iniciais dos pixeis colocados a zero


  std::cout<<" Basic Linear Transforms "<<std::endl;

  std::cout<<"* Enter the alpha value [1.0-3.0]: ";std::cin>>alpha;
  std::cout<<"* Enter the beta value [0-100]: "; std::cin>>beta;
  double t = (double)getTickCount();

  /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
  // acesso a cada pixel
  for( int y = 0; y < image.rows; y++ )  // percorrer as linhas da imagem
    {
      for( int x = 0; x < image.cols; x++ ) // percorrer as colunas da imagem
	{
	  for( int c = 0; c < 3; c++ ) // os tais 3 valores por pixel (R, G, B) -> Red, Green, Blue (0,1,2)
            {
	      new_image.at<Vec3b>(y,x)[c] =
                saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta ); // assegurar que os valores sao validos
            }
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
