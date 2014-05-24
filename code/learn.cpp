
//#include <iostream.h>
#include"highgui.h"
#include <cv.h>

//using namespace cvb;
//using namespace std;



     


int main()
{
    IplImage* img;
    IplImage* img_hsv;
    CvCapture *capture=cvCreateCameraCapture(0);
    img=cvQueryFrame(capture);
     //cvShowImage("Picture",img);
      cvWaitKey(4000);
    img=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
   img_hsv=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);  
 
     while(1)
     { 
     
        img=cvQueryFrame(capture);
        cvCvtColor(img,img_hsv,CV_BGR2HSV);
        cvNamedWindow("Picture");
        cvShowImage("Picture",img_hsv);
        cvWaitKey(4);
        
        //cvReleaseImage( &img );
       // cvDestroyWindow("Picture");    
    
    //getchar();
}
return 0;    
}
