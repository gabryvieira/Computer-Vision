/***********************************************************************************
Name:           ex2.cpp
Revision:
Author:         Miguel Azevedo
Comments:       ChessBoard Tracking from the webcam


images
Revision:
Libraries:
***********************************************************************************/
#include <iostream>
#include <vector>

// OpenCV Includes
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Function FindAndDisplayChessboard
// find corners in a cheesboard with board_w x board_h dimensions
// Display the corners in image and return number of detected corners
void DrawCube(cv::Mat *image,
	      int board_w,
	      int board_h,
	      cv::Mat *intrinsic,
	      cv::Mat *distCoeffs,
	      std::vector<cv::Point3f> *cubePoints,
	      std::vector<cv::Point3f> *objectPoints
	     )
{
  int board_size = board_w * board_h;
  CvSize board_sz = cvSize(board_w,board_h);

  cv::Mat grey_image;

  cv::cvtColor(*image, grey_image, CV_BGR2GRAY);

  cv::Mat rvec(3,1,cv::DataType<double>::type);
  cv::Mat tvec(3,1,cv::DataType<double>::type);

  std::vector<cv::Point2f> corners;
  // find chessboard corners
  bool found = cv::findChessboardCorners(grey_image, board_sz, corners, 0);
  if (!found)
    return;

  // cv::Size winSize = cv::Size( 5, 5 );
  // cv::Size zeroZone = cv::Size( -1, -1 );

  // cv::TermCriteria criteria = cv::TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 54, 0.001 );

  // cv::cornerSubPix( grey_image, corners, winSize, zeroZone, criteria );

  cv::solvePnP(*objectPoints, corners, *intrinsic, *distCoeffs, rvec, tvec, false);
  std::vector<cv::Point2f> imagePoints;
  cv::projectPoints(*cubePoints, rvec, tvec, *intrinsic, *distCoeffs, imagePoints);

  // Draw results
  line(*image, imagePoints[0], imagePoints[1], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[0], imagePoints[2], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[1], imagePoints[3], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[2], imagePoints[3], cv::Scalar( 0, 255, 0 ), 2);

  line(*image, imagePoints[4], imagePoints[5], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[4], imagePoints[6], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[5], imagePoints[7], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[6], imagePoints[7], cv::Scalar( 0, 255, 0 ), 2);

  line(*image, imagePoints[0], imagePoints[4], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[1], imagePoints[5], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[2], imagePoints[6], cv::Scalar( 0, 255, 0 ), 2);
  line(*image, imagePoints[3], imagePoints[7], cv::Scalar( 0, 255, 0 ), 2);

}

int main(int argc, char **argv)
{
  cv::FileStorage fs(argv[1], cv::FileStorage::READ);
  if (!fs.isOpened())
    {
      std::cout << "Could not open the configuration file: \""
		<< "../CamParams_subpix.xml"
		<< "\""
		<< std::endl;
      return -1;
    }
  cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
  cv::Mat distCoeffs;
  fs["cameraMatrix"] >> intrinsic;
  fs["distCoeffs"] >> distCoeffs;
  std::cout << "camera matrix: " << intrinsic << std::endl
	    << "distortion coeffs: " << distCoeffs << std::endl;


  // ChessBoard Properties
  int board_w = 9;
  int board_h = 6;

  std::vector<cv::Point3f> objectPoints, cubePoints;
  cubePoints.push_back(cv::Point3f(0.0, 0.0, 0.0));
  cubePoints.push_back(cv::Point3f(0.0, 1.0, 0.0));
  cubePoints.push_back(cv::Point3f(1.0, 0.0, 0.0));
  cubePoints.push_back(cv::Point3f(1.0, 1.0, 0.0));
  cubePoints.push_back(cv::Point3f(0.0, 0.0, -1.0));
  cubePoints.push_back(cv::Point3f(0.0, 1.0, -1.0));
  cubePoints.push_back(cv::Point3f(1.0, 0.0, -1.0));
  cubePoints.push_back(cv::Point3f(1.0, 1.0, -1.0));

  for (int i = 0; i < board_h; i++)
    for (int j = 0; j < board_w; j++)
      objectPoints.push_back(cv::Point3f((float)j,(float)i, 0.0));

  cv::Mat image;
  int i = 0;

  int sucesses = 0;

  cv::VideoCapture cap(0); // open the video camera no. 0
  // Access Video
  if (!cap.isOpened()) // if not success, exit program
    {
      std::cout << "Cannot open the video file" << std::endl;
      getchar();
      return -1;
    }

  while (true)
  {
    cap >> image; // get a new frame from camera
    DrawCube(&image, board_w, board_h,  &intrinsic, &distCoeffs, &cubePoints, &objectPoints);
    imshow("cam", image);
    if(cv::waitKey(30) >= 0) break;
  }
}
