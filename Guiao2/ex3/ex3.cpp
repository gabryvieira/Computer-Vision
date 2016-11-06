#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main (int argc, char** argv)
{


    Mat img=imread("../../images/ja.jpg",CV_LOAD_IMAGE_COLOR);
    Mat dest;

    if(!img.data)
    {
        cout<<"could not open or find the image"<<endl;
        return -1;
    }


        // conversao RGB ---> HSV
    cvtColor(img, dest, COLOR_RGB2HSV_FULL, 0);
    Mat channel[3];
    split(dest, channel); // ler valores da imagem
    imshow("channel0", channel[0]);
    imshow("channel1", channel[1]);
    imshow("channel2", channel[2]);
    namedWindow("HSV",0);
    imshow("HSV",dest);


    Mat rangeChannel0;
    inRange(channel[0], 154, 160, rangeChannel0);
    imshow("range0", rangeChannel0);

    Vec3b minRange = Vec3b(154, 0, 0);
    Vec3b maxRange = Vec3b(160, 255, 255);
    inRange(dest, minRange, maxRange, dest);
    imshow("range hsv", dest);
    //img = img & dest;

    Mat mask;
    vector<Mat> ch;
    ch.push_back(rangeChannel0);
    ch.push_back(rangeChannel0);
    ch.push_back(rangeChannel0);
    merge(ch, mask);
    bitwise_and(img, mask, img);
    imshow("maks", mask);

    namedWindow("Original",0);
    imshow("Result chromacy image",img);


    waitKey(0);

}

