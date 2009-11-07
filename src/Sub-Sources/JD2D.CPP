#include<iostream.h>
#include<conio.h>

void main()
{
int y,m,d;
long int j;
cin>>j;
j=j-1721119;
y=(4*j-1)/146097;
j=4*j-1-146097*y;
d=j/4;
j=(4*d+3)/1461;
d=4*d+3-1461*j;
d=(d+4)/4;
m=(5*d-3)/153;
d=5*d-3-153*m;
d=(d+5)/5;
y=100*y+j;
if(m<10)
	m=m+3;
else
{
	m=m-9;
	y=y+1;
}
d=d+1;
cout<<"\n\n"<<y<<" "<<m<<" "<<d;
getch();
}
