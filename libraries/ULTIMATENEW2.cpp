#ifdef __BORLANDC__
#pragma hdrstop            // borland specific
#include <condefs.h>
#pragma argsused
USEUNIT("Tserial.cpp");
#endif
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "Tserial.cpp"
#include <time.h>
#include <cv.h>
#include <highgui.h>
#include "cvblob.cpp"
#include "cvlabel.cpp"
#include<iostream>
#include<fstream>
#include<string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace cvb;
using namespace std;
#define DELAY 3000            //time delay
#define PIXEL_MIN 200   //Mininum no of pixels in blob
#define PIXEL_MAX 2000    //Max no of pixels in blob 
#define PIXEL_MINCYAN 50
#define PIXEL_MAXCYAN 450   
#define PIXEL_MINGOLD 50
#define PIXEL_MAXGOLD 450   
#define PIXEL_MINWOOD 50
#define PIXEL_MAXWOOD 450
#define PIXEL_MINBOMB 50
#define PIXEL_MAXBOMB 450

#define THETA1 16
#define THETA2 35
#define THETA3 16 
#define THETA4 16
#define THETA5 20
/*#define WALL_MINX 66
#define WALL_MINY 30
#define WALL_MAXX 603
#define WALL_MAXY 453*/

#define NEGLECTX 148
#define NEGLECTY 398

#define NEGLECT_BOMBX 565              //high value
#define NEGLECT_BOMBY 76                    //low value

#define boundaryx 108
#define boundaryy 439

#define bomb_boundaryx 606                     //high value
#define bomb_boundaryy 34                    //low value

#define boundary2x 194
#define boundary2y 362

#define BOMB_AVOIDX_MIN 220                   //middle value
#define BOMB_AVOIDX_MAX 465                  //middle value

#define BOMB_AVOIDY_MIN 385                //high value
#define BOMB_AVOIDY_MAX 463                 //high value


#define grip_open 87
#define grip_close 32

//double BOT_FRONT_BOMBX=160, BOT_FRONT_BOMBY=240; 
double onex, oney, twox, twoy;
//double boundaryx=65, boundaryy=27;
//double bomb_boundaryx=600 , bomb_boundaryy=445;
int gripped=0;
double goldx[3], goldy[3], stonex[3], stoney[3], woodx[3], woody[3], bombx[3], bomby[3];
int goldleft, stoneleft, woodleft, bombleft;
int check_capture=1, counter=0, major=0, bombcheckvar=0;
double alpha=0;

//boundaryx = 50;                   //set the values
//boundaryy = 88;

char calculate(double blackx, double blacky);
char dropper();
char dropperforbomb();

void sort(double arrx[],double arry[], int n)
{
     int i, j;
     double temp;
     for(i=0; i<n-1; i++)
     {
         for(j=0; j<n-1-i; i++)
         {
             if(((arrx[j]-twox)*(arrx[j]-twox)+(arry[j]-twoy)*(arry[j]-twoy))>
             ((arrx[j+1]-twox)*(arrx[j+1]-twox)+(arry[j+1]-twoy)*(arry[j+1]-twoy)))
             {
                       temp=arrx[j];
                       arrx[j]=arrx[j+1];
                       arrx[j+1]=temp;
                       temp=arry[j];
                       arry[j]=arry[j+1];
                       arry[j+1]=temp;
             }  
         }
     }
}


