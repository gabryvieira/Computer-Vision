#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {


    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 int iLowH = 0;
 int iHighH = 179;

 int iLowS = 0;
 int iHighS = 255;

 int iLowV = 0;
 int iHighV = 255;

 //Create trackbars in "Control" window
 // feita mais tarde a conversao para a gama HSV onde H [22-38] para detetar objectos vermelhos,
 // [0-22] para objectos laranjas, etc
 cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Control", &iHighH, 179);

 cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

 cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Control", &iHighV, 255);

  VideoCapture cap(0); // open the default camera
      if(!cap.isOpened())  // check if we succeeded
         return -1;

 int iLastX = -1;
 int iLastY = -1;

 Mat imgLines;
  //Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
    for (;;)
     {

          Mat frame;
          cap >> frame;
                //Mat imgOriginal;

          Mat imgHSV;

          cvtColor(frame, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

          Mat imgThresholded;

          inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

            //morphological opening (remove small objects from the foreground)
          erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
          dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

          //morphological closing (fill small holes in the foreground)
          dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
          erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

              //Calculate the moments of the thresholded image
          Moments oMoments = moments(imgThresholded);

          double dM01 = oMoments.m01;
          double dM10 = oMoments.m10;
          double dArea = oMoments.m00;

            // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
          if (dArea > 10000)
          {
           //calculate the position of the ball
           int posX = dM10 / dArea;
           int posY = dM01 / dArea;

           if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
           {
            //Draw a red line from the previous point to the current point
            line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(255,255,0), 2);
           }

           iLastX = posX;
           iLastY = posY;
          }

          imshow("Thresholded Image", imgThresholded); //show the thresholded image

          frame = frame + imgLines; // desenhar as linhas para o movimento do objecto
          imshow("Original", frame); //show the original image

                if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
               {
                    cout << "esc key is pressed by user" << endl;
                    break;
               }
    }

   return 0;

}
