#include"highgui.h"
#include <cv.h>

IplImage *temp;

void my_mouse_callback( int event, int x, int y, int flags, void* param )
{
   uchar *ptemp  =  (uchar*)temp->imageData;
printf("\nx=%d(%d)\t y=%d(%d)\t\th=%d \ts=%d \tv=%d\n",x,temp->width,y,temp->height,ptemp[y*temp->widthStep + x*temp->nChannels+0],ptemp[y*temp->widthStep + x*temp->nChannels+1],ptemp[y*temp->widthStep + x*temp->nChannels+2]);
} 

int main()
{
    int c;//to store ascii value of key pressed
    int i,j,h,s,v;
    CvCapture *capture=cvCreateCameraCapture(0);//initiate camera 
    //because of recursively updating frame , here we dont need to wait for camera as for some ms frame will be black and then as camera starts , frame will update and shaw image
    IplImage *frame;
    
     IplImage* outputred;
   IplImage* outputone;
   IplImage* outputtwo;
  // IplImage* outputblue;
   IplImage* outputwhite;
  // IplImage* outputorange;


    uchar *ptemp;
   // uchar *poutputorange;
    uchar *poutputred;
    uchar *poutputwhite;
   // uchar *poutputblue;
    uchar *poutputone;
    uchar *poutputtwo;
   
   
    if(capture!=NULL)
    {
                     //frame=cvQueryFrame(capture);//take current image in camera and give it to frame pointer
                    frame=cvLoadImage("blob.jpg");
                     cvNamedWindow("img");
                     while(1)
                     {
                             
                             cvShowImage("img",frame);
                           // frame=cvQueryFrame(capture);
                            frame=cvLoadImage("blob.jpg");
                            temp=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);
                            cvCvtColor(frame,temp,CV_BGR2HSV);
                            //frame rate time period (if not given system will hang as system processing speed is very fast
                           // cvNamedWindow("output",1);
                            //cvShowImage("output",temp);
                            cvSetMouseCallback("img", my_mouse_callback, NULL);
                            c=cvWaitKey(1);
                            outputred=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
     outputone=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
      outputtwo=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
     //  outputblue=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
        outputwhite=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
     //    outputorange=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
    
    cvCvtColor(frame,temp,CV_BGR2HSV);
    ptemp  =  (uchar*)temp->imageData;
    poutputone  =  (uchar*)outputone->imageData;
    poutputtwo  =  (uchar*)outputtwo->imageData;
   // poutputblue  =  (uchar*)outputblue->imageData;
    poutputwhite  =  (uchar*)outputwhite->imageData;
    poutputred  =  (uchar*)outputred->imageData;
  //  poutputorange  =  (uchar*)outputorange->imageData;
    
     for(i=0;i<frame->height;i++)
            for(j=0;j<frame->width;j++)
            {
                                       h=ptemp[i*temp->widthStep + j*temp->nChannels+0];
                                       s=ptemp[i*temp->widthStep + j*temp->nChannels+1];
                                       v=ptemp[i*temp->widthStep + j*temp->nChannels+2];
                                       
                                         if((h>=157&&h<=178)&&s>=110 && s<=255 &&v>=90)//red
                                                                         poutputred[i*outputred->widthStep + j]=255;
                                       else
                                                                          poutputred[i*outputred->widthStep + j]=0;
                                       if((h==0 && s==0 &&v<150 && v>9)||(h>=25 &&h<=110 && s>=20&&s<=200&& v>=13 && v<=120))//one
                                                                         poutputone[i*outputone->widthStep + j]=255;
                                       else
                                                                          poutputone[i*outputone->widthStep + j]=0;
                                  
                               /*       if((h>=145 &&h<=160)&&s>=175 && s<=255 && v>=80 && v<=150)//one
                                                                         poutputone[i*outputone->widthStep + j]=255;
                                       else
                                                                          poutputone[i*outputone->widthStep + j]=0;
                                 */                                         
                                       if(h>=110 &&h<=153&&s>=90&&v>=7 && v<=150)//two
                                                                         poutputtwo[i*outputtwo->widthStep + j]=255;
                                       else
                                                                          poutputtwo[i*outputtwo->widthStep + j]=0;
                                          if( (h==0 && s==0 && v>=250) || (((h>=0 && h<=30)) && s>=50&&s<=200&&v>=110) )//white 
                                                                         poutputwhite[i*outputwhite->widthStep + j]=255;
                                       else
                                                                          poutputwhite[i*outputwhite->widthStep + j]=0;
                                   
                                                                          
            }
     
    //cvNamedWindow("output",1);
    cvNamedWindow("outputred",1);
    cvNamedWindow("outputone",1);
    cvNamedWindow("outputtwo",1);
  //  cvNamedWindow("outputblue",1);
    cvNamedWindow("outputwhite",1);
    //cvNamedWindow("outputorange",1);

    //cvShowImage("output",temp);
    cvShowImage("outputred",outputred);
    cvShowImage("outputone",outputone);
    cvShowImage("outputtwo",outputtwo);
   // cvShowImage("outputblue",outputblue);
    cvShowImage("outputwhite",outputwhite);
   // cvShowImage("outputorange",outputorange);
    cvWaitKey(1);
 /*   imgOutred=cvCreateImage(cvGetSize(input),IPL_DEPTH_8U,3);
    labelImgred=cvCreateImage(cvGetSize(input),IPL_DEPTH_LABEL,1);
    CvBlobs blobsred;
    cvLabel(outputred, labelImgred, blobsred);
    cvRenderBlobs(labelImgred, blobsred, input, imgOutred);
    cvFilterByArea(blobsred,PIXEL_MIN,PIXEL_MAX);

    imgOutone=cvCreateImage(cvGetSize(input),IPL_DEPTH_8U,3);
    labelImgone=cvCreateImage(cvGetSize(input),IPL_DEPTH_LABEL,1);
    CvBlobs blobsone;
    cvLabel(outputone, labelImgone, blobsone);
    cvRenderBlobs(labelImgone, blobsone, input, imgOutone);
    cvFilterByArea(blobsone,PIXEL_MIN,PIXEL_MAX);

    imgOuttwo=cvCreateImage(cvGetSize(input),IPL_DEPTH_8U,3);
    labelImgtwo=cvCreateImage(cvGetSize(input),IPL_DEPTH_LABEL,1);
    CvBlobs blobstwo;
    cvLabel(outputtwo, labelImgtwo, blobstwo);
    cvRenderBlobs(labelImgtwo, blobstwo, input, imgOuttwo);
    cvFilterByArea(blobstwo,PIXEL_MIN,PIXEL_MAX);

    imgOutblue=cvCreateImage(cvGetSize(input),IPL_DEPTH_8U,3);
    labelImgblue=cvCreateImage(cvGetSize(input),IPL_DEPTH_LABEL,1);
    CvBlobs blobsblue;
    cvLabel(outputblue, labelImgblue, blobsblue);
    cvRenderBlobs(labelImgblue, blobsblue, input, imgOutblue);
    cvFilterByArea(blobsblue,PIXEL_MIN,PIXEL_MAX);
    
    imgOutwhite=cvCreateImage(cvGetSize(input),IPL_DEPTH_8U,3);
    labelImgwhite=cvCreateImage(cvGetSize(input),IPL_DEPTH_LABEL,1);
    CvBlobs blobswhite;
    cvLabel(outputwhite, labelImgwhite, blobswhite);
    cvRenderBlobs(labelImgwhite, blobswhite, input, imgOutwhite);
    cvFilterByArea(blobswhite,PIXEL_MINWHITE,PIXEL_MAX);
    
   */ 
    
    
    
    
    cvReleaseImage( &outputred ); 
    cvReleaseImage( &outputone ); 
    cvReleaseImage( &outputtwo ); 
    //cvReleaseImage( &outputblue ); 
    cvReleaseImage( &outputwhite ); 
    //cvReleaseImage( &outputorange );
                            cvReleaseImage(&temp ); 
                            //cvDestroyWindow( "output" );
                            if(c>0&&c<255)
                                     {
                                            cvDestroyWindow( "img" );
                                            cvReleaseImage( &frame ); 
                                            cvReleaseCapture(&capture);
     //                                       cvDestroyWindow( "outputred" );
                                            //cvDestroyWindow( "output" );
  //  cvDestroyWindow( "outputone" );
   // cvDestroyWindow( "outputtwo" );
    //cvDestroyWindow( "outputblue" );
   // cvDestroyWindow( "outputwhite" );
    //cvDestroyWindow( "outputorange" );
    
    cvReleaseImage( &outputred ); 
    cvReleaseImage( &outputone ); 
    cvReleaseImage( &outputtwo ); 
    //cvReleaseImage( &outputblue ); 
    cvReleaseImage( &outputwhite ); 
    //cvReleaseImage( &outputorange ); 
     
    
                                            return 0;
                                     }
                     }
    }
}