char detectregion(IplImage* img)
{
    IplImage* output_gold;
    IplImage* blob_outputgold;
    IplImage* labelImg_gold;  
    IplImage* output_stone;
    IplImage* blob_outputstone;
    IplImage* labelImg_stone;  
    IplImage* output_wood;
    IplImage* blob_outputwood;
    IplImage* labelImg_wood;
      IplImage* output_bomb;
    IplImage* blob_outputbomb;
    IplImage* labelImg_bomb;  
    IplImage* output_one;
    IplImage* blob_outputone;
    IplImage* labelImg_one;
    IplImage* output_two;
    IplImage* blob_outputtwo;
    IplImage* labelImg_two;

    uchar* pointer;
    uchar* output_pointer_wood;
    uchar* output_pointer_stone;
    uchar* output_pointer_gold;
     uchar* output_pointer_bomb;
    uchar* output_pointer_one;
    uchar* output_pointer_two;

    int i, j, h, s, v, count;
    double temp;   
    
    output_wood=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    output_stone=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    output_gold=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    output_bomb=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1); 
    output_one=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    output_two=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);

    pointer = (uchar*)img->imageData;
    output_pointer_wood = (uchar*)output_wood->imageData;
    output_pointer_stone = (uchar*)output_stone->imageData;
    output_pointer_gold = (uchar*)output_gold->imageData;
      output_pointer_bomb = (uchar*)output_bomb->imageData;
    output_pointer_one = (uchar*)output_one->imageData;
    output_pointer_two = (uchar*)output_two->imageData;

    for(i=0;i<img->height;i++)
            for(j=0;j<img->width;j++)
            {
                   h=pointer[i*img->widthStep + j*img->nChannels+0];
                   s=pointer[i*img->widthStep + j*img->nChannels+1];
                   v=pointer[i*img->widthStep + j*img->nChannels+2];
         
                                       if((h>=85 &&h<=97)&&s>=65&&s<=255 &&v>=235 &&v<=255)//cyan or yellow
                                                                         output_pointer_stone[i*output_stone->widthStep + j]=255;
                                       else
                                                                          output_pointer_stone[i*output_stone->widthStep + j]=0;
                                                                          
                                       if(h>=144 &&h<=160&&s>=6&&v>=235&&v<=255)//green or one
                                                                         output_pointer_one[i*output_one->widthStep + j]=255;
                                       else
                                                                          output_pointer_one[i*output_one->widthStep + j]=0;
                                        if(h>=100 &&h<=114&&s>=80&&v>=110&&v<=255)//two or blue
                                                                         output_pointer_two[i*output_two->widthStep + j]=255;
                                       else
                                                                          output_pointer_two[i*output_two->widthStep + j]=0;
                                        if(((h>=25&&h<=40))&&v>=235&&v<=255)//gold or white
                                                                         output_pointer_gold[i*output_gold->widthStep + j]=255;
                                       else
                                                                          output_pointer_gold[i*output_gold->widthStep + j]=0;
                                                                          
                                      
                                        if(((h>=176&&h<=180)||(h<=10))&&s>=120&&s<=230&&v>=210&&v<=255)//wood or red
                                                                         output_pointer_wood[i*output_wood->widthStep + j]=255;
                                       else
                                                                          output_pointer_wood[i*output_wood->widthStep + j]=0;
                                        if(((h>=165&&h<=180 )||(h<4))&&s>60&&s<130&&v>=95&&v<=250)//bomb or orange
                                                                         output_pointer_bomb[i*output_bomb->widthStep + j]=255;
                                       else
                                                                          output_pointer_bomb[i*output_bomb->widthStep + j]=0;
                                        
                                        
                                                   
               }
            
       cvNamedWindow("output_gold");
      cvShowImage("output_gold", output_gold);
      cvNamedWindow("output_stone");
      cvShowImage("output_stone", output_stone);
      cvNamedWindow("output_wood");
      cvShowImage("output_wood", output_wood);
        cvNamedWindow("output_bomb");
      cvShowImage("output_bomb", output_bomb);

    cvNamedWindow("output_one");
      cvShowImage("output_one", output_one);
  cvNamedWindow("output_two");
      cvShowImage("output_two", output_two);
 //     cvWaitKey(0);
 //     cvDestroyWindow("output_gold");
//      cvDestroyWindow("output_stone");
//      cvDestroyWindow("output_wood");
 //    cvDestroyWindow("output_bomb");   
 //     cvDestroyWindow("output_one");
