#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

struct file_distance {
    int id;
    float distance;
};

int compare_distances (file_distance f1, file_distance f2)
{
  // float diff = f1.distance - f2.distance;
  // return (diff < FLT_EPSILON) && (-diff < FLT_EPSILON);
  return (f1.distance < f2.distance);
}

void HSVHistogram (Mat* src, MatND* hist_src)
{
  Mat src_hsv;

  // /// Using 50 bins for hue and 60 for saturation
  int h_bins = 50;
  int s_bins = 60;
  int histSize[] = { h_bins, s_bins };

  // hue varies from 0 to 179, saturation from 0 to 255
  float h_ranges[] = { 0, 180 };
  float s_ranges[] = { 0, 256 };

  const float* ranges[] = { h_ranges, s_ranges };

  // Use the o-th and 1-st channels
  int channels[] = { 0, 1 };

  cvtColor( *src, src_hsv, COLOR_BGR2HSV );

  calcHist( &src_hsv, 1, channels, Mat(), *hist_src, 2, histSize, ranges, true, false );
  normalize( *hist_src, *hist_src, 0, 1, NORM_MINMAX, -1, Mat() );
}

/**
 * @function main
 */
int main ( int argc, char** argv )
{
  if( argc < 4 )
    {
      printf("** Error. Usage: ./compareHist_Demo <image0> <image1> ... <imageN>\n");
      printf("   Image 1 is the base image\n");
      return -1;
    }

  int n_images = argc-1;
  Mat srcs[n_images];
  MatND hists[n_images];
  file_distance distances[4][n_images]; //methods * images

  int sorted[n_images];

  /// Load three images with different environment settings
  for ( int i = 0; i < n_images; i++ )
    srcs[i] = imread( argv[i + 1], 1 );

  /// Calculate the histograms for the HSV images
  for (int i = 0; i < n_images; i++)
    HSVHistogram( &srcs[i], &hists[i] );

  /// Apply the histogram comparison methods
  for( int compare_method = 0; compare_method < 4; compare_method++ )
    {
      for (int img = 0; img < n_images; img++)
	{
	  distances[compare_method][img].distance = compareHist( hists[0],
								 hists[img],
								 compare_method );
	  distances[compare_method][img].id = img;
	}

      //Output results
      printf( " Method [%d]\n", compare_method);

      std::sort(distances[compare_method],
		distances[compare_method] + n_images,
		&compare_distances);

      for (int idx = 0; idx < n_images; idx++)
	printf( " %s : %f\n",
		argv[ distances[compare_method][idx].id + 1 ],
		distances[compare_method][idx].distance);
    }

  printf( "Done \n" );

  return 0;
}
