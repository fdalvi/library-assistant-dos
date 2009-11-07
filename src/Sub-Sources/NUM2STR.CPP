#include<iostream.h>
#include<conio.h>

void nts(int num,char s[])
{
	for(int i=4;i>=0;i--)
	{
		s[i]=(num%10+'0');
		num=num/10;
	}
	s[5]='\0';
}

void main()
{
clrscr();
int a=101;
char sa[6];
nts(a,sa);
cout<<sa;
getch();
}