//      cvDestroyWindow("output_two");
         
          
            
    blob_outputwood=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    blob_outputstone=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    blob_outputgold=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
      blob_outputbomb=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    blob_outputone=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    blob_outputtwo=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    
    labelImg_wood=cvCreateImage(cvGetSize(img),IPL_DEPTH_LABEL,1);
    labelImg_stone=cvCreateImage(cvGetSize(img),IPL_DEPTH_LABEL,1);
    labelImg_gold=cvCreateImage(cvGetSize(img),IPL_DEPTH_LABEL,1);
       labelImg_bomb=cvCreateImage(cvGetSize(img),IPL_DEPTH_LABEL,1);
    labelImg_one=cvCreateImage(cvGetSize(img),IPL_DEPTH_LABEL,1);
    labelImg_two=cvCreateImage(cvGetSize(img),IPL_DEPTH_LABEL,1);

     CvBlobs blobs_one;
    cvLabel(output_one, labelImg_one, blobs_one);
    cvRenderBlobs(labelImg_one, blobs_one, img, blob_outputone);
    cvFilterByArea(blobs_one,PIXEL_MIN,PIXEL_MAX);                                //set pixel_minand pixel_max points
    for(CvBlobs::const_iterator it=blobs_one.begin();it!=blobs_one.end();++it) //red
    {                         
         onex=it->second->centroid.x;
         oney=it->second->centroid.y; 
         //cout<<"one enter"<<endl;                            
    }

    CvBlobs blobs_two;
    cvLabel(output_two, labelImg_two, blobs_two);
    cvRenderBlobs(labelImg_two, blobs_two, img, blob_outputtwo);
    cvFilterByArea(blobs_two,PIXEL_MIN,PIXEL_MAX);                                //set pixel_minand pixel_max points
    for(CvBlobs::const_iterator it=blobs_two.begin();it!=blobs_two.end();++it) //red
    {                         
         twox=it->second->centroid.x;
         twoy=it->second->centroid.y;   
         //cout<<"two enter"<<endl;                          
    }
  
  
    goldleft=0;   
    CvBlobs blobs_gold;
    cvLabel(output_gold, labelImg_gold, blobs_gold);
    cvRenderBlobs(labelImg_gold, blobs_gold, img, blob_outputgold);
    cvFilterByArea(blobs_gold,PIXEL_MINGOLD,PIXEL_MAXGOLD);                                //set pixel_minand pixel_max points
    for(CvBlobs::const_iterator it=blobs_gold.begin();it!=blobs_gold.end();++it) //red
    {                                   
         goldx[goldleft]=it->second->centroid.x;
         goldy[goldleft]=it->second->centroid.y;                             
         if(goldx[goldleft]<NEGLECTX && goldy[goldleft]>NEGLECTY) continue;                  //set the pixel for 15cm
         goldleft++;
    }
   // cout<<"goldx, goldy="<<goldx[0]<<" "<<goldy[0]<<endl;
   //cout<<"goldleft"<<goldleft<<endl;
    sort(goldx, goldy, goldleft);

    stoneleft=0;   
    CvBlobs blobs_stone;
    cvLabel(output_stone, labelImg_stone, blobs_stone);
    cvRenderBlobs(labelImg_stone, blobs_stone, img, blob_outputstone);
    cvFilterByArea(blobs_stone,PIXEL_MINCYAN,PIXEL_MAXCYAN);                                //set pixel_minand pixel_max points
    for(CvBlobs::const_iterator it=blobs_stone.begin();it!=blobs_stone.end();++it) //red
    {                         
         stonex[stoneleft]=it->second->centroid.x;
         stoney[stoneleft]=it->second->centroid.y;                             
         if(stonex[stoneleft]<NEGLECTX && stoney[stoneleft]>NEGLECTY) continue;                  //set the pixel for 15cm
         stoneleft++;
    }
//   cout<<"stoneleft"<<stoneleft<<endl;
    sort(stonex, stoney, stoneleft);
    
    woodleft=0;   
    CvBlobs blobs_wood;
    cvLabel(output_wood, labelImg_wood, blobs_wood);
    cvRenderBlobs(labelImg_wood, blobs_wood, img, blob_outputwood);
    cvFilterByArea(blobs_wood,PIXEL_MINWOOD,PIXEL_MAXWOOD);                                //set pixel_minand pixel_max points
    for(CvBlobs::const_iterator it=blobs_wood.begin();it!=blobs_wood.end();++it) //red
    {                         
         woodx[woodleft]=it->second->centroid.x;
         woody[woodleft]=it->second->centroid.y;                             
         if(woodx[woodleft]<NEGLECTX && woody[woodleft]>NEGLECTY) continue;                  //set the pixel for 15cm  
         woodleft++;
    }
 //  cout<<"woodleft"<<woodleft<<endl;
    //cvWaitKey(0);
    sort(woodx, woody, woodleft);
 
      bombleft=0;   
    CvBlobs blobs_bomb;
    cvLabel(output_bomb, labelImg_bomb, blobs_bomb);
    cvRenderBlobs(labelImg_bomb, blobs_bomb, img, blob_outputbomb);
    cvFilterByArea(blobs_bomb,PIXEL_MINBOMB,PIXEL_MAXBOMB);                                //set pixel_minand pixel_max points
    for(CvBlobs::const_iterator it=blobs_bomb.begin();it!=blobs_bomb.end();++it) //red
    {                         
         bombx[bombleft]=it->second->centroid.x;
         bomby[bombleft]=it->second->centroid.y;                             
         if(bombx[bombleft]>NEGLECT_BOMBX && bomby[bombleft]<NEGLECT_BOMBY)
         { 
                                          continue;                  //set the pixel for end blocks
         }
         bombleft++;
    }
  // cout<<"bombleft="<<bombleft<<endl;
   // cvWaitKey(0);
    sort(bombx, bomby, bombleft);

    
    
    cvReleaseImage(&output_gold);
    cvReleaseImage(&blob_outputgold);
    cvReleaseImage(&labelImg_gold);
    cvReleaseImage(&output_stone);
    cvReleaseImage(&blob_outputstone);
    cvReleaseImage(&labelImg_stone);
    cvReleaseImage(&output_wood);
    cvReleaseImage(&blob_outputwood);
    cvReleaseImage(&labelImg_wood);
      cvReleaseImage(&output_bomb);
    cvReleaseImage(&blob_outputbomb);
    cvReleaseImage(&labelImg_bomb);
    cvReleaseImage(&output_one);
    cvReleaseImage(&output_two);
    cvReleaseImage(&blob_outputone);
    cvReleaseImage(&blob_outputtwo);
    cvReleaseImage(&labelImg_one);
    cvReleaseImage(&labelImg_two);
    cvWaitKey(1);

  // cout<<"one="<<onex<<" "<<oney<<endl;
  //  cout<<"two="<<twox<<" "<<twoy<<endl;
   /* cout<<"yellow="<<blackx<<" "<<blacky<<endl;
    cvWaitKey(0);
  */ 

     if(gripped==0)
    {
         if(goldleft>0)
         {
               bombcheckvar=0;        
               return calculate(goldx[0], goldy[0]);
         }    
         else if(stoneleft>0)
         {
              bombcheckvar=0;
               return calculate(stonex[0], stoney[0]);
         }
         else if(woodleft>0)
         {
              bombcheckvar=0;
               return calculate(woodx[0], woody[0]);
         }  
         else if(bombleft>0)
         {
              bombcheckvar=1;
              return calculate(bombx[0], bomby[0]);
         }  
    }
    if(gripped==1)
    {
          if(bombcheckvar==0)
                  return dropper();
          else 
                  return dropperforbomb();
    } 

    
}

