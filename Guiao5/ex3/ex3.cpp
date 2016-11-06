#include <iostream>
#include <vector>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <unistd.h>
 
using namespace cv;
using namespace std;


 void readXMLFile(cv::FileStorage fs, cv::Mat cameraMatrix1, cv::Mat distCoeffs1, cv::Mat cameraMatrix2, 
    cv::Mat distCoeffs2, cv::Mat R, cv::Mat T, cv::Mat E, cv::Mat F){
        std::cout << "Reading stereo parameters" << std::endl;
        fs["cameraMatrix_1"] >> cameraMatrix1;
        fs["distCoeffs_1"] >> distCoeffs1;
        fs["cameraMatrix_2"] >> cameraMatrix2;
        fs["distCoeffs_2"] >> distCoeffs2;
        fs["rotation"] >> R;
        fs["translation"] >> T;
        fs["essential"] >> E;
        fs["fundamental"] >> F;
        fs.release();

       /* cout << endl << "cameraMatrix1" << cameraMatrix1 << endl;
        cout << endl << "distCoeffs1" << distCoeffs1 << endl;        
        cout << endl << "cameraMatrix2" << cameraMatrix2 << endl;
        cout << endl << "distCoeffs2" << distCoeffs2 << endl;
        cout << endl << "rotation" << R << endl;
        cout << endl << "translation" << T << endl;
        cout << endl << "essential" << E << endl;
        cout << endl << "fundamental" << F << endl;*/ 

 }
// write to XML
void writetoXMLFile(cv::FileStorage fs, cv::Mat cameraMatrix1, cv::Mat distCoeffs1, cv::Mat cameraMatrix2, 
    cv::Mat distCoeffs2, cv::Mat R, cv::Mat T, cv::Mat E, cv::Mat F){

    

    std::cout << "Writing stereo parameters" << std::endl;
        fs << "cameraMatrix_1" << cameraMatrix1;
        fs << "distCoeffs_1" << distCoeffs1;
        fs << "cameraMatrix_2" << cameraMatrix2;
        fs << "distCoeffs_2" << distCoeffs2;
        fs << "rotation" << R;
        fs << "translation" << T;
        fs << "essential" << E;
        fs << "fundamental" << F;
        fs.release();
}
 
// Function FindAndDisplayChessboard
// find corners in a cheesboard with board_w x board_h dimensions
// Display the corners in image and return number of detected corners
int FindAndDisplayChessboard(cv::Mat image,int board_w,int board_h, std::vector<cv::Point2f> *corners){
 
    int board_size = board_w * board_h;
    CvSize board_sz = cvSize(board_w,board_h);
 
    cv::Mat grey_image;
 
    cv::cvtColor(image, grey_image, CV_BGR2GRAY);
 
    // find chessboard corners
    bool found = cv::findChessboardCorners(grey_image, board_sz, *corners,0);
 
    // Draw results
    if (true){
        cv::drawChessboardCorners(image, board_sz, cv::Mat(*corners), found);
        cv::imshow("Calibration",image);
        //printf("\n Number of corners: %d",corners->size());
        cv::waitKey(0);
    }
    return corners->size();
}
 


int main(int argc, char **argv){ 
 
  // ChessBoard Properties
    int n_boards = 13; //Number of images
    int board_w = 9;
    int board_h = 6;
 
    int board_sz = board_w * board_h;
 
    char filename[200];
 
    // Chessboard coordinates and image pixels
    std::vector<std::vector<cv::Point3f> > object_points;
    std::vector<std::vector<cv::Point2f> > image_pointsL, image_pointsR ;
 
    // Corners detected in each image
    std::vector<cv::Point2f> cornersR, cornersL;
 
    int corner_countL, corner_countR;
 
    cv::Mat imageL, imageR;
    int i;
 
    int sucesses = 0;
 
    // chessboard coordinates
    std::vector<cv::Point3f> obj;
    for(int j=0;j<board_sz;j++)
    obj.push_back(cv::Point3f(float(j/board_w), float(j%board_w), 0.0));
 
 
 
    for (i=0;i<n_boards;i++){
        // read image
        sprintf(filename,"images/left%02d.jpg",i+1);
        printf("\nReading %s",filename);
        imageR = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
 
 
        if(!imageR.data){
            printf("\nCould not load image file: %s\n",filename);
            getchar();
            return 0;
        }
 
        // find and display corners
        corner_countR = FindAndDisplayChessboard(imageR,board_w,board_h,&cornersR);
 
 
        sprintf(filename,"images/right%02d.jpg",i+1);
        printf("\nReading %s",filename);
        imageL = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
 
 
        if(!imageL.data){
            printf("\nCould not load image file: %s\n",filename);
            getchar();
            return 0;
        }
 
 
        corner_countL = FindAndDisplayChessboard(imageL,board_w,board_h,&cornersL);
 
        if (corner_countL == board_w * board_h){
            image_pointsL.push_back(cornersL);
            object_points.push_back(obj);
            sucesses++;
        }
 
        if (corner_countR == board_w * board_h){
            image_pointsR.push_back(cornersR);
            //object_pointsR.push_back(obj);
            sucesses++;
        }
 
    }
 
 
    Mat cameraMatrix1, distCoeffs1;
    Mat cameraMatrix2, distCoeffs2;
 
    Mat R, T, E, F;
  
        // calculate rms (stereocalibrate) igual usar imageL.size ou right, porque tamanho e o mesmo
    double rms = stereoCalibrate(object_points, image_pointsL, image_pointsR,
            cameraMatrix1, distCoeffs1,
            cameraMatrix2, distCoeffs2,
            imageL.size(), R, T, E, F,
            TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 50, 1e-6),
            CV_CALIB_FIX_FOCAL_LENGTH );
 
 
    cout <<  endl <<"done with RMS error=" << rms << endl;
        
        // ler parametros do ficheiro existente do exercicio anterior
    cv::FileStorage fs("stereoParams.xml", cv::FileStorage::READ);
 
    if (!fs.isOpened()){
        std::cout << "Failed to read stereoParams.xml" << std::endl;
        return 1;
    }

    else
        readXMLFile(fs, cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, R, T, E, F);
 
    Mat imageUndistortedL;
    for (i=0;i<n_boards;i++){
        // read image
        sprintf(filename,"images/left%02d.jpg",i+1);
        printf("\nReading %s\n",filename);
        imageL = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
 
        undistort(imageL, imageUndistortedL, cameraMatrix1, distCoeffs1);
                
        imshow("Original", imageL);
        imshow("Undistorted", imageUndistortedL);
        cv::waitKey(-1);
    }

    return 0;
}
