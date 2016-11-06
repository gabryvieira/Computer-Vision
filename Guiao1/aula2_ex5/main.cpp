#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;

int main(int argc, char *argv[])
{

       // load image
    double alpha = 0.5; double beta; double input;

    Mat img1, img2, dst;

     // enter alhpa
     std::cout<<" Simple Linear Blender "<<std::endl;
     std::cout<<"* Enter alpha [0-1]: ";
     std::cin>>input;

     if( input >= 0.0 && input <= 1.0 )
       { alpha = input; }

     // read image -> ensure that the type and size of images are the same
     img1 = imread("../images/lena.jpg");
     img2 = imread("../aula2_ex5/Gray_Image.jpg");

     if( !img1.data ) { printf("Error loading image 1 \n"); return -1; }
     if( !img2.data ) { printf("Error loading image 2 \n"); return -1; }

     namedWindow("Result image", 1);
     beta = ( 1.0 - alpha );
     addWeighted( img1, alpha, img2, beta, 0.0, dst);

     imshow( "Result image", dst );

     waitKey(0);
     return 0;
    }