char dropperforbomb()
{
     double m1, m2, theta, distance;
     cout<<"major="<<major<<endl;
       m1=(twoy-oney)/(twox-onex);
               m1=atan(m1)*180/3.14159;
               if((twoy-oney)<0 && (twox-onex)>0)
               {
                   m1=m1+ 360;
               }
               else if((twox-onex)<0)
               {
                     m1=m1+ 180;
               } 
  
               m2=(bomb_boundaryy-oney)/(bomb_boundaryx-onex);
               m2=atan(m2)*180/3.14159;
                if((bomb_boundaryy-oney)<0 && (bomb_boundaryx-onex)>0)
                {
                       m2=m2+ 360;
                }
                else if((bomb_boundaryx-onex)<0)
                {
                   m2=m2+ 180;
                }         
       
               theta=m1-m2;
               if(theta>180)
                  theta=theta-360;
               if(theta<-180)
                   theta=theta+360;
   //            cout<<"m1, m2, theta"<<m1<<", "<<m2<<", "<<theta<<endl;
               distance=(bomb_boundaryx-twox)*(bomb_boundaryx-twox)+ (bomb_boundaryy-twoy)*(bomb_boundaryy-twoy);
               distance=sqrt(distance);
   //           cout<<"distance_drop="<<distance<<endl;
               if(distance>60)
               {
                   /*  if(twoy<WALL_MINY && (m1<=20 || (m1>=300 && m1<=360)))
                             return 'x';
                     else if(twoy<WALL_MINY && m1>=160 && m1<=240)
                             return 'y';
                     else if(twoy<WALL_MINY && m1>240 && m1<=300)
                             return 'b';
                     else if(twoy>WALL_MAXY && (m1<=60 || (m1>=340 && m1<=360)))
                             return 'y';
                     else if(twoy>WALL_MAXY && m1>=120 && m1<=200)
                             return 'x';
                     else if(twoy>WALL_MAXY && m1>=60 && m1<=120)
                             return 'b';
                     else if(twox>WALL_MAXX && m1>=30 && m1<=110)
                             return 'x';
                     else if(twox>WALL_MAXX && m1>=250 && m1<=330)
                             return 'y';                        
                     else if(twox>WALL_MAXX && (m1<=30 || (m1>=330 && m1<=360)))
                             return 'b';
                     else if(twox<WALL_MINX && m1>=70 && m1<=150)
                             return 'y';
                     else if(twox<WALL_MINX && m1>=150 && m1<=210)
                             return 'b';
                     else if(twox<WALL_MINX && m1>=210 && m1<=290)
                             return 'x';*/
                      if(theta>THETA1 && theta<THETA2 && alpha==0)                                                  //acw or left turn
                     {
                          alpha=1;
                          return 'L';                                                             
                     }
                     else if(theta<-THETA1 && theta>-THETA2 && alpha==0)                                             //cw or right turn
                     {
                          alpha=1;
                          return 'R';
                     }
                     else if(theta<THETA1 && theta >-THETA1 && alpha==0)                               //forward 
                     {
                          return 'F';
                     }
                     else if(theta>THETA3 && theta<THETA2 && alpha==1)
                     {
                          return 'L';
                     }
                     else if(theta<-THETA3 && theta>-THETA2 && alpha==1)
                     {
                          return 'R';
                     }
                     else if(theta<THETA3 && theta>-THETA3 && alpha==1)
                     {
                          alpha=0;
                          return 'F';
                     }
                     else if(theta>THETA2)
                     {
                          return 'l';
                     }
                     else if(theta<-THETA2)
                     {
                          return 'r';
                     }
               }
               else if(distance>28 && distance<=60)
               {
                       if(theta>THETA4 && theta<THETA2)                                                  //acw or left turn
                       {
                                     
                                      return 'L';                                                             
                       }
                       else if(theta<-THETA4 && theta>-THETA2)                                             //cw or right turn
                       {
                             
                                   return 'R';
                       }
                       else if(theta<THETA4 && theta >-THETA4)                               //forward 
                       {
                                return 'f';
                       }
                       else if(theta>THETA2)
                       {
                                return 'l';
                       }
                       else if(theta<-THETA2)
                       {
                             return 'r';
                       }
               }
               else if(distance<=28)
               {
             //       if(theta<THETA5 && theta>-THETA5)
              //      {
                                                                                     //so that code runs enough time for dropping
                                  gripped=0;
                                  return 'w';
               //     }
                /*    else if(theta>THETA5)
                         return 'L';
                    else if(theta<-THETA5)
                         return 'R';*/
                }
}


