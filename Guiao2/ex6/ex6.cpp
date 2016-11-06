#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;



void RGBHistogram(VideoCapture cap){


    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera;

        /// separa a imagem em R, G, B
        vector<Mat> bgr_planes;
        split( frame, bgr_planes );

        /// Establish the number of bins
        int histSize = 256;

        /// Set the ranges ( for B,G,R) )
        float range[] = { 0, 256 } ;
        const float* histRange = { range };

        /// We want our bins to have the same size (uniform) and to clear the histograms in the beginning
        bool uniform = true;
        bool accumulate = false;

        /// matrizes para onde vai cada histograma de cada channel
        Mat b_hist, g_hist, r_hist;

        /// calcular os histogramas

        calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

        /// Draw the histograms for B, G and R
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize ); /// cvRound --> arredonda para as unidades

        Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

        /// Normalize the result to [ 0, histImage.rows ]
        ///before drawing, we first normalize the histogram so its values fall in the range indicated by the parameters entered:
                // input   // output                  // tipo normalizacao
        normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() ); // -1 dType quando negativo a saida do array tem o mesmo tipo que a src
        normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

        /// Draw for each channel
        for( int i = 1; i < histSize; i++ )
        {
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                               Scalar( 255, 0, 0), 2, 8, 0  );
              line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                               Scalar( 0, 255, 0), 2, 8, 0  );
              line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                               Scalar( 0, 0, 255), 2, 8, 0  );
        }

        namedWindow("Original image", WINDOW_AUTOSIZE);
        imshow("Original image", frame);
        /// Display
        namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
        imshow("calcHist Demo", histImage );

        if(waitKey(30) >= 0) break;
    }
}

void GrayHistogram(VideoCapture cap){


    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, frame, COLOR_BGR2GRAY);

        imshow("Gray image", frame);
        // Set histogram bins count
        int bins = 256;
        int histSize[] = {bins};
        // Set ranges for histogram bins
        float lranges[] = {0, 256};
        const float* ranges[] = {lranges};
        // create matrix for histogram
        cv::Mat hist;
        int channels[] = {0};

        // create matrix for histogram visualization
        int const hist_height = 256;
        Mat3b hist_image = Mat3b::zeros(hist_height, bins);

        calcHist(&frame, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);
	normalize(hist, hist, 0, hist_image.rows, NORM_MINMAX, -1, Mat() );
        double max_val=0;
        minMaxLoc(hist, 0, &max_val);
	
        // visualize each bin
        for(int b = 0; b < bins; b++) {
            float const binVal = hist.at<float>(b);
            int   const height = cvRound(binVal*hist_height/max_val);
            line( hist_image, Point(b, hist_height-height), cv::Point(b, hist_height), Scalar::all(255));
        }
        imshow("Histograma", hist_image);

        if(waitKey(30) >= 0) break;
    }

}

int main(int argc, char *argv[])
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    int op;
    cout << "Choose if you want a RGB or Gray scale histogram of the image\n";
    cout << "1 - RGB\n";
    cout << "2 - Gray Scale\n";
    cout << "Option --> ";
    cin >> op;

    switch(op){
        case 1:
        RGBHistogram(cap);
        break;

        case 2:
        GrayHistogram(cap);
        break;
    }

    return 0;
}
