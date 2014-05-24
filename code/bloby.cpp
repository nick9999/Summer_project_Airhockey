//OpenCV Headers
#include<cv.h>
#include<highgui.h>
//Input-Output
#include<stdio.h>
//Blob Library Headers
#include"cvblob.h"
//NameSpaces


using namespace cvb;
using namespace std;


void detectblob(IplImage* img)
{
     int i,h,s,v,j;
     IplImage* output_blue;
    IplImage* blob_outputblue;
    IplImage* labelImg_blue;
    IplImage* output_one;
    
    uchar* pointer;
    
    uchar* output_pointer_blue;
    
    output_blue=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    
    pointer = (uchar*)img->imageData;
    
    output_pointer_blue = (uchar*)output_blue->imageData;
    
    for(i=0;i<img->height;i++)
            for(j=0;j<img->width;j++)
            {
                   h=pointer[i*img->widthStep + j*img->nChannels+0];
                   s=pointer[i*img->widthStep + j*img->nChannels+1];
                   v=pointer[i*img->widthStep + j*img->nChannels+2];
         
                                       if((h>=95 &&h<=105)&&s>=252&&s<=255 &&v>=230 &&v<=255)//blue
                                                                         output_pointer_blue[i*output_blue->widthStep + j]=255;
                                       else
                                                                          output_pointer_blue[i*output_blue->widthStep + j]=0;
             }
     
     
     cvNamedWindow("output_blue");
      cvShowImage("output_blue", output_blue);
      //cvWaitKey(5);
}   
      /*blob_outputblue=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
      
      labelImg_blue=cvCreateImage(cvGetSize(img),IPL_DEPTH_LABEL,1);
      
      CvBlobs blobs_one;
    cvLabel(output_one, labelImg_one, blobs_one);
    cvRenderBlobs(labelImg_one, blobs_one, img, blob_outputone);}*/
      
      

int main()
{
  IplImage* img;
  IplImage* img_hsv;
  
    
  img=cvLoadImage("blob.jpg");
  
  img_hsv=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    
    while(1)
    {
          
        cvNamedWindow("Picture");
        cvShowImage("Picture",img);
        cvWaitKey(4);
        
        cvCvtColor(img,img_hsv,CV_BGR2HSV);
        
        detectblob(img_hsv);
        
        
    }
    
    cvReleaseImage(&img_hsv);
}    
    
    