char dropper()
{
   //  cvWaitKey(0);
     double m1, m2, theta, distance;
     cout<<"major="<<major<<endl;
            if(major==0)
             {               
                m1=(twoy-oney)/(twox-onex);
               m1=atan(m1)*180/3.14159;
               if((twoy-oney)<0 && (twox-onex)>0)
               {
                   m1=m1+ 360;
               }
               else if((twox-onex)<0)
               {
                     m1=m1+ 180;
               } 
  
               m2=(boundary2y-oney)/(boundary2x-onex);
               m2=atan(m2)*180/3.14159;
                if((boundary2y-oney)<0 && (boundary2x-onex)>0)
                {
                       m2=m2+ 360;
                }
                else if((boundary2x-onex)<0)
                {
                   m2=m2+ 180;
                }         
       
               theta=m1-m2;
               if(theta>180)
                  theta=theta-360;
               if(theta<-180)
                   theta=theta+360;
     //          cout<<"m1, m2, theta"<<m1<<", "<<m2<<", "<<theta<<endl;
               distance=(boundary2x-twox)*(boundary2x-twox)+ (boundary2y-twoy)*(boundary2y-twoy);
               distance=sqrt(distance);
     //         cout<<"distance_drop="<<distance<<endl;
               if(distance>60)
               {
                    /* if(twoy<WALL_MINY && (m1<=20 || (m1>=300 && m1<=360)))
                             return 'x';
                     else if(twoy<WALL_MINY && m1>=160 && m1<=240)
                             return 'y';
                     else if(twoy<WALL_MINY && m1>240 && m1<=300)
                             return 'b';
                     else if(twoy>WALL_MAXY && (m1<=60 || (m1>=340 && m1<=360)))
                             return 'y';
                     else if(twoy>WALL_MAXY && m1>=120 && m1<=200)
                             return 'x';
                     else if(twoy>WALL_MAXY && m1>=60 && m1<=120)
                             return 'b';
                     else if(twox>WALL_MAXX && m1>=30 && m1<=110)
                             return 'x';
                     else if(twox>WALL_MAXX && m1>=250 && m1<=330)
                             return 'y';                        
                     else if(twox>WALL_MAXX && (m1<=30 || (m1>=330 && m1<=360)))
                             return 'b';
                     else if(twox<WALL_MINX && m1>=70 && m1<=150)
                             return 'y';
                     else if(twox<WALL_MINX && m1>=150 && m1<=210)
                             return 'b';
                     else if(twox<WALL_MINX && m1>=210 && m1<=290)
                             return 'x';*/
                       if(twox<BOMB_AVOIDX_MAX && twox>BOMB_AVOIDX_MIN && twoy>BOMB_AVOIDY_MIN && twoy<BOMB_AVOIDY_MAX && m1>80 && m1<190)
                             return 'r';  
                else if(twox<BOMB_AVOIDX_MAX && twox>BOMB_AVOIDX_MIN && twoy>BOMB_AVOIDY_MIN && twoy<BOMB_AVOIDY_MAX && m1>190 && m1<240)
                             return 'f';   
                                                              
                else if(theta>THETA1 && theta<THETA2 && alpha==0)                                                  //acw or left turn
     
                     {
                          alpha=1;
                          return 'L';                                                             
                     }
                     else if(theta<-THETA1 && theta>-THETA2 && alpha==0)                                             //cw or right turn
                     {
                          alpha=1;
                          return 'R';
                     }
                     else if(theta<THETA1 && theta >-THETA1 && alpha==0)                               //forward 
                     {
                          return 'F';
                     }
                     else if(theta>THETA3 && theta<THETA2 && alpha==1)
                     {
                          return 'L';
                     }
                     else if(theta<-THETA3 && theta>-THETA2 && alpha==1)
                     {
                          return 'R';
                     }
                     else if(theta<THETA3 && theta>-THETA3 && alpha==1)
                     {
                          alpha=0;
                          return 'F';
                     }
                     else if(theta>THETA2)
                     {
                          return 'l';
                     }
                     else if(theta<-THETA2)
                     {
                          return 'r';
                     }
               }
               else if(distance>28 && distance<=60)
               {
                       if(theta>THETA4 && theta<THETA2)                                                  //acw or left turn
                       {
                                     
                                      return 'L';                                                             
                       }
                       else if(theta<-THETA4 && theta>-THETA2)                                             //cw or right turn
                       {
                             
                                   return 'R';
                       }
                       else if(theta<THETA4 && theta >-THETA4)                               //forward 
                       {
                                return 'f';
                       }
                       else if(theta>THETA2)
                       {
                                return 'l';
                       }
                       else if(theta<-THETA2)
                       {
                             return 'r';
                       }
               }
               else if(distance<=28)
               {
                     major=1;
               }             
                            
     }      
            if(major==1)
            {
               m1=(twoy-oney)/(twox-onex);
               m1=atan(m1)*180/3.14159;
               if((twoy-oney)<0 && (twox-onex)>0)
               {
                   m1=m1+ 360;
               }
               else if((twox-onex)<0)
               {
                     m1=m1+ 180;
               } 
  
               m2=(boundaryy-oney)/(boundaryx-onex);
               m2=atan(m2)*180/3.14159;
                if((boundaryy-oney)<0 && (boundaryx-onex)>0)
                {
                       m2=m2+ 360;
                }
                else if((boundaryx-onex)<0)
                {
                   m2=m2+ 180;
                }         
       
               theta=m1-m2;
               if(theta>180)
                  theta=theta-360;
               if(theta<-180)
                   theta=theta+360;
     //          cout<<"m1, m2, theta"<<m1<<", "<<m2<<", "<<theta<<endl;
               distance=(boundaryx-twox)*(boundaryx-twox)+ (boundaryy-twoy)*(boundaryy-twoy);
               distance=sqrt(distance);
     //         cout<<"distance_drop="<<distance<<endl;
               if(distance>60)
               {
                    /* if(twoy<WALL_MINY && (m1<=20 || (m1>=300 && m1<=360)))
                             return 'x';
                     else if(twoy<WALL_MINY && m1>=160 && m1<=240)
                             return 'y';
                     else if(twoy<WALL_MINY && m1>240 && m1<=300)
                             return 'b';
                     else if(twoy>WALL_MAXY && (m1<=60 || (m1>=340 && m1<=360)))
                             return 'y';
                     else if(twoy>WALL_MAXY && m1>=120 && m1<=200)
                             return 'x';
                     else if(twoy>WALL_MAXY && m1>=60 && m1<=120)
                             return 'b';
                     else if(twox>WALL_MAXX && m1>=30 && m1<=110)
                             return 'x';
                     else if(twox>WALL_MAXX && m1>=250 && m1<=330)
                             return 'y';                        
                     else if(twox>WALL_MAXX && (m1<=30 || (m1>=330 && m1<=360)))
                             return 'b';
                     else if(twox<WALL_MINX && m1>=70 && m1<=150)
                             return 'y';
                     else if(twox<WALL_MINX && m1>=150 && m1<=210)
                             return 'b';
                     else if(twox<WALL_MINX && m1>=210 && m1<=290)
                             return 'x';*/
                                                              
                     if(twox<BOMB_AVOIDX_MAX && twox>BOMB_AVOIDX_MIN && twoy>BOMB_AVOIDY_MIN && twoy<BOMB_AVOIDY_MAX && m1>80 && m1<190)
                             return 'r';  
                else if(twox<BOMB_AVOIDX_MAX && twox>BOMB_AVOIDX_MIN && twoy>BOMB_AVOIDY_MIN && twoy<BOMB_AVOIDY_MAX && m1>190 && m1<240)
                             return 'f';   
                     else if(theta>THETA1 && theta<THETA2 && alpha==0)                                                  //acw or left turn
     
                     {
                          alpha=1;
                          return 'L';                                                             
                     }
                     else if(theta<-THETA1 && theta>-THETA2 && alpha==0)                                             //cw or right turn
                     {
                          alpha=1;
                          return 'R';
                     }
                     else if(theta<THETA1 && theta >-THETA1 && alpha==0)                               //forward 
                     {
                          return 'F';
                     }
                     else if(theta>THETA3 && theta<THETA2 && alpha==1)
                     {
                          return 'L';
                     }
                     else if(theta<-THETA3 && theta>-THETA2 && alpha==1)
                     {
                          return 'R';
                     }
                     else if(theta<THETA3 && theta>-THETA3 && alpha==1)
                     {
                          alpha=0;
                          return 'F';
                     }
                     else if(theta>THETA2)
                     {
                          return 'l';
                     }
                     else if(theta<-THETA2)
                     {
                          return 'r';
                     }
               }
               else if(distance>28 && distance<=60)
               {
                       if(theta>THETA4 && theta<THETA2)                                                  //acw or left turn
                       {
                                     
                                      return 'L';                                                             
                       }
                       else if(theta<-THETA4 && theta>-THETA2)                                             //cw or right turn
                       {
                             
                                   return 'R';
                       }
                       else if(theta<THETA4 && theta >-THETA4)                               //forward 
                       {
                                return 'f';
                       }
                       else if(theta>THETA2)
                       {
                                return 'l';
                       }
                       else if(theta<-THETA2)
                       {
                             return 'r';
                       }
               }
               else if(distance<=28)
               {
             //       if(theta<THETA5 && theta>-THETA5)
              //      {
                                                                                        //so that code runs enough time for dropping
                                  gripped=0;
                                  return 'w';
               //     }
                /*    else if(theta>THETA5)
                         return 'L';
                    else if(theta<-THETA5)
                         return 'R';*/
               }
     }
}


