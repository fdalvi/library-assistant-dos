#include<iostream.h>
#include<conio.h>

void main()
{
int Y,M,D;
float UT;
clrscr();
unsigned int t;
cin>>Y>>M>>D>>UT;
float JD;
t=(100*Y+M-190002.5);
JD = 367*Y - int((7*(Y+int((M+9)/12)))/4) + int((275*M)/9) + D + 1721013.5 + UT/24 ;
cout<<"\n\n"<<JD+720896 ;
getch();
}