#ifdef __BORLANDC__
#pragma hdrstop            // borland specific
#include <condefs.h>
#pragma argsused
USEUNIT("Tserial.cpp");
#endif
#include <cv.h>
#include <highgui.h>
#include "Tserial.cpp"
#include <conio.h>
#include<stdio.h>
#include<math.h>
#include <time.h>
#include<iostream>
using namespace std;



int main()
{   char a;
   Tserial *com;
   com = new Tserial();
   com->connect("COM11", 9600, spNONE);                            //check com port
   cvWaitKey(5000);
   while(1)
   {       
           a=com->getChar();
           cout<<a<<endl;
           cvWaitKey(1000);
          // cout<<"i";
   }
}
