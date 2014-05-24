#ifdef __BORLANDC__
#pragma hdrstop            // borland specific
#include <condefs.h>
#pragma argsused
USEUNIT("Tserial.cpp");
#endif
#include <conio.h>
#include<stdio.h>
#include<math.h>
#include <time.h>
#include<iostream.h>
#include "Tserial.cpp"
#include"Tserial.h"
using namespace std;

int main()
{
   char a;

   Tserial *com;
   com = new Tserial();
  // com->connect("\\\\.\\COM57", 9600, spNONE);                            //check com port
   
   // cin>>a;
           /*a=com->getChar();
           cout<<a<<endl;
           b=getchar();*/
           while(1)
           {
                   // a=getchar(); 
                   cin>>a;
                   com->connect("\\\\.\\COM57", 9600, spNONE);
                   com->sendChar(a);
                   com->disconnect();
           }
                 
   //  return 0;
         
   
   
}