char calculate(double blackx, double blacky)
{
     double m1, m2, theta, distance;
     m1=(twoy-oney)/(twox-onex);
     m1=atan(m1)*180/3.14159;
     if((twoy-oney)<0 && (twox-onex)>0)
     {
          m1=m1+ 360;
     }
     else if((twox-onex)<0)
     {
          m1=m1+ 180;
     }  
     
     m2=(blacky-oney)/(blackx-onex);
     m2=atan(m2)*180/3.14159;
     if((blacky-oney)<0 && (blackx-onex)>0)
     {
          m2=m2+ 360;
     }
     else if((blackx-onex)<0)
     {
          m2=m2+ 180;
     }
     theta=m1-m2;
     if(theta>180)
                  theta=theta-360;
     if(theta<-180)
                   theta=theta+360;
 //    cout<<"m1, m2, theta"<<m1<<", "<<m2<<", "<<theta<<endl;
     distance=(blackx-twox)*(blackx-twox)+ (blacky-twoy)*(blacky-twoy);
     distance=sqrt(distance);
     cout<<"distance="<<distance<<endl;
  //  cvWaitKey(0);
     if(distance>=grip_open)
     {
      /*if(twoy<WALL_MINY && (m1<=20 || (m1>=300 && m1<=360)))
                             return 'x';
                     else if(twoy<WALL_MINY && m1>=160 && m1<=240)
                             return 'y';
                     else if(twoy<WALL_MINY && m1>240 && m1<=300)
                             return 'b';
                     else if(twoy>WALL_MAXY && (m1<=60 || (m1>=340 && m1<=360)))
                             return 'y';
                     else if(twoy>WALL_MAXY && m1>=120 && m1<=200)
                             return 'x';
                     else if(twoy>WALL_MAXY && m1>=60 && m1<=120)
                             return 'b';
                     else if(twox>WALL_MAXX && m1>=30 && m1<=110)
                             return 'x';
                     else if(twox>WALL_MAXX && m1>=250 && m1<=330)
                             return 'y';                        
                     else if(twox>WALL_MAXX && (m1<=30 || (m1>=330 && m1<=360)))
                             return 'b';
                     else if(twox<WALL_MINX && m1>=70 && m1<=150)
                             return 'y';
                     else if(twox<WALL_MINX && m1>=150 && m1<=210)
                             return 'b';
                     else if(twox<WALL_MINX && m1>=210 && m1<=290)
                             return 'x'; */
 if(bombcheckvar==0 && twox<BOMB_AVOIDX_MAX && twox>BOMB_AVOIDX_MIN && twoy>BOMB_AVOIDY_MIN && twoy<BOMB_AVOIDY_MAX && ((m1>0 && m1<100) || (m1>350 && m1<360)))
                             return 'l';  
else if(bombcheckvar==0 && twox<BOMB_AVOIDX_MAX && twox>BOMB_AVOIDX_MIN && twoy>BOMB_AVOIDY_MIN && twoy<BOMB_AVOIDY_MAX && m1>300 && m1<350)
                             return 'f';

     else if(theta>THETA1 && theta<THETA2 && alpha==0)                                                  //acw or left turn
     {     
           alpha=1;
           return 'L';    
     //      cvWaitKey(10);                                                         
     }
     else if(theta<-THETA1 && theta>-THETA2 && alpha==0)                                             //cw or right turn
     {
          alpha=1;
          return 'R';
     //     cvWaitKey(10);
     }
   
     else if(theta<THETA1 && theta >-THETA1 && alpha==0)                               //forward 
     {
          
          return 'F';
     //     cvWaitKey(10);
     }
       else if(theta>THETA3 && theta<THETA2 && alpha==1)
     {
          return 'L';
     }
     else if(theta<-THETA3 && theta>-THETA2 && alpha==1)
     {
          return 'R';
     }
     else if(theta<THETA3 && theta>-THETA3 && alpha==1)
     {
          alpha=0;
          return 'F';
     }
     else if(theta>THETA2)
     {
          return 'l';
     //     cvWaitKey(10);
     }
     else if(theta<-THETA2)
     {
          return 'r';
     //     cvWaitKey(10);
     }
     }
     else if(distance> grip_close && distance< grip_open)
     {
          if(counter==0)
          {
                        counter=1;
                        return 'D';
          }
       if(theta>THETA4 && theta<THETA2)                                                  //acw or left turn
       {
                     
             return 'L';                                                             
       }
       else if(theta<-THETA4 && theta>-THETA2)                                             //cw or right turn
       {
          
             return 'R';
       }
       else if(theta<THETA4 && theta >-THETA4)                               //forward 
       {
            return 'f';
       }
   
       else if(theta>THETA2)
       {
            return 'l';
       }
       else if(theta<-THETA2)
       {
          return 'r';
       }
     }
     else if(distance<= grip_close)
     {
          if(theta<THETA5 && theta>-THETA5)
          {
               major=0;
               gripped=1;
               counter=0;
               cout<<"gripped="<<gripped<<endl;
                return 'g';
              // Sleep(4);                                                         //wait for holding
              
          }
          else if(theta>THETA5)
          {
               return 'L';
          }
          else if(theta<-THETA5)
          {
               return 'R';
          }
     }

}


