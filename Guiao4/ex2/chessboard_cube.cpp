/***********************************************************************************
Name:           chessboard_cube.cpp
Revision:
Author:         Paulo Dias, Miguel Azevedo
Comments:       ChessBoard tracking with wireframe cube projection.


images
Revision:
Libraries:
***********************************************************************************/
#include <iostream>
#include <vector>
#include <stdlib.h>

// OpenCV Includes
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//using namespace cv;
//using namespace std;

// Function FindAndDisplayChessboard
// find corners in a cheesboard with board_w x board_h dimensions
// Display the corners in image and return number of detected corners
int DrawCube(cv::Mat image,
	     int board_w,
	     int board_h,
	     std::vector<cv::Point2f> *corners,
	     cv::Mat tvec,
	     cv::Mat rvec,
	     cv::Mat *intrinsic,
	     cv::Mat *distCoeffs
	     )
{
  int board_size = board_w * board_h;
  CvSize board_sz = cvSize(board_w,board_h);

  cv::Mat grey_image;

  cv::cvtColor(image, grey_image, CV_BGR2GRAY);

  // find chessboard corners
  bool found = cv::findChessboardCorners(grey_image, board_sz, *corners,0);
  cv::Size winSize = cv::Size( 5, 5 );
  cv::Size zeroZone = cv::Size( -1, -1 );

  cv::TermCriteria criteria = cv::TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 54, 0.001 );

  cv::cornerSubPix( grey_image, *corners, winSize, zeroZone, criteria );

  std::vector<cv::Point3f> linePoints;
  linePoints.push_back(cv::Point3f(0.0, 0.0, 0.0));
  linePoints.push_back(cv::Point3f(0.0, 1.0, 0.0));
  linePoints.push_back(cv::Point3f(1.0, 0.0, 0.0));
  linePoints.push_back(cv::Point3f(1.0, 1.0, 0.0));
  linePoints.push_back(cv::Point3f(0.0, 0.0, 1.0));
  linePoints.push_back(cv::Point3f(0.0, 1.0, 1.0));
  linePoints.push_back(cv::Point3f(1.0, 0.0, 1.0));
  linePoints.push_back(cv::Point3f(1.0, 1.0, 1.0));

  std::vector<cv::Point2f> imagePoints;
  cv::projectPoints(linePoints, rvec, tvec, *intrinsic, *distCoeffs, imagePoints);

  // Draw results
  line(image, imagePoints[0], imagePoints[1], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[0], imagePoints[2], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[1], imagePoints[3], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[2], imagePoints[3], cv::Scalar( 0, 255, 0 ), 2);

  line(image, imagePoints[4], imagePoints[5], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[4], imagePoints[6], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[5], imagePoints[7], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[6], imagePoints[7], cv::Scalar( 0, 255, 0 ), 2);

  line(image, imagePoints[0], imagePoints[4], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[1], imagePoints[5], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[2], imagePoints[6], cv::Scalar( 0, 255, 0 ), 2);
  line(image, imagePoints[3], imagePoints[7], cv::Scalar( 0, 255, 0 ), 2);
  cv::imshow("Cube",image);
  printf("\n Number of corners: %lu",corners->size());
  cv::waitKey(0);

  return corners->size();
}

// Function FindAndDisplayChessboard
// find corners in a cheesboard with board_w x board_h dimensions
int FindChessboard(cv::Mat image,int board_w,int board_h, std::vector<cv::Point2f> *corners)
{
  int board_size = board_w * board_h;
  CvSize board_sz = cvSize(board_w,board_h);

  cv::Mat grey_image;

  cv::cvtColor(image, grey_image, CV_BGR2GRAY);

  // find chessboard corners
  bool found = cv::findChessboardCorners(grey_image, board_sz, *corners,0);
  cv::Size winSize = cv::Size( 5, 5 );
  cv::Size zeroZone = cv::Size( -1, -1 );

  cv::TermCriteria criteria = cv::TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 54, 0.001 );

  cornerSubPix( grey_image, *corners, winSize, zeroZone, criteria );
  return corners->size();
}

int main(int argc, char **argv)
{
  // ChessBoard Properties
  int n_boards = 13; //Number of images
  int board_w = 9;
  int board_h = 6;

  int board_sz = board_w * board_h;

  char filename[200];

  // Chessboard coordinates and image pixels
  std::vector<std::vector<cv::Point3f> > object_points;
  std::vector<std::vector<cv::Point2f> > image_points;

  // Corners detected in each image
  std::vector<cv::Point2f> corners;

  int corner_count;

  cv::Mat image;
  int i;

  int sucesses = 0;

  //
  cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
  cv::Mat distCoeffs;
  std::vector<cv::Mat> rvecs;
  std::vector<cv::Mat> tvecs;

  // chessboard coordinates
  std::vector<cv::Point3f> obj;
  for(int j=0;j<board_sz;j++)
    obj.push_back(cv::Point3f(float(j/board_w), float(j%board_w), 0.0));

  for (i=0;i<n_boards;i++)
  {
    // read image
    sprintf(filename,"..//images//left%02d.jpg",i+1);
    printf("\nReading %s",filename);
    image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);


    if(!image.data)
    {
      printf("\nCould not load image file: %s\n",filename);
	  getchar();
      return 0;
    }

	// find and display corners
    corner_count = FindChessboard(image,board_w,board_h,&corners);

    if (corner_count == board_w * board_h)
      {
	image_points.push_back(corners);
	object_points.push_back(obj);
	sucesses++;
      }
  }
  calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

  for (i=0;i<n_boards;i++)
  {
    // read image
    sprintf(filename,"..//images//left%02d.jpg",i+1);
    printf("\nReading %s",filename);
    image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);


    if(!image.data)
    {
      printf("\nCould not load image file: %s\n",filename);
	  getchar();
      return 0;
    }

    // draw wireframe cube
    DrawCube(image, board_w, board_h, &corners, tvecs.at(i), rvecs.at(i), &intrinsic, &distCoeffs);
  }
}
