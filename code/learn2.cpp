//OpenCV Headers
#include<cv.h>
#include<highgui.h>
//Input-Output
#include<stdio.h>
//Blob Library Headers
#include"cvblob.h"
//NameSpaces

#define NEGLECTX 148
#define NEGLECTY 398
using namespace cvb;
using namespace std;
double bluex,bluey;
int blueleft;

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
         
                                       if((h>=110 &&h<=120)&&s>=250&&s<=260 &&v>=50 &&v<=125)//cyan or yellow
                                                                         output_pointer_blue[i*output_blue->widthStep + j]=255;
                                       else
                                                                          output_pointer_blue[i*output_blue->widthStep + j]=0;
             }
     
     
     cvNamedWindow("output_blue");
      cvShowImage("output_blue", output_blue);
          blueleft=0;   
  /*  CvBlobs blobs_blue;
    cvLabel(output_blue, labelImg_blue, blobs_blue);
    cvRenderBlobs(labelImg_blue, blobs_blue, img, blob_outputblue);
//    cvFilterByArea(blobs_blue,PIXEL_MINBLUE ,PIXEL_MAXBLUE);                                //set pixel_minand pixel_max points
    for(CvBlobs::const_iterator it=blobs_blue.begin();it!=blobs_blue.end();++it) //red
           {                                   
                 bluex[blueleft]=it->second->centroid.x;
                 bluey[blueleft]=it->second->centroid.y;
                 cout<< bluex[blueleft]<< bluey[blueleft]<<endl;                           
                if(bluex[blueleft]<NEGLECTX && bluey[blueleft]>NEGLECTY) continue;                  //set the pixel for 15cm
                blueleft++;
            }*/
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
  
  
  IplImage* blue;
  IplImage* blue_hsv;

 CvCapture *capture=cvCreateCameraCapture(0);     
  img=cvQueryFrame(capture);  
 // img=cvLoadImage("blob.jpg");
  img=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
   img_hsv=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3); 
  //img_hsv=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    
    while(1)
    {
        img=cvQueryFrame(capture);
        cvCvtColor(img,img_hsv,CV_BGR2HSV);
        cvNamedWindow("Picture");
        cvShowImage("Picture",img_hsv);
        cvWaitKey(4);
        
        
        
        detectblob(img_hsv);
    }
}    
    
    