int main()
{
    int check_capture=1;
    int z;
    char done, done2;
    Tserial *com;
    com = new Tserial();
    com->connect("COM9",4800, spNONE);       // set the com port, also the baud rate
    CvCapture *capture=cvCreateCameraCapture(0);//initiate camera    
 //   com->sendChar('d');
    for(z=0;z<200000000&&capture!=NULL;z++);//starting camera takes some time 
    
    if(capture==NULL)
    {
                     cout<<"camera not initialised\n";
                     return -1;
    }
    
    while(check_capture!=0)
    {
  //  cout<<"hi\n";
   // IplImage* img1;
    IplImage* img;
    IplImage* img_hsv;
    CvCapture *capture=cvCreateCameraCapture(0);//initiate camera 
    img=cvQueryFrame(capture);//take current image in camera and give it to frame pointer
  //  cout<<"hi2\n";
    cvNamedWindow("Picture");
    cvShowImage("Picture",img);
    //cvSetImageROI(img1, cvRect(150,77,800,800));              //set the values accordingly
    //img = cvCreateImage(cvGetSize(img1),img1->depth,img1->nChannels);
    /* copy subimage */
    //cvCopy(img1,img, NULL);
    //cvResetImageROI(img1);    

    img_hsv=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);              //Conversion to hsv
    cvCvtColor(img,img_hsv,CV_BGR2HSV);
    done=detectregion(img_hsv);
    if(done=='w')
    {
                 com->sendChar('d');
                 //cout<<"hi"<<endl;
                 for(int k=0; k<=800; k++)
                  com->sendChar('B');
                  //cout<<"asdfgh"<<endl;
                  com->sendChar('G');
//                 cvWaitKey(10);
    }
  
    com->sendChar(done);
/*    if(done=='d')
    {
                 for(int k=0; k<=800; k++)
                  com->sendChar('b');
               //   com->sendChar('g');
                  //cout<<"hi"<<endl;
//                 cvWaitKey(10);
    }*/
    //cvReleaseImage(&img1);
    cvReleaseImage(&img);
    cvReleaseImage(&img_hsv);
  //  cout<<"hello\n";
  //  cvWaitKey(0);
 //   cvDestroyWindow("Picture");
  //  cout<<"hello2\n";
  //  cvWaitKey(1);
    }
    
}
