#include<iostream.h>
#include<conio.h>
#include<string.h>

int strtonum(char num[])
{
	int number=0;
	int l;
	l=strlen(num);
	for(int i=0;i<l;i++)
		number=(number*10)+(num[i]-'0');
	return number;
}

void main()
{
char t[6];
strcpy(t,"0123");
int a;
a=strtonum(t);
cout<<a;
getch();
}