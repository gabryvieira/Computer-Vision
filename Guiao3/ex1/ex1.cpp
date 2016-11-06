#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    /// declaracao variaveis
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    int c;

        /// declaracao matrizes
    Mat image_gray; // imagem em tons de cinza
    Mat gradient;  // imagem em gradiente
    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;


    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    for(;;){
        Mat frame;
        cap >> frame;

        GaussianBlur( frame, frame, Size(3,3), 0, 0, BORDER_DEFAULT );

        /// Convert it to gray
        cvtColor( frame, image_gray, CV_BGR2GRAY);

         /// Gradient X
        //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
        Sobel( image_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT ); /// explorar paramentros da funcao
        convertScaleAbs( grad_x, abs_grad_x ); //Scales, calculates absolute values, and converts the result to 8-bit.

        /// Gradient Y
        //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
        Sobel( image_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_y, abs_grad_y ); //Scales, calculates absolute values, and converts the result to 8-bit.
        /// Total Gradient (approximate)
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradient ); /// adciona as imagens e guarda na matriz gradient com os respectivos pesos x e y


        imshow( "Gradient image", gradient );
        imshow("Original", frame);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}



