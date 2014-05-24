#include <cv.h>
#include <iostream>
#include<time.h>
#include"highgui.h"
#define IMAGEWIDTH  (640)
#define IMAGEHEIGHT (480)

int main(void)
{
    IplImage *img;
    unsigned int x, y, channels, imgstep;
     char *imgData;

    img = cvCreateImage(cvSize(IMAGEWIDTH, IMAGEHEIGHT),IPL_DEPTH_8U,3);

    channels = img->nChannels;
    imgstep  = img->widthStep / sizeof (unsigned char); // Values per row
    imgData  = img->imageData;

    for (y = 0; y < (img->height); y++)
    {
        for (x = 0; x < (img->width); x++)
        {
            imgData[(y * imgstep) + (x * channels) + 0] = 255;  // Blue
            imgData[(y * imgstep) + (x * channels) + 1] = 128;  // Green
            imgData[(y * imgstep) + (x * channels) + 2] = 0;    // Red
        }
    }
    cvNamedWindow("Picture");
   // cvWaitKey(50);
    cvShowImage("Picture",img);
    cvWaitKey(10000);
    //cvDestroyWindow("Picture");
    getchar();
    cvReleaseImage(&img );
  return 0;
    }
