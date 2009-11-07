#include<fstream.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<string.h>
#include<stdio.h>

unsigned int mouse_x, mouse_y, mouse_b;   //Variables for Mouse
int choice,quit=1,readb,readm;

/* Structures And Classes*\
\* ====================== */
struct LOGS
{
	char date[9];
	char action[50];
}log;

class SETTINGS
{
public:
	char username[20];
	char password[20];
	int total_books, total_members, loaned_books;
	int loan_days, min_copies, fine_per_day;
	void addbook();
	void delbook();
	void addmember();
	void delmember();
	void addloan();
	void delloan();
}sdb;

class BOOK_DB
{
private:
	char bcode[6], title[20], author[20], category[20];
	int status;
	char member[10],duedate[9],mcode[6];
	int noc;
public:
	void add();
	void showtable(int mode);
	void show(char code[],int mode);
	void rtitle(char str[],int &stat);
	int loan(char due[]);
	float ret();
}bdb;

class MEMBER_DB
{
private:
	char mcode[6], first[10], last[10], phone[10], email[25], address1[15], address2[15], address3[15];
	int status,nlog;
	char book[20],bcode[6];
public:
	void add();
	void showtable(int mode);
	void show(char code[],int mode);
	void rname(char str[],int &stat);
	void loan(char str[],char code[],char Bname[],char Bcode[]);
	void ret(char Bcode[]);
	void rcode(char str[]);
}mdb;

/* Functions *\
\* ========= */
//Mouse Functions
int detect_mouse();                       	//Checks Availability of Mouse
void show_mouse();			  	//Shows Mouse Cursor
void hide_mouse();                        	//Hides Mouse Cursor
void test_mouse();                       	//Checks if a button is pressed
void set_mouse_speed(int sx,int sy);      	//Sets Mouse Speed
void move_mouse(int x,int y);             	//Moves Mouse Cursor

//Program Modules
void loading();                           	//Loading Screen
void login();				  	//Login Screen
void logindraw(char username[],char password[]);//To Draw the Login Screen
int mainpage();					//Main Screen
void mainpagedraw();				//To Draw the Main Page
void lend();					//Lending Screen
void returns();					//Return Screen
void settings();				//Settings Screen
int tablebutton(int);				//Buttons for Tables
int modify();					//Modify Database Page
void help();                                    //Help/About Pop-Up
int exit();			      	  	//Exit Screen

//Calendar and Info Bars
void calendar();				//Calender Box
void infobox();					//Info Box
void utime(int color=7);				//Time Updater

//Input/Output Functions
void inputstr(int x,int y,char str[]);    	//To Input a String
void inputpass(int x,int y,char str[]);    	//To Input a Password
void outpassxy(int x,int y,char pass[]);        //To Print Password
void numtostr(int number,char num[],int l=6);	//To Convert Number to String
int strtonum(char num[]);			//To Convert String to Number
void outnum(int x,int y,int num,int spacing=8); //To Print Numbers

//Date Conversion Functions
float gdtojd(char date[]); 			//To Convert from Gregorian to Julian
void jdtogd(char date[],float jd);              //To Convert from Julian to Gregorian

int main()
{
//Changing to Graphics Mode
int gdriver=DETECT,gmode;
initgraph(&gdriver,&gmode,"C:\\TC\\BGI");
//initgraph(&gdriver,&gmode,"D:\\PROGRA~1\\TC\\BGI");
setbkcolor(7);
loading();
login();
if(::quit==0)
	return 0;
do
{
	choice=mainpage();
	switch(choice)
	{
	case 1:
		lend();
		break;
	case 2:
		returns();
		break;
	case 3:
		bdb.showtable(0);
		break;
	case 4:
		mdb.showtable(0);
		break;
	case 5:
		int ch;
		do
		{
			ch=modify();
			switch(ch)
			{
			case 1:
				bdb.add();
				break;
			case 2:
				bdb.showtable(2);
				break;
			case 3:
				bdb.showtable(1);
				break;
			case 4:
				mdb.add();
				break;
			case 5:
				mdb.showtable(2);
				break;
			case 6:
				mdb.showtable(1);
				break;
			}
		}while(ch!=0);
		break;
	case 6:
		settings();
		break;
	case 7:
		break;
	}
}
while(::quit!=0);
return 0;
}

/* Class Definitions *\
\* ================= */
//SETTINGS
void SETTINGS::addbook()
{
	ifstream abook("CONFIG.DAT");
	abook.read((char*)&sdb,sizeof(sdb));
	abook.close();
	ofstream abook1("CONFIG.DAT");
	sdb.total_books++;
	abook1.write((char*)&sdb,sizeof(sdb));
	abook1.close();
}

void SETTINGS::delbook()
{
	ifstream dbook("CONFIG.DAT");
	dbook.read((char*)&sdb,sizeof(sdb));
	dbook.close();
	ofstream dbook1("CONFIG.DAT");
	sdb.total_books--;
	dbook1.write((char*)&sdb,sizeof(sdb));
	dbook1.close();
}

void SETTINGS::addmember()
{
	ifstream amem("CONFIG.DAT");
	amem.read((char*)&sdb,sizeof(sdb));
	amem.close();
	ofstream amem1("CONFIG.DAT");
	sdb.total_members++;
	amem1.write((char*)&sdb,sizeof(sdb));
	amem1.close();
}

void SETTINGS::delmember()
{
	ifstream dmem("CONFIG.DAT");
	dmem.read((char*)&sdb,sizeof(sdb));
	dmem.close();
	ofstream dmem1("CONFIG.DAT");
	sdb.total_members--;
	dmem1.write((char*)&sdb,sizeof(sdb));
	dmem1.close();
}

void SETTINGS::addloan()
{
	ifstream aloan("CONFIG.DAT");
	aloan.read((char*)&sdb,sizeof(sdb));
	aloan.close();
	ofstream aloan1("CONFIG.DAT");
	sdb.loaned_books++;
	aloan1.write((char*)&sdb,sizeof(sdb));
	aloan1.close();
}

void SETTINGS::delloan()
{
	ifstream dloan("CONFIG.DAT");
	dloan.read((char*)&sdb,sizeof(sdb));
	dloan.close();
	ofstream dloan1("CONFIG.DAT");
	sdb.loaned_books--;
	dloan1.write((char*)&sdb,sizeof(sdb));
	dloan1.close();
}

//BOOK_DB
void BOOK_DB::add()
{
	cleardevice();
	setcolor(6);
	settextstyle(3,0,4);
	outtextxy(220,5,"Add Book");
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(3,0,2);
	outtextxy(50,440,"< Back");
	rectangle(40,444,130,465);
	outtextxy(520,440,"Add >");
	rectangle(510,444,600,465);
	setcolor(1);
	outtextxy(50,100,"Book Code  ");
	outtextxy(50,140,"Book Name  ");
	outtextxy(50,180,"Author     ");
	outtextxy(50,220,"Category   ");
	outtextxy(50,260,"Copies     ");
	rectangle(250,100,550,130);
	rectangle(250,140,550,170);
	rectangle(250,180,550,210);
	rectangle(250,220,550,250);
	rectangle(250,260,550,290);
	//Initialising Variables to Null
	char tnoc[6];
	tnoc[0]='\0';
	bcode[0]='\0';
	title[0]='\0';
	author[0]='\0';
	category[0]='\0';
	delay(150);
	if (detect_mouse())
		show_mouse();
	while (::quit==1)
	{
		test_mouse();
		if((mouse_x>40)&&(mouse_x<130)&&(mouse_y>444)&&(mouse_y<465))
		{
			setcolor(6);
			rectangle(39,443,131,466);
		}
		else
		if((mouse_x>510)&&(mouse_x<600)&&(mouse_y>444)&&(mouse_y<465))
		{
			setcolor(6);
			rectangle(509,443,601,466);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>100)&&(mouse_y<130))
		{
			setcolor(1);
			rectangle(249,99,551,131);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>140)&&(mouse_y<170))
		{
			setcolor(1);
			rectangle(249,139,551,171);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>180)&&(mouse_y<210))
		{
			setcolor(1);
			rectangle(249,179,551,209);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>220)&&(mouse_y<250))
		{
			setcolor(1);
			rectangle(249,219,551,251);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>260)&&(mouse_y<290))
		{
			setcolor(1);
			rectangle(249,259,551,291);
		}
		else
		{
			setcolor(7);
			rectangle(39,443,131,466);
			rectangle(509,443,601,466);
			rectangle(249,99,551,131);
			rectangle(249,139,551,171);
			rectangle(249,179,551,209);
			rectangle(249,219,551,251);
			rectangle(249,259,551,291);
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			setcolor(1);
			if((mouse_x>40)&&(mouse_x<130)&&(mouse_y>444)&&(mouse_y<465))
				break;
			else
			if((mouse_x>510)&&(mouse_x<600)&&(mouse_y>444)&&(mouse_y<465))
			{
				int f=0;
				char *f1;
				BOOK_DB temp;
				ifstream check("BOOKS.DAT");
				while(check.read((char*)&temp,sizeof(temp)))
				{
					f1=strstr(bdb.bcode,temp.bcode);
					if(f1)
						f=1;
				}
				if(f==0)
				{
					ofstream of("BOOKS.DAT",ios::app);
					of.seekp(0,ios::end);
					status=0;
					strcpy(member,"     -");
					strcpy(duedate,"--/--/--");
					strcpy(mcode,"00000");
					of.write((char*)&bdb,sizeof(bdb));
					of.close();
					setcolor(7);
					outtextxy(250,300,"Book Number Already Exists!");
					setcolor(1);
					outtextxy(270,300,"Record Added!");
					sdb.addbook();
					delay(500);
					break;
				}
				else
				{
					setcolor(4);
					outtextxy(250,300,"Book Number Already Exists!");
					delay(500);
				}
			}
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>100)&&(mouse_y<130))
				inputstr(254,101,bcode);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>140)&&(mouse_y<170))
				inputstr(254,141,title);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>180)&&(mouse_y<210))
				inputstr(254,181,author);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>220)&&(mouse_y<250))
				inputstr(254,221,category);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>260)&&(mouse_y<290))
			{
				inputstr(254,261,tnoc);
				noc=strtonum(tnoc);
			}
			show_mouse();
		}
	}
}

void BOOK_DB::showtable(int mode)
{
	int rec=0,cho=101,page,spage1,spage2,spage3,spage4,sflag;
	page=spage1=spage2=spage3=spage4=0;
	sflag=0;
	char *p;
	char search[20];
	search[0]='\0';
	do
	{
		if(cho>50)
		{
			cleardevice();
			rec=0;
			setcolor(6);
			settextstyle(3,0,4);
			outtextxy(220,5,"Book Details");
			rectangle(3,3,636,476);
			rectangle(4,4,635,475);
			//Main View
			rectangle(20,100,620,430);
			setcolor(1);
			rectangle(20,50,620,70);
			setcolor(6);
			line(20,120,620,120);
			settextstyle(2,0,5);
			outtextxy(25,100,"Code       Title               Author       Category           Loaned To");
			line(65,100,65,430);
			line(215,100,215,430);
			line(365,100,365,430);
			line(515,100,515,430);
			//Top Buttons
			setcolor(1);
			settextstyle(3,0,1);
			outtextxy(50,74,"Search By :   Book No.     Title     Author     Loaned To ");
			rectangle(172,78,255,97);
			rectangle(280,78,353,97);
			rectangle(373,78,445,97);
			rectangle(473,78,575,97);
			setcolor(6);
			//Bottom Buttons
			outtextxy(50,440,"Back to Main Menu                            < Prev   Next >");
			rectangle(40,444,230,463);
			rectangle(455,444,533,463);
			rectangle(537,444,615,463);
			//Reading
			ifstream ifile("BOOKS.DAT");
			ifile.seekg((page*13*sizeof(bdb)),ios::beg);
			while(ifile.read((char*)&bdb,sizeof(bdb))&&rec<=304)
			{
				rec+=24;
				settextstyle(2,0,5);
				outtextxy(25,100+rec,bcode);
				outtextxy(70,100+rec,title);
				outtextxy(220,100+rec,author);
				outtextxy(370,100+rec,category);
				outtextxy(520,100+rec,member);
			}
			ifile.close();
		}
		delay(100);
		cho=tablebutton((sdb.total_books)-(page*13));
		setcolor(6);
		settextstyle(3,0,1);
		switch(cho)
		{
		case 1:
			setcolor(1);
			rectangle(19,49,621,71);
			inputstr(23,46,search);
			setcolor(7);
			rectangle(19,49,621,71);
			setcolor(6);
			break;
		case 11:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			outtextxy(25,100,"Code       Title               Author       Category           Loaned To");
			line(20,120,620,120);
			line(65,100,65,430);
			line(215,100,215,430);
			line(365,100,365,430);
			line(515,100,515,430);
			ifstream ifile("BOOKS.DAT");
			rec=0;
			while(ifile.read((char*)&bdb,sizeof(bdb))&&rec<=240)
			{
				p=strstr(bdb.bcode,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,bcode);
					outtextxy(70,100+rec,title);
					outtextxy(220,100+rec,author);
					outtextxy(370,100+rec,category);
					outtextxy(520,100+rec,member);
				}
			}
			ifile.close();
			sflag=1;
			break;
		 case 12:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			outtextxy(25,100,"Code       Title               Author       Category           Loaned To");
			line(20,120,620,120);
			line(65,100,65,430);
			line(215,100,215,430);
			line(365,100,365,430);
			line(515,100,515,430);
			ifstream ifile1("BOOKS.DAT");
			rec=0;
			while(ifile1.read((char*)&bdb,sizeof(bdb))&&rec<=240)
			{
				p=strstr(title,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,bcode);
					outtextxy(70,100+rec,title);
					outtextxy(220,100+rec,author);
					outtextxy(370,100+rec,category);
					outtextxy(520,100+rec,member);
				}
			}
			ifile1.close();
			sflag=2;
			break;
		case 13:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			outtextxy(25,100,"Code       Title               Author       Category           Loaned To");
			line(20,120,620,120);
			line(65,100,65,430);
			line(215,100,215,430);
			line(365,100,365,430);
			line(515,100,515,430);
			ifstream ifile2("BOOKS.DAT");
			rec=0;
			while(ifile2.read((char*)&bdb,sizeof(bdb))&&rec<=240)
			{
				p=strstr(author,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,bcode);
					outtextxy(70,100+rec,title);
					outtextxy(220,100+rec,author);
					outtextxy(370,100+rec,category);
					outtextxy(520,100+rec,member);
				}
			}
			ifile2.close();
			sflag=3;
			break;
		case 14:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			outtextxy(25,100,"Code       Title               Author       Category           Loaned To");
			line(20,120,620,120);
			line(65,100,65,430);
			line(215,100,215,430);
			line(365,100,365,430);
			line(515,100,515,430);
			ifstream ifile3("BOOKS.DAT");
			rec=0;
			while(ifile3.read((char*)&bdb,sizeof(bdb))&&rec<=240)
			{
				p=strstr(member,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,bcode);
					outtextxy(70,100+rec,title);
					outtextxy(220,100+rec,author);
					outtextxy(370,100+rec,category);
					outtextxy(520,100+rec,member);
				}
			}
			ifile3.close();
			sflag=4;
			break;
		case 51:
			if(sflag==0&&page!=0)
				page--;
			else
			if(sflag==1&&spage1!=0)
				spage1--;
			else
			if(sflag==2&&spage2!=0)
				spage2--;
			else
			if(sflag==3&&spage3!=0)
				spage3--;
			else
			if(sflag==4&&spage4!=0)
				spage4--;
			break;
		case 52:
			if(sdb.total_books>(page+1)*13)
			{
				if(sflag==0)
					page++;
				else
				if(sflag==1)
					spage1++;
				else
				if(sflag==2)
					spage2++;
				else
				if(sflag==3)
					spage3++;
				else
				if(sflag==4)
					spage4++;
			}
			break;
		case 101:
			ifstream ifile101("BOOKS.DAT");
			if(sflag==0)
				ifile101.seekg((page*13)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile101.seekg((spage1*13)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile101.seekg((spage2*13)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile101.seekg((spage3*13)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile101.seekg((spage4*13)*sizeof(bdb),ios::beg);
			ifile101.read((char*)&bdb,sizeof(bdb));
			ifile101.close();
			show(bcode,mode);
			break;
		case 102:
			ifstream ifile102("BOOKS.DAT");
			if(sflag==0)
				ifile102.seekg((page*13+1)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile102.seekg((spage1*13+1)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile102.seekg((spage2*13+1)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile102.seekg((spage3*13+1)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile102.seekg((spage4*13+1)*sizeof(bdb),ios::beg);
			ifile102.read((char*)&bdb,sizeof(bdb));
			ifile102.close();
			show(bcode,mode);
			break;
		case 103:
			ifstream ifile103("BOOKS.DAT");
			if(sflag==0)
				ifile103.seekg((page*13+2)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile103.seekg((spage1*13+2)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile103.seekg((spage2*13+2)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile103.seekg((spage3*13+2)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile103.seekg((spage4*13+2)*sizeof(bdb),ios::beg);
			ifile103.read((char*)&bdb,sizeof(bdb));
			ifile103.close();
			show(bcode,mode);
			break;
		case 104:
			ifstream ifile104("BOOKS.DAT");
			if(sflag==0)
				ifile104.seekg((page*13+3)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile104.seekg((spage1*13+3)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile104.seekg((spage2*13+3)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile104.seekg((spage3*13+3)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile104.seekg((spage4*13+3)*sizeof(bdb),ios::beg);
			ifile104.read((char*)&bdb,sizeof(bdb));
			ifile104.close();
			show(bcode,mode);
			break;
		case 105:
			ifstream ifile105("BOOKS.DAT");
			if(sflag==0)
				ifile105.seekg((page*13+4)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile105.seekg((spage1*13+4)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile105.seekg((spage2*13+4)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile105.seekg((spage3*13+4)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile105.seekg((spage4*13+4)*sizeof(bdb),ios::beg);
			ifile105.read((char*)&bdb,sizeof(bdb));
			ifile105.close();
			show(bcode,mode);
			break;
		case 106:
			ifstream ifile106("BOOKS.DAT");
			if(sflag==0)
				ifile106.seekg((page*13+5)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile106.seekg((spage1*13+5)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile106.seekg((spage2*13+5)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile106.seekg((spage3*13+5)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile106.seekg((spage4*13+5)*sizeof(bdb),ios::beg);
			ifile106.read((char*)&bdb,sizeof(bdb));
			ifile106.close();
			show(bcode,mode);
			break;
		case 107:
			ifstream ifile107("BOOKS.DAT");
			if(sflag==0)
				ifile107.seekg((page*13+6)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile107.seekg((spage1*13+6)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile107.seekg((spage2*13+6)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile107.seekg((spage3*13+6)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile107.seekg((spage4*13+6)*sizeof(bdb),ios::beg);
			ifile107.read((char*)&bdb,sizeof(bdb));
			ifile107.close();
			show(bcode,mode);
			break;
		case 108:
			ifstream ifile108("BOOKS.DAT");
			if(sflag==0)
				ifile108.seekg((page*13+7)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile108.seekg((spage1*13+7)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile108.seekg((spage2*13+7)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile108.seekg((spage3*13+7)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile108.seekg((spage4*13+7)*sizeof(bdb),ios::beg);
			ifile108.read((char*)&bdb,sizeof(bdb));
			ifile108.close();
			show(bcode,mode);
			break;
		case 109:
			ifstream ifile109("BOOKS.DAT");
			if(sflag==0)
				ifile109.seekg((page*13+8)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile109.seekg((spage1*13+8)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile109.seekg((spage2*13+8)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile109.seekg((spage3*13+8)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile109.seekg((spage4*13+8)*sizeof(bdb),ios::beg);
			ifile109.read((char*)&bdb,sizeof(bdb));
			ifile109.close();
			show(bcode,mode);
			break;
		case 110:
			ifstream ifile110("BOOKS.DAT");
			if(sflag==0)
				ifile110.seekg((page*13+9)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile110.seekg((spage1*13+9)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile110.seekg((spage2*13+9)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile110.seekg((spage3*13+9)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile110.seekg((spage4*13+9)*sizeof(bdb),ios::beg);
			ifile110.read((char*)&bdb,sizeof(bdb));
			ifile110.close();
			show(bcode,mode);
			break;
		case 111:
			ifstream ifile111("BOOKS.DAT");
			if(sflag==0)
				ifile111.seekg((page*13+10)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile111.seekg((spage1*13+10)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile111.seekg((spage2*13+10)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile111.seekg((spage3*13+10)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile111.seekg((spage4*13+10)*sizeof(bdb),ios::beg);
			ifile111.read((char*)&bdb,sizeof(bdb));
			ifile111.close();
			show(bcode,mode);
			break;
		case 112:
			ifstream ifile112("BOOKS.DAT");
			if(sflag==0)
				ifile112.seekg((page*13+11)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile112.seekg((spage1*13+11)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile112.seekg((spage2*13+11)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile112.seekg((spage3*13+11)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile112.seekg((spage4*13+11)*sizeof(bdb),ios::beg);
			ifile112.read((char*)&bdb,sizeof(bdb));
			ifile112.close();
			show(bcode,mode);
			break;
		case 113:
			ifstream ifile113("BOOKS.DAT");
			if(sflag==0)
				ifile113.seekg((page*13+12)*sizeof(bdb),ios::beg);
			else
			if(sflag==1)
				ifile113.seekg((spage1*13+12)*sizeof(bdb),ios::beg);
			else
			if(sflag==2)
				ifile113.seekg((spage2*13+12)*sizeof(bdb),ios::beg);
			else
			if(sflag==3)
				ifile113.seekg((spage3*13+12)*sizeof(bdb),ios::beg);
			else
			if(sflag==4)
				ifile113.seekg((spage4*13+12)*sizeof(bdb),ios::beg);
			ifile113.read((char*)&bdb,sizeof(bdb));
			ifile113.close();
			show(bcode,mode);
			break;
		case 0:
			break;
		}
		if(cho>100)
			readb=1;
		else
			readb=0;
	}
	while(cho!=0&&mode!=3);
}

void BOOK_DB::show(char code[],int mode)
{
if(mode!=3)
{
	cleardevice();
	setcolor(6);
	settextstyle(3,0,4);
	outtextxy(220,5,"Book Details");
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(3,0,2);
	outtextxy(50,100,"Book Code ");
	outtextxy(50,130,"Book Title ");
	outtextxy(50,160,"Author ");
	outtextxy(50,190,"Category ");
	outtextxy(50,220,"Status ");
	outtextxy(50,250,"Loaned To ");
	outtextxy(50,280,"Due Date ");
	outtextxy(50,310,"Count ");
	outtextxy(40,350,"< Back");
	rectangle(38,350,130,380);
	if(mode==1)
	{
		outtextxy(200,350," Delete");
		rectangle(198,350,290,380);
	}
	else
	if(mode==2)
	{
		outtextxy(200,350," Update");
		rectangle(198,350,290,380);
		setcolor(1);
		rectangle(215,104,415,130);
		rectangle(215,134,415,160);
		rectangle(215,164,415,190);
		rectangle(215,194,415,220);
		rectangle(215,314,415,340);
		setcolor(6);
	}
	fstream file("BOOKS.DAT",ios::in|ios::out|ios::binary);
	int flag=0,count=0;
	char t[10];
	strcpy(t,code);
	while(file.read((char*)&bdb,sizeof(bdb))&&flag==0)
	{
		if(strcmpi(t,bcode)==0)
		{
			if(mode==2)
				setcolor(1);
			outtextxy(220,100,bcode);
			outtextxy(220,130,title);
			outtextxy(220,160,author);
			outtextxy(220,190,category);
			if(status==0)
				outtextxy(220,220,"Available");
			else
				outtextxy(220,220,"Loaned");
			outtextxy(220,250,member);
			outtextxy(220,280,duedate);
			char tnoc[6];
			numtostr(noc,tnoc,4);
			outtextxy(220,310,tnoc);
			setcolor(6);
			strcpy(t,bcode);
			delay(150);
			if(detect_mouse)
				show_mouse();
			while(flag==0)
			{
				test_mouse();
				if((mouse_x>38)&&(mouse_x<130)&&(mouse_y>350)&&(mouse_y<380))
				{
					setcolor(6);
					rectangle(37,349,131,381);
				}
				else
				if((mouse_x>198)&&(mouse_x<290)&&(mouse_y>350)&&(mouse_y<380)&&(mode==1||mode==2))
				{
					setcolor(6);
					rectangle(197,349,291,381);
				}
				else
				if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>104)&&(mouse_y<130)&&mode==2)
				{
					setcolor(1);
					rectangle(214,103,416,131);
				}
				else
				if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>134)&&(mouse_y<160)&&mode==2)
				{
					setcolor(1);
					rectangle(214,133,416,161);
				}
				else
				if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>164)&&(mouse_y<190)&&mode==2)
				{
					setcolor(1);
					rectangle(214,163,416,191);
				}
				else
				if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>194)&&(mouse_y<220)&&mode==2)
				{
					setcolor(1);
					rectangle(214,193,416,221);
				}
				else
				if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>314)&&(mouse_y<340)&&mode==2)
				{
					setcolor(1);
					rectangle(214,313,416,341);
				}
				else
				{
					setcolor(7);
					rectangle(37,349,131,381);
					rectangle(197,349,291,381);
					rectangle(214,103,416,131);
					rectangle(214,133,416,161);
					rectangle(214,163,416,191);
					rectangle(214,193,416,221);
					rectangle(214,313,416,341);
				}
				if (mouse_b & 1)
				{
					hide_mouse();
					if((mouse_x>38)&&(mouse_x<130)&&(mouse_y>350)&&(mouse_y<380))
					{
						flag=1;
						break;
					}
					else
					if((mouse_x>198)&&(mouse_x<290)&&(mouse_y>350)&&(mouse_y<380)&&mode==2)
					{
						file.seekp((count)*(sizeof(bdb)),ios::beg);
						file.write((char*)&bdb,sizeof(bdb));
						flag=1;
						break;
					}
					if((mouse_x>198)&&(mouse_x<290)&&(mouse_y>350)&&(mouse_y<380)&&mode==1)
					{
						file.seekg(0,ios::beg);
						ofstream ofile("TEMP.DAT");
						while(file.read((char*)&bdb,sizeof(bdb)))
						{
							if(strcmpi(t,bcode)!=0)
								ofile.write((char*)&bdb,sizeof(bdb));
						}
						ofile.close();
						remove("BOOKS.DAT");
						rename("TEMP.DAT","BOOKS.DAT");
						sdb.delbook();
						break;
					}
					else
					if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>104)&&(mouse_y<130)&&mode==2)
						inputstr(220,100,bcode);
					else
					if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>134)&&(mouse_y<160)&&mode==2)
						inputstr(220,130,title);
					else
					if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>164)&&(mouse_y<190)&&mode==2)
						inputstr(220,160,author);
					else
					if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>194)&&(mouse_y<220)&&mode==2)
						inputstr(220,190,category);
					else
					if((mouse_x>215)&&(mouse_x<415)&&(mouse_y>314)&&(mouse_y<340)&&mode==2)
					{
						inputstr(220,310,tnoc);
						noc=strtonum(tnoc);
					}
					show_mouse();
				}
			}
		}
		count++;
	}
	file.close();
}
}

void BOOK_DB::rtitle(char str[],int &stat)
{
	strcpy(str,title);
	stat=status;
}

int BOOK_DB::loan(char due[])
{
	if(noc<sdb.min_copies)
	{
		outtextxy(10,350,"This Book cannot be loaned due to insufficient Copies");
		return 0;
	}
	else
	{
		char tcode[10];
		int fl=0,c=0;
		strcpy(tcode,bcode);
		fstream l("BOOKS.DAT",ios::in|ios::out|ios::binary);
		l.seekg(0,ios::beg);
		while(l.read((char*)&bdb,sizeof(bdb))&&fl==0)
		{
			if(strcmpi(tcode,bcode)==0)
			{
				fl=1;
				l.seekp((c)*sizeof(bdb),ios::beg);
				strcpy(duedate,due);
				status=1;
				mdb.loan(member,mcode,title,bcode);
				noc--;
				l.write((char*)&bdb,sizeof(bdb));
				sdb.addloan();
			}
			c++;
		}
		l.close();
		delay(150);
		return 1;
	}
}

float BOOK_DB::ret()
{
	char tcode[10];
	float j;
	strcpy(tcode,bcode);
	int fl=0,c=0;
	fstream l("BOOKS.DAT",ios::in|ios::out|ios::binary);
	l.seekg(0,ios::beg);
	while(l.read((char*)&bdb,sizeof(bdb))&&fl==0)
	{
		if(strcmpi(tcode,bcode)==0)
		{
			fl=1;
			l.seekp((c)*sizeof(bdb),ios::beg);
			j=gdtojd(duedate);
			status=0;
			strcpy(member,"     -");
			strcpy(duedate,"--/--/--");
			mdb.ret(tcode);
			bdb.noc++;
			l.write((char*)&bdb,sizeof(bdb));
			sdb.delloan();
		}
		c++;
	}
	return j;
}

//MEMBER_DB
void MEMBER_DB::add()
{
	cleardevice();
	setcolor(6);
	settextstyle(3,0,4);
	outtextxy(220,5,"Add Member");
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(3,0,2);
	outtextxy(50,440,"< Back");
	rectangle(40,444,130,465);
	outtextxy(520,440,"Add >");
	rectangle(510,444,600,465);
	setcolor(1);
	outtextxy(50,100,"Member Code  ");
	outtextxy(50,140,"Member Name  ");
	outtextxy(50,180,"Phone        ");
	outtextxy(50,220,"E-Mail      ");
	outtextxy(50,260,"Address     ");
	rectangle(250,100,550,130);
	rectangle(250,140,400,170);
	rectangle(403,140,550,170);
	rectangle(250,180,550,210);
	rectangle(250,220,550,250);
	rectangle(250,260,550,290);
	rectangle(250,300,550,330);
	rectangle(250,340,550,370);
	//Initialising Variables to Null
	char tnoc[6];
	tnoc[0]='\0';
	mcode[0]='\0';
	first[0]='\0';
	last[0]='\0';
	phone[0]='\0';
	email[0]='\0';
	address1[0]='\0';
	address2[0]='\0';
	address3[0]='\0';
	delay(150);
	if (detect_mouse())
		show_mouse();
	while (::quit==1)
	{
		test_mouse();
		if((mouse_x>40)&&(mouse_x<130)&&(mouse_y>444)&&(mouse_y<465))
		{
			setcolor(6);
			rectangle(39,443,131,466);
		}
		else
		if((mouse_x>510)&&(mouse_x<600)&&(mouse_y>444)&&(mouse_y<465))
		{
			setcolor(6);
			rectangle(509,443,601,466);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>100)&&(mouse_y<130))
		{
			setcolor(1);
			rectangle(249,99,551,131);
		}
		else
		if((mouse_x>250)&&(mouse_x<400)&&(mouse_y>140)&&(mouse_y<170))
		{
			setcolor(1);
			rectangle(249,139,401,171);
		}
		else
		if((mouse_x>403)&&(mouse_x<550)&&(mouse_y>140)&&(mouse_y<170))
		{
			setcolor(1);
			rectangle(402,139,551,171);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>180)&&(mouse_y<210))
		{
			setcolor(1);
			rectangle(249,179,551,209);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>220)&&(mouse_y<250))
		{
			setcolor(1);
			rectangle(249,219,551,251);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>260)&&(mouse_y<290))
		{
			setcolor(1);
			rectangle(249,259,551,291);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>300)&&(mouse_y<330))
		{
			setcolor(1);
			rectangle(249,299,551,331);
		}
		else
		if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>340)&&(mouse_y<370))
		{
			setcolor(1);
			rectangle(249,339,551,371);
		}
		else
		{
			setcolor(7);
			rectangle(39,443,131,466);
			rectangle(509,443,601,466);
			rectangle(249,99,551,131);
			rectangle(249,139,401,171);
			rectangle(402,139,551,171);
			rectangle(249,179,551,209);
			rectangle(249,219,551,251);
			rectangle(249,259,551,291);
			rectangle(249,299,551,331);
			rectangle(249,339,551,371);
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			setcolor(1);
			if((mouse_x>40)&&(mouse_x<130)&&(mouse_y>444)&&(mouse_y<465))
				break;
			else
			if((mouse_x>510)&&(mouse_x<600)&&(mouse_y>444)&&(mouse_y<465))
			{
				int f=0;
				char *f1;
				MEMBER_DB temp;
				ifstream check("MEMBER.DAT");
				while(check.read((char*)&temp,sizeof(temp)))
				{
					f1=strstr(mdb.mcode,temp.mcode);
					if(f1)
						f=1;
				}
				if(f==0)
				{
					ofstream of("MEMBER.DAT",ios::app);
					of.seekp(0,ios::end);
					status=0;
					nlog=1;
					strcpy(book,"     -");
					strcpy(bcode,"00000");
					of.write((char*)&mdb,sizeof(mdb));
					of.close();
					setcolor(7);
					outtextxy(250,400,"Member Number Already Exists!");
					setcolor(1);
					outtextxy(270,400,"Record Added!");
					sdb.addmember();
					struct dosdate_t d;
					_dos_getdate(&d);
					char D[3],M[3],Y[3],filename[15];
					numtostr(d.day,D,1);
					numtostr(d.month,M,1);
					numtostr(d.year,Y,1);
					log.date[0]='\0';
					strcat(log.date,D);
					strcat(log.date,"\\");
					strcat(log.date,M);
					strcat(log.date,"\\");
					strcat(log.date,Y);
					strcpy(log.action,"User Joined the Library");
					strcat(log.action,"                                                              ");
					strcpy(filename,mcode);
					strcat(filename,".txt");
					ofstream LOG(filename);
					LOG.write((char*)&log,sizeof(log));
					LOG.close();
					outtextxy(270,420,filename);
					outtextxy(370,420,"Created!");
					delay(500);
					break;
				}
				else
				{
					setcolor(4);
					outtextxy(250,400,"Member Number Already Exists!");
					delay(500);
				}
			}
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>100)&&(mouse_y<130))
				inputstr(254,101,mcode);
			else
			if((mouse_x>250)&&(mouse_x<400)&&(mouse_y>140)&&(mouse_y<170))
				inputstr(254,141,first);
			else
			if((mouse_x>403)&&(mouse_x<550)&&(mouse_y>140)&&(mouse_y<170))
				inputstr(407,141,last);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>180)&&(mouse_y<210))
				inputstr(254,181,phone);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>220)&&(mouse_y<250))
				inputstr(254,221,email);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>260)&&(mouse_y<290))
				inputstr(254,261,address1);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>300)&&(mouse_y<330))
				inputstr(254,301,address2);
			else
			if((mouse_x>250)&&(mouse_x<550)&&(mouse_y>340)&&(mouse_y<370))
				inputstr(254,341,address3);
			show_mouse();
		}
	}
}

void MEMBER_DB::showtable(int mode)
{
	int rec=0,cho=101,page,spage1,spage2,spage3,spage4,sflag;
	page=spage1=spage2=spage3=spage4=0;
	sflag=0;
	char *p;
	char search[20],name[25];
	search[0]='\0';
	do
	{
		if(cho>50)
		{
			cleardevice();
			rec=0;
			setcolor(6);
			settextstyle(3,0,4);
			outtextxy(220,5,"Member Details");
			rectangle(3,3,636,476);
			rectangle(4,4,635,475);
			//Main View
			rectangle(20,100,620,430);
			setcolor(1);
			rectangle(20,50,620,70);
			setcolor(6);
			line(20,120,620,120);
			settextstyle(2,0,5);
			outtextxy(25,100,"Code         Name                   Phone               Book Loaned");
			line(65,100,65,430);
			line(255,100,255,430);
			line(405,100,405,430);
			//Top Buttons
			setcolor(1);
			settextstyle(3,0,1);
			outtextxy(50,74,"Search By :     Code      Name     Phone     Book Loan. ");
			rectangle(172,78,255,97);
			rectangle(280,78,353,97);
			rectangle(373,78,445,97);
			rectangle(473,78,575,97);
			setcolor(6);
			//Bottom Buttons
			outtextxy(50,440,"Back to Main Menu                            < Prev   Next >");
			rectangle(40,444,230,463);
			rectangle(455,444,533,463);
			rectangle(537,444,615,463);
			//Reading
			ifstream ifile("MEMBER.DAT");
			ifile.seekg((page*13*sizeof(mdb)),ios::beg);
			while(ifile.read((char*)&mdb,sizeof(mdb))&&rec<=304)
			{
				rec+=24;
				settextstyle(2,0,5);
				outtextxy(25,100+rec,mcode);
				name[0]='\0';
				strcat(name,first);
				strcat(name," ");
				strcat(name,last);
				outtextxy(70,100+rec,name);
				outtextxy(260,100+rec,phone);
				outtextxy(410,100+rec,book);
			}
			ifile.close();
		}
		delay(100);
		cho=tablebutton((sdb.total_members)-(page*13));
		setcolor(6);
		settextstyle(3,0,1);
		switch(cho)
		{
		case 1:
			setcolor(1);
			rectangle(19,49,621,71);
			inputstr(23,46,search);
			setcolor(7);
			rectangle(19,49,621,71);
			setcolor(6);
			break;
		case 11:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			line(20,120,620,120);
			outtextxy(25,100,"Code         Name                   Phone               Book Loaned");
			line(65,100,65,430);
			line(255,100,255,430);
			line(405,100,405,430);
			ifstream ifile("MEMBER.DAT");
			rec=0;
			while(ifile.read((char*)&mdb,sizeof(mdb))&&rec<=240)
			{
				p=strstr(mdb.mcode,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,mcode);
					name[0]='\0';
					strcat(name,first);
					strcat(name," " );
					strcat(name,last);
					outtextxy(70,100+rec,name);
					outtextxy(260,100+rec,phone);
					outtextxy(410,100+rec,book);
				}
			}
			ifile.close();
			sflag=1;
			break;
		 case 12:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			line(20,120,620,120);
			outtextxy(25,100,"Code         Name                   Phone               Book Loaned");
			line(65,100,65,430);
			line(255,100,255,430);
			line(405,100,405,430);
			ifstream ifile1("MEMBER.DAT");
			rec=0;
			while(ifile1.read((char*)&mdb,sizeof(mdb))&&rec<=240)
			{
				name[0]='\0';
				strcat(name,first);
				strcat(name," " );
				strcat(name,last);
				p=strstr(name,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,mcode);
					name[0]='\0';
					strcat(name,first);
					strcat(name," " );
					strcat(name,last);
					outtextxy(70,100+rec,name);
					outtextxy(260,100+rec,phone);
					outtextxy(410,100+rec,book);
				}
			}
			ifile1.close();
			sflag=2;
			break;
		case 13:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			line(20,120,620,120);
			outtextxy(25,100,"Code         Name                   Phone               Book Loaned");
			line(65,100,65,430);
			line(255,100,255,430);
			line(405,100,405,430);
			ifstream ifile3("MEMBER.DAT");
			rec=0;
			while(ifile3.read((char*)&mdb,sizeof(mdb))&&rec<=240)
			{
				p=strstr(mdb.phone,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,mcode);
					name[0]='\0';
					strcat(name,first);
					strcat(name," " );
					strcat(name,last);
					outtextxy(70,100+rec,name);
					outtextxy(260,100+rec,phone);
					outtextxy(410,100+rec,book);
				}
			}
			ifile3.close();
			sflag=3;
			break;
		case 14:
			setfillstyle(SOLID_FILL,7);
			bar(21,101,619,429);
			settextstyle(2,0,5);
			line(20,120,620,120);
			outtextxy(25,100,"Code         Name                   Phone               Book Loaned");
			line(65,100,65,430);
			line(255,100,255,430);
			line(405,100,405,430);
			ifstream ifile4("MEMBER.DAT");
			rec=0;
			while(ifile4.read((char*)&mdb,sizeof(mdb))&&rec<=240)
			{
				p=strstr(mdb.book,search);
				if(p)
				{
					rec+=24;
					settextstyle(2,0,5);
					outtextxy(25,100+rec,mcode);
					name[0]='\0';
					strcat(name,first);
					strcat(name," ");
					strcat(name,last);
					outtextxy(70,100+rec,name);
					outtextxy(260,100+rec,phone);
					outtextxy(410,100+rec,book);
				}
			}
			ifile4.close();
			sflag=4;
			break;
		case 51:
			if(sflag==0&&page!=0)
				page--;
			else
			if(sflag==1&&spage1!=0)
				spage1--;
			else
			if(sflag==2&&spage2!=0)
				spage2--;
			else
			if(sflag==3&&spage3!=0)
				spage3--;
			else
			if(sflag==4&&spage4!=0)
				spage4--;
			break;
		case 52:
			if(sdb.total_members>(page+1)*13)
			{
				if(sflag==0)
					page++;
				else
				if(sflag==1)
					spage1++;
				else
				if(sflag==2)
					spage2++;
				else
				if(sflag==3)
					spage3++;
				else
				if(sflag==4)
					spage4++;
			}
			break;
		case 101:
			ifstream ifile101("MEMBER.DAT");
			if(sflag==0)
				ifile101.seekg((page*13)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile101.seekg((spage1*13)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile101.seekg((spage2*13)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile101.seekg((spage3*13)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile101.seekg((spage4*13)*sizeof(mdb),ios::beg);
			ifile101.read((char*)&mdb,sizeof(mdb));
			ifile101.close();
			show(mcode,mode);
			break;
		case 102:
			ifstream ifile102("MEMBER.DAT");
			if(sflag==0)
				ifile102.seekg((page*13+1)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile102.seekg((spage1*13+1)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile102.seekg((spage2*13+1)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile102.seekg((spage3*13+1)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile102.seekg((spage4*13+1)*sizeof(mdb),ios::beg);
			ifile102.read((char*)&mdb,sizeof(mdb));
			ifile102.close();
			show(mcode,mode);
			break;
		case 103:
			ifstream ifile103("MEMBER.DAT");
			if(sflag==0)
				ifile103.seekg((page*13+2)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile103.seekg((spage1*13+2)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile103.seekg((spage2*13+2)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile103.seekg((spage3*13+2)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile103.seekg((spage4*13+2)*sizeof(mdb),ios::beg);
			ifile103.read((char*)&mdb,sizeof(mdb));
			ifile103.close();
			show(mcode,mode);
			break;
		case 104:
			ifstream ifile104("MEMBER.DAT");
			if(sflag==0)
				ifile104.seekg((page*13+3)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile104.seekg((spage1*13+3)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile104.seekg((spage2*13+3)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile104.seekg((spage3*13+3)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile104.seekg((spage4*13+3)*sizeof(mdb),ios::beg);
			ifile104.read((char*)&mdb,sizeof(mdb));
			ifile104.close();
			show(mcode,mode);
			break;
		case 105:
			ifstream ifile105("MEMBER.DAT");
			if(sflag==0)
				ifile105.seekg((page*13+4)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile105.seekg((spage1*13+4)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile105.seekg((spage2*13+4)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile105.seekg((spage3*13+4)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile105.seekg((spage4*13+4)*sizeof(mdb),ios::beg);
			ifile105.read((char*)&mdb,sizeof(mdb));
			ifile105.close();
			show(mcode,mode);
			break;
		case 106:
			ifstream ifile106("MEMBER.DAT");
			if(sflag==0)
				ifile106.seekg((page*13+5)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile106.seekg((spage1*13+5)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile106.seekg((spage2*13+5)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile106.seekg((spage3*13+5)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile106.seekg((spage4*13+5)*sizeof(mdb),ios::beg);
			ifile106.read((char*)&mdb,sizeof(mdb));
			ifile106.close();
			show(mcode,mode);
			break;
		case 107:
			ifstream ifile107("MEMBER.DAT");
			if(sflag==0)
				ifile107.seekg((page*13+6)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile107.seekg((spage1*13+6)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile107.seekg((spage2*13+6)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile107.seekg((spage3*13+6)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile107.seekg((spage4*13+6)*sizeof(mdb),ios::beg);
			ifile107.read((char*)&mdb,sizeof(mdb));
			ifile107.close();
			show(mcode,mode);
			break;
		case 108:
			ifstream ifile108("MEMBER.DAT");
			if(sflag==0)
				ifile108.seekg((page*13+7)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile108.seekg((spage1*13+7)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile108.seekg((spage2*13+7)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile108.seekg((spage3*13+7)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile108.seekg((spage4*13+7)*sizeof(mdb),ios::beg);
			ifile108.read((char*)&mdb,sizeof(mdb));
			ifile108.close();
			show(mcode,mode);
			break;
		case 109:
			ifstream ifile109("MEMBER.DAT");
			if(sflag==0)
				ifile109.seekg((page*13+8)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile109.seekg((spage1*13+8)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile109.seekg((spage2*13+8)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile109.seekg((spage3*13+8)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile109.seekg((spage4*13+8)*sizeof(mdb),ios::beg);
			ifile109.read((char*)&mdb,sizeof(mdb));
			ifile109.close();
			show(mcode,mode);
			break;
		case 110:
			ifstream ifile110("MEMBER.DAT");
			if(sflag==0)
				ifile110.seekg((page*13+9)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile110.seekg((spage1*13+9)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile110.seekg((spage2*13+9)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile110.seekg((spage3*13+9)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile110.seekg((spage4*13+9)*sizeof(mdb),ios::beg);
			ifile110.read((char*)&mdb,sizeof(mdb));
			ifile110.close();
			show(mcode,mode);
			break;
		case 111:
			ifstream ifile111("MEMBER.DAT");
			if(sflag==0)
				ifile111.seekg((page*13+10)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile111.seekg((spage1*13+10)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile111.seekg((spage2*13+10)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile111.seekg((spage3*13+10)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile111.seekg((spage4*13+10)*sizeof(mdb),ios::beg);
			ifile111.read((char*)&mdb,sizeof(mdb));
			ifile111.close();
			show(mcode,mode);
			break;
		case 112:
			ifstream ifile112("MEMBER.DAT");
			if(sflag==0)
				ifile112.seekg((page*13+11)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile112.seekg((spage1*13+11)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile112.seekg((spage2*13+11)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile112.seekg((spage3*13+11)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile112.seekg((spage4*13+11)*sizeof(mdb),ios::beg);
			ifile112.read((char*)&mdb,sizeof(mdb));
			ifile112.close();
			show(mcode,mode);
			break;
		case 113:
			ifstream ifile113("MEMBER.DAT");
			if(sflag==0)
				ifile113.seekg((page*13+12)*sizeof(mdb),ios::beg);
			else
			if(sflag==1)
				ifile113.seekg((spage1*13+12)*sizeof(mdb),ios::beg);
			else
			if(sflag==2)
				ifile113.seekg((spage2*13+12)*sizeof(mdb),ios::beg);
			else
			if(sflag==3)
				ifile113.seekg((spage3*13+12)*sizeof(mdb),ios::beg);
			else
			if(sflag==4)
				ifile113.seekg((spage4*13+12)*sizeof(mdb),ios::beg);
			ifile113.read((char*)&mdb,sizeof(mdb));
			ifile113.close();
			show(mcode,mode);
			break;
		case 0:
			break;
		}
		if(cho>100)
			readm=1;
		else
			readm=0;
	}
	while(cho!=0&&mode!=3);
}

void MEMBER_DB::show(char code[],int mode)
{
if(mode!=3)
{
	cleardevice();
	setcolor(6);
	settextstyle(3,0,4);
	outtextxy(220,5,"Book Details");
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(3,0,2);
	outtextxy(50,100,"Member Code ");
	outtextxy(50,130,"Member Name ");
	outtextxy(50,160,"Phone ");
	outtextxy(50,190,"Email ");
	outtextxy(50,220,"Address  ");
	outtextxy(50,310,"Loaned Book ");
	outtextxy(40,350,"< Back");
	rectangle(38,350,130,380);
	if(mode==1)
	{
		outtextxy(200,350," Delete");
		rectangle(198,350,290,380);
	}
	else
	if(mode==2)
	{
		outtextxy(200,350," Update");
		rectangle(198,350,290,380);
		setcolor(1);
		rectangle(215,104,515,130);
		rectangle(215,134,365,160);
		rectangle(368,134,515,160);
		rectangle(215,164,515,190);
		rectangle(215,194,515,220);
		rectangle(215,224,515,250);
		rectangle(215,254,515,280);
		rectangle(215,284,515,310);
		setcolor(6);
	}
	fstream file("MEMBER.DAT",ios::in|ios::out|ios::binary);
	int flag=0,count=0;
	char temp[10];
	strcpy(temp,code);
	while(file.read((char*)&mdb,sizeof(mdb))&&flag==0)
	{
		if(strcmpi(temp,mcode)==0)
		{
			if(mode==2)
				setcolor(1);
			outtextxy(220,100,mcode);
			char name[25];
			name[0]='\0';
			strcat(name,first);
			strcat(name," " );
			strcat(name,last);
			if(mode==2)
			{
				outtextxy(220,130,first);
				outtextxy(373,130,last);
			}
			else
				outtextxy(220,130,name);
			outtextxy(220,160,phone);
			outtextxy(220,190,email);
			outtextxy(220,220,address1);
			outtextxy(220,250,address2);
			outtextxy(220,280,address3);
			outtextxy(220,310,book);
			setcolor(6);
			delay(150);
			if(detect_mouse)
				show_mouse();
			while(flag==0)
			{
				test_mouse();
				if((mouse_x>38)&&(mouse_x<130)&&(mouse_y>350)&&(mouse_y<380))
				{
					setcolor(6);
					rectangle(37,349,131,381);
				}
				else
				if((mouse_x>198)&&(mouse_x<290)&&(mouse_y>350)&&(mouse_y<380)&&(mode==1||mode==2))
				{
					setcolor(6);
					rectangle(197,349,291,381);
				}
				else
				if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>104)&&(mouse_y<130)&&mode==2)
				{
					setcolor(1);
					rectangle(214,103,516,131);
				}
				else
				if((mouse_x>215)&&(mouse_x<365)&&(mouse_y>134)&&(mouse_y<160)&&mode==2)
				{
					setcolor(1);
					rectangle(214,133,366,161);
				}
				else
				if((mouse_x>368)&&(mouse_x<515)&&(mouse_y>134)&&(mouse_y<160)&&mode==2)
				{
					setcolor(1);
					rectangle(367,133,516,161);
				}
				else
				if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>164)&&(mouse_y<190)&&mode==2)
				{
					setcolor(1);
					rectangle(214,163,516,191);
				}
				else
				if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>194)&&(mouse_y<220)&&mode==2)
				{
					setcolor(1);
					rectangle(214,193,516,221);
				}
				else
				if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>224)&&(mouse_y<250)&&mode==2)
				{
					setcolor(1);
					rectangle(214,223,516,251);
				}
				else
				if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>254)&&(mouse_y<280)&&mode==2)
				{
					setcolor(1);
					rectangle(214,253,516,281);
				}
				else
				if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>284)&&(mouse_y<310)&&mode==2)
				{
					setcolor(1);
					rectangle(214,283,516,311);
				}
				else
				{
					setcolor(7);
					rectangle(37,349,131,381);
					rectangle(197,349,291,381);
					rectangle(214,103,516,131);
					rectangle(214,133,366,161);
					rectangle(367,133,516,161);
					rectangle(214,163,516,191);
					rectangle(214,193,516,221);
					rectangle(214,193,516,221);
					rectangle(214,223,516,251);
					rectangle(214,253,516,281);
					rectangle(214,283,516,311);
				}
				if (mouse_b & 1)
				{
					hide_mouse();
					if((mouse_x>38)&&(mouse_x<130)&&(mouse_y>350)&&(mouse_y<380))
					{
						flag=1;
						break;
					}
					else
					if((mouse_x>198)&&(mouse_x<290)&&(mouse_y>350)&&(mouse_y<380)&&mode==2)
					{
						file.seekp((count)*(sizeof(mdb)),ios::beg);
						file.write((char*)&mdb,sizeof(mdb));
						flag=1;
						break;
					}
					if((mouse_x>198)&&(mouse_x<290)&&(mouse_y>350)&&(mouse_y<380)&&mode==1)
					{
						file.seekg(0,ios::beg);
						ofstream ofile("TEMP.DAT");
						while(file.read((char*)&mdb,sizeof(mdb)))
						{
							if(strcmpi(temp,mcode)!=0)
								ofile.write((char*)&mdb,sizeof(mdb));
						}
						ofile.close();
						remove("MEMBER.DAT");
						rename("TEMP.DAT","MEMBER.DAT");
						sdb.delmember();
						break;
					}
					else
					if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>104)&&(mouse_y<130)&&mode==2)
						inputstr(220,100,mcode);
					else
					if((mouse_x>215)&&(mouse_x<365)&&(mouse_y>134)&&(mouse_y<160)&&mode==2)
						inputstr(220,130,first);
					else
					if((mouse_x>368)&&(mouse_x<515)&&(mouse_y>134)&&(mouse_y<160)&&mode==2)
						inputstr(373,130,last);
					else
					if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>164)&&(mouse_y<190)&&mode==2)
						inputstr(220,160,phone);
					else
					if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>194)&&(mouse_y<220)&&mode==2)
						inputstr(220,190,email);
					else
					if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>224)&&(mouse_y<250)&&mode==2)
						inputstr(220,220,address1);
					else
					if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>254)&&(mouse_y<280)&&mode==2)
						inputstr(220,250,address2);
					else
					if((mouse_x>215)&&(mouse_x<515)&&(mouse_y>284)&&(mouse_y<310)&&mode==2)
						inputstr(220,280,address3);
					show_mouse();
				}
			}
		}
		count++;
	}
	file.close();
}
}

void MEMBER_DB::rname(char str[],int &stat)
{
	char name[25];
	name[0]='\0';
	strcat(name,first);
	strcat(name," " );
	strcat(name,last);
	strcpy(str,name);
	stat=status;
}

void MEMBER_DB::loan(char name[],char code[],char Bname[],char Bcode[])
{
	strcpy(name,first);
	strcpy(code,mcode);
	char tempc[5];
	int fg=0,co=0;
	strcpy(tempc,mcode);
	fstream mloan("MEMBER.DAT",ios::in|ios::out|ios::binary);
	while(mloan.read((char*)&mdb,sizeof(mdb))&&fg==0)
	{
		if(strcmpi(tempc,mcode)==0)
		{
			fg=1;
			mloan.seekp((co)*sizeof(mdb),ios::beg);
			strcpy(book,Bname);
			strcpy(bcode,Bcode);
			status=1;
			mloan.write((char*)&mdb,sizeof(mdb));
		}
		co++;
	}
}

void MEMBER_DB::ret(char Bcode[])
{
	char tempc[5];
	int fg=0,co=0;
	strcpy(tempc,Bcode);
	fstream mloan("MEMBER.DAT",ios::in|ios::out|ios::binary);
	while(mloan.read((char*)&mdb,sizeof(mdb))&&fg==0)
	{
		if(strcmpi(tempc,bcode)==0)
		{
			fg=1;
			mloan.seekp((co)*sizeof(mdb),ios::beg);
			strcpy(book,"     -");
			strcpy(bcode,"     -");
			status=0;
			mloan.write((char*)&mdb,sizeof(mdb));
		}
		co++;
	}
}

void MEMBER_DB::rcode(char str[])
{
	strcpy(str,mcode);
}

/* Function Definitions *\
\* ==================== */
int detect_mouse()
{
      int nButtons,mouseFound;
      asm{
	     mov ax,0
	     int 0x33
	     mov mouseFound,ax
	     mov nButtons,bx
      }

      if(mouseFound)
		return nButtons;
      else
		return 0;
}

void show_mouse()
{
      asm{
	     mov ax,0x0001
	     int 0x33
      }
}

void hide_mouse()
{
      asm{
	      mov ax,0x002
	      int 0x33
      }
}

void test_mouse()
{
       asm{
	       mov ax,0x0003
	       int 0x33
	       mov mouse_b,bx
	       mov mouse_x,cx
	       mov mouse_y,dx
       }
}

void set_mouse_speed(int sx,int sy)
{
      asm{
	      mov ax,0x000f
	      mov cx,sx
	      mov dx,sy
	      int 0x33
      }
}

void move_mouse(int x,int y)
{
      asm{
	      mov ax,4
	      mov cx,x
	      mov dx,y
	      int 0x33
      }
      mouse_x=x;
      mouse_y=y;
}

void loading()
{
	int i,temp;
	setcolor(6);
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(10,0,5);
	outtextxy(120,100,"Library");
	outtextxy(190,150,"Management");
	outtextxy(250,210,"System!");
	setcolor(1);
	settextstyle(2,0,5);
	outtextxy(250,425,"Loading, Please Wait......");
	rectangle(10,445,629,465);
	setfillstyle(SOLID_FILL,1);
	temp=0;
	for(i=12;i<627;i++)
	{
		bar(12,447,i,463);
		setcolor(2);
		settextstyle(2,0,4);
		if(i<100)
			outtextxy(270,450,"Loading Modules...");
		else
		if(i<200)
		{
			if(temp==0)
			{
				setcolor(0);
				outtextxy(270,450,"Loading Modules...");
				setcolor(2);
				temp=1;
			}
			outtextxy(270,450,"Checking Database...");
		}
		else
		if(i<400)
		{
			if(temp==1)
			{
				setcolor(0);
				outtextxy(270,450,"Checking Database...");
				setcolor(2);
				temp=2;
			}
			outtextxy(270,450,"Initialising Mouse...");
		}
		else
		if(i<627)
			outtextxy(270,450,"Starting Program...");
		delay(10);
	}
}

void login()
{
	ifstream config("CONFIG.DAT"); 	//Opening Settings File.
	config.read((char*)&sdb,sizeof(sdb));
	char username[20],password[20];
	config.close();
	//Initialising Strings to NULL
	username[0]='\0';
	password[0]='\0';
	logindraw(username,password);
	if (detect_mouse())
		show_mouse();
	while (::quit==1)
	{
		test_mouse();
		if((mouse_x>180)&&(mouse_x<225)&&(mouse_y>252)&&(mouse_y<272))
		{
			setcolor(6);
			rectangle(179,251,226,273);
		}
		else
		if((mouse_x>425)&&(mouse_x<480)&&(mouse_y>252)&&(mouse_y<272))
		{
			setcolor(6);
			rectangle(424,251,481,273);
		}
		else
		if((mouse_x>300)&&(mouse_x<460)&&(mouse_y>190)&&(mouse_y<208))
		{
			setcolor(1);
			rectangle(299,189,461,209);
		}
		else
		if((mouse_x>300)&&(mouse_x<460)&&(mouse_y>220)&&(mouse_y<238))
		{
			setcolor(1);
			rectangle(299,219,461,239);
		}
		else
		{
			setcolor(7);
			rectangle(299,189,461,209);
			rectangle(299,219,461,239);
			rectangle(179,251,226,273);
			rectangle(424,251,481,273);
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			if((mouse_x>180)&&(mouse_x<225)&&(mouse_y>252)&&(mouse_y<272))
			{
				show_mouse();
				setcolor(8);
				rectangle(299,189,461,209);
				rectangle(299,219,461,239);
				rectangle(179,251,226,273);
				rectangle(424,251,481,273);
				::quit=exit();
				if(::quit==1)
					logindraw(username,password);
				else
					break;
			}
			else
			if((mouse_x>300)&&(mouse_x<460)&&(mouse_y>190)&&(mouse_y<208))
			{
				setcolor(1);
				rectangle(299,189,461,209);
				inputstr(303,190,username);
				setcolor(7);
				rectangle(299,189,461,209);
				setcolor(1);
			}
			else
			if((mouse_x>300)&&(mouse_x<460)&&(mouse_y>220)&&(mouse_y<238))
			{
				setcolor(1);
				rectangle(299,219,461,239);
				inputpass(303,220,password);
				setcolor(7);
				rectangle(299,219,461,239);
				setcolor(1);
			}
			else
			if((mouse_x>425)&&(mouse_x<480)&&(mouse_y>252)&&(mouse_y<272))
			{
				setcolor(7);
				outtextxy(270,255,"Login Successful!");
				outtextxy(230,255,"Invalid Username/Password");
				setcolor(6);
				if(strcmpi(username,sdb.username)==0&&strcmp(password,sdb.password)==0)
				{
					outtextxy(270,255,"Login Successful!");
					delay(1000);
					break;
				}
				else
				{
					setcolor(4);
					outtextxy(230,255,"Invalid Username/Password");
					setcolor(6);
				}
			}
			show_mouse();
		}
	}
}

void logindraw(char username[],char password[])
{
	cleardevice();
	setbkcolor(7);
	setcolor(6);
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(3,0,3);
	outtextxy(250,150,"Welcome!");
	settextstyle(2,0,5);
	setcolor(1);
	rectangle(180,180,480,250);
	rectangle(300,190,460,208);
	rectangle(300,220,460,238);
	outtextxy(200,190,"User Name :");
	outtextxy(200,220,"Password  :");
	setcolor(6);
	outtextxy(190,253,"Exit");
	outtextxy(435,253,"Login");
	rectangle(180,252,225,272);
	rectangle(425,252,480,272);
	setcolor(1);
	outtextxy(303,190,username);
	outpassxy(303,220,password);
}

int mainpage()
{
	mainpagedraw();
	if (detect_mouse())
		show_mouse();
	while (::quit==1)
	{
		test_mouse();
		utime();
		if((mouse_x>615)&&(mouse_x<635)&&(mouse_y>4)&&(mouse_y<26))
		{
			setcolor(6);
			rectangle(614,3,636,27);
		}
		else
		if((mouse_x>4)&&(mouse_x<24)&&(mouse_y>4)&&(mouse_y<26))
		{
			setcolor(6);
			rectangle(3,3,25,27);
		}
		else
		if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>120)&&(mouse_y<146))
		{
			setcolor(6);
			rectangle(444,119,631,147);
		}
		else
		if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>156)&&(mouse_y<182))
		{
			setcolor(6);
			rectangle(444,155,631,183);
		}
		else
		if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>192)&&(mouse_y<216))
		{
			setcolor(6);
			rectangle(444,191,631,217);
		}
		else
		if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>228)&&(mouse_y<254))
		{
			setcolor(6);
			rectangle(444,227,631,255);
		}
		else
		if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>264)&&(mouse_y<290))
		{
			setcolor(6);
			rectangle(444,263,631,291);
		}
		else
		if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>300)&&(mouse_y<326))
		{
			setcolor(6);
			rectangle(444,299,631,325);
		}
		else
		if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>336)&&(mouse_y<362))
		{
			setcolor(6);
			rectangle(444,335,631,363);
		}
		else
		{
			setcolor(7);
			rectangle(444,119,631,147);
			rectangle(444,155,631,183);
			rectangle(444,191,631,217);
			rectangle(444,227,631,255);
			rectangle(444,263,631,291);
			rectangle(444,299,631,325);
			rectangle(444,335,631,363);
			line(25,5,25,27);
			line(5,27,25,27);
			line(614,5,614,27);
			line(614,27,634,27);
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			if((mouse_x>615)&&(mouse_x<635)&&(mouse_y>4)&&(mouse_y<26))
			{
				show_mouse();
				setcolor(8);
				rectangle(444,119,631,147);
				rectangle(444,155,631,183);
				rectangle(444,191,631,217);
				rectangle(444,227,631,255);
				rectangle(444,263,631,291);
				rectangle(444,299,631,325);
				rectangle(444,335,631,363);
				rectangle(444,371,631,399);
				line(25,5,25,27);
				line(5,27,25,27);
				line(614,5,614,27);
				line(614,27,634,27);
				utime(8);
				::quit=exit();
				if(::quit==1)
					mainpagedraw();
				else
					break;
			}
			else
			if((mouse_x>4)&&(mouse_x<24)&&(mouse_y>4)&&(mouse_y<26))
			{
				show_mouse();
				setcolor(8);
				rectangle(444,119,631,147);
				rectangle(444,155,631,183);
				rectangle(444,191,631,217);
				rectangle(444,227,631,255);
				rectangle(444,263,631,291);
				rectangle(444,299,631,325);
				rectangle(444,335,631,363);
				rectangle(444,371,631,399);
				line(25,5,25,27);
				line(5,27,25,27);
				line(614,5,614,27);
				line(614,27,634,27);
				utime(8);
				help();
				mainpagedraw();
			}
			else
			if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>120)&&(mouse_y<146))
				return 1;
			else
			if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>156)&&(mouse_y<182))
				return 2;
			else
			if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>192)&&(mouse_y<216))
				return 3;
			else
			if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>228)&&(mouse_y<254))
				return 4;
			else
			if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>264)&&(mouse_y<290))
				return 5;
			else
			if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>300)&&(mouse_y<326))
				return 6;
			else
			if((mouse_x>445)&&(mouse_x<630)&&(mouse_y>336)&&(mouse_y<362))
			{
				show_mouse();
				setcolor(8);
				rectangle(444,119,631,147);
				rectangle(444,155,631,183);
				rectangle(444,191,631,217);
				rectangle(444,227,631,255);
				rectangle(444,263,631,291);
				rectangle(444,299,631,325);
				rectangle(444,335,631,363);
				rectangle(444,371,631,399);
				line(25,5,25,27);
				line(5,27,25,27);
				line(614,5,614,27);
				line(614,27,634,27);
				utime(8);
				::quit=exit();
				if(::quit==1)
					mainpagedraw();
				else
					break;
			}
			show_mouse();
		}
	}
	return 7;
}

void mainpagedraw()
{
	cleardevice();
	setbkcolor(7);
	setcolor(6);
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(3,0,4);
	outtextxy(120,15,"Library Management System!");
	rectangle(4,80,635,81);
	//Corner Buttons
	settextstyle(3,0,3);
	rectangle(615,4,635,26);
	outtextxy(620,0,"X");
	rectangle(4,4,24,26);
	outtextxy(9,0,"?");
	//Main Buttons
	rectangle(445,120,630,146);
	outtextxy(450,116,"Lend");
	rectangle(445,156,630,182);
	outtextxy(450,152,"Return");
	rectangle(445,192,630,216);
	outtextxy(450,188,"Book Details");
	rectangle(445,228,630,254);
	outtextxy(450,224,"Member Details");
	rectangle(445,264,630,290);
	outtextxy(450,260,"Modify database");
	rectangle(445,300,630,326);
	outtextxy(450,296,"Settings");
	rectangle(445,336,630,362);
	outtextxy(450,332,"Exit");
	//Info Bars
	calendar();
	infobox();
}

void lend()
{
	int Flag=0;
	readb=readm=0;
	int bstatus,mstatus;
	char book[25],member[25],date[9];
	do
	{
		bstatus=mstatus=0;
		cleardevice();
		setbkcolor(7);
		setcolor(6);
		rectangle(3,3,636,476);
		rectangle(4,4,635,475);
		settextstyle(1,0,5);
		outtextxy(250,10,"Lend");
		line(4,70,635,70);
		settextstyle(3,0,3);
		outtextxy(50,150,"Select Book");
		outtextxy(50,190,"Select Member");
		rectangle(220,150,520,180);
		rectangle(220,190,520,220);
		outtextxy(50,400,"<Cancel                              Process>");
		rectangle(44,402,150,435);
		rectangle(475,402,596,435);
		if(readb==1)
		{
			bdb.rtitle(book,bstatus);
			outtextxy(224,150,book);
		}
		else
			book[0]='\0';
		if(readm==1)
		{
			mdb.rname(member,mstatus);
			outtextxy(224,190,member);
		}
		else
			member[0]='\0';
		if (detect_mouse())
			show_mouse();
		for(;;)
		{
			test_mouse();
			if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>150)&&(mouse_y<180))
			{
				setcolor(6);
				rectangle(219,149,521,181);
			}
			else
			if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>190)&&(mouse_y<220))
			{
				setcolor(6);
				rectangle(219,189,521,221);
			}
			else
			if((mouse_x>44)&&(mouse_x<150)&&(mouse_y>402)&&(mouse_y<435))
			{
				setcolor(6);
				rectangle(43,401,151,436);
			}
			else
			if((mouse_x>475)&&(mouse_x<596)&&(mouse_y>402)&&(mouse_y<435))
			{
				setcolor(6);
				rectangle(474,401,597,436);
			}
			else
			{
				setcolor(7);
				rectangle(219,149,521,181);
				rectangle(219,189,521,221);
				rectangle(43,401,151,436);
				rectangle(474,401,597,436);
			}
			if(mouse_b & 1)
			{
				if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>150)&&(mouse_y<180))
				{
					hide_mouse();
					bdb.showtable(3);
					show_mouse();
					break;
				}
				else
				if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>190)&&(mouse_y<220))
				{
					hide_mouse();
					mdb.showtable(3);
					show_mouse();
					break;
				}
				else
				if((mouse_x>44)&&(mouse_x<150)&&(mouse_y>402)&&(mouse_y<435))
				{
					Flag=1;
					break;
				}
				else
				if((mouse_x>475)&&(mouse_x<596)&&(mouse_y>402)&&(mouse_y<435))
				{
					setcolor(7);
					outtextxy(250,350,"Please Select a Member");
					outtextxy(250,350,"Please Select a Book");
					outtextxy(220,350,"The Book Has Already been Loaned");
					outtextxy(220,350,"The Member has Already Loaned another Book");
					outtextxy(10,350,"This Book cannot be loaned due to insufficient Copies");
					setcolor(6);
					if(book[0]=='\0')
						outtextxy(250,350,"Please Select a Book");
					else
					if(member[0]=='\0')
						outtextxy(250,350,"Please Select a Member");
					else
					if(bstatus==1)
						outtextxy(220,350,"The Book Has Already been Loaned");
					else
					if(mstatus==1)
						outtextxy(220,350,"The Member has Already Loaned another Book");
					else
					{
						struct dosdate_t d;
						_dos_getdate(&d);
						char D[3],M[3],Y[3],curd[9],dued[9];
						float J;
						numtostr(d.day,D,1);
						numtostr(d.month,M,1);
						numtostr(d.year,Y,1);
						curd[0]='\0';
						strcat(curd,D);
						strcat(curd,"\\");
						strcat(curd,M);
						strcat(curd,"\\");
						strcat(curd,Y);
						J=gdtojd(curd);
						J+=sdb.loan_days;
						jdtogd(dued,J);
						Flag=bdb.loan(dued);
						char filename[15],bookname[20],mcode[10];
						int dummy;
						bdb.rtitle(bookname,dummy);
						strcpy(log.date,curd);
						strcpy(log.action,"User Borrowed \"");
						strcat(log.action,bookname);
						strcat(log.action,"\"                                                              ");
						mdb.rcode(mcode);
						strcpy(filename,mcode);
						strcat(filename,".txt");
						ofstream LOG(filename,ios::app);
						LOG.write((char*)&log,sizeof(log));
						LOG.close();
						if(Flag==1)
							break;

					}
				}
			}
		}
		hide_mouse();
	}
	while(Flag==0);
}

void returns()
{
	int Flag=0;
	readb=readm=0;
	int bstatus,mstatus;
	char book[25],member[25],date[9];
	do
	{
		bstatus=mstatus=0;
		cleardevice();
		setbkcolor(7);
		setcolor(6);
		rectangle(3,3,636,476);
		rectangle(4,4,635,475);
		settextstyle(1,0,5);
		outtextxy(250,10,"Return");
		line(4,70,635,70);
		settextstyle(3,0,3);
		outtextxy(50,150,"Select Book");
		outtextxy(50,190,"Select Member");
		rectangle(220,150,520,180);
		rectangle(220,190,520,220);
		outtextxy(50,400,"<Cancel                              Process>");
		rectangle(44,402,150,435);
		rectangle(475,402,596,435);
		if(readb==1)
		{
			bdb.rtitle(book,bstatus);
			outtextxy(224,150,book);
		}
		else
			book[0]='\0';
		if(readm==1)
		{
			mdb.rname(member,mstatus);
			outtextxy(224,190,member);
		}
		else
			member[0]='\0';
		if (detect_mouse())
			show_mouse();
		for(;;)
		{
			test_mouse();
			if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>150)&&(mouse_y<180))
			{
				setcolor(6);
				rectangle(219,149,521,181);
			}
			else
			if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>190)&&(mouse_y<220))
			{
				setcolor(6);
				rectangle(219,189,521,221);
			}
			else
			if((mouse_x>44)&&(mouse_x<150)&&(mouse_y>402)&&(mouse_y<435))
			{
				setcolor(6);
				rectangle(43,401,151,436);
			}
			else
			if((mouse_x>475)&&(mouse_x<596)&&(mouse_y>402)&&(mouse_y<435))
			{
				setcolor(6);
				rectangle(474,401,597,436);
			}
			else
			{
				setcolor(7);
				rectangle(219,149,521,181);
				rectangle(219,189,521,221);
				rectangle(43,401,151,436);
				rectangle(474,401,597,436);
			}
			if(mouse_b & 1)
			{
				if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>150)&&(mouse_y<180))
				{
					hide_mouse();
					bdb.showtable(3);
					show_mouse();
					break;
				}
				else
				if((mouse_x>220)&&(mouse_x<520)&&(mouse_y>190)&&(mouse_y<220))
				{
					hide_mouse();
					mdb.showtable(3);
					show_mouse();
					break;
				}
				else
				if((mouse_x>44)&&(mouse_x<150)&&(mouse_y>402)&&(mouse_y<435))
				{
					Flag=1;
					break;
				}
				else
				if((mouse_x>475)&&(mouse_x<596)&&(mouse_y>402)&&(mouse_y<435))
				{
					setcolor(7);
					outtextxy(250,350,"Please Select a Member");
					outtextxy(250,350,"Please Select a Book");
					outtextxy(220,350,"The Book Has Not been Loaned");
					outtextxy(220,350,"The Member has Not Loaned a Book");
					setcolor(6);
					if(book[0]=='\0')
						outtextxy(250,350,"Please Select a Book");
					else
					if(member[0]=='\0')
						outtextxy(250,350,"Please Select a Member");
					else
					if(bstatus==0)
						outtextxy(220,350,"The Book Has Not been Loaned");
					else
					if(mstatus==0)
						outtextxy(220,350,"The Member has Not Loaned a Book");
					else
					{
						struct dosdate_t d;
						_dos_getdate(&d);
						char D[3],M[3],Y[3],curd[9],dued[9];
						float J,J2,diff;
						numtostr(d.day,D,1);
						numtostr(d.month,M,1);
						numtostr(d.year,Y,1);
						curd[0]='\0';
						strcat(curd,D);
						strcat(curd,"\\");
						strcat(curd,M);
						strcat(curd,"\\");
						strcat(curd,Y);
						J=gdtojd(curd);
						J2=bdb.ret();
						char msg[100],fine[15];
						strcpy(msg,"The Member has to Pay QR ");
						diff=J-J2;
						diff*=sdb.fine_per_day;
						if(J>J2)
						{
							numtostr(diff,fine);
							strcat(msg,fine);
							strcat(msg," /-");
							outtextxy(10,350,msg);
							getch();
						}
						numtostr(d.day,D,1);
						numtostr(d.month,M,1);
						numtostr(d.year,Y,1);
						log.date[0]='\0';
						strcat(log.date,D);
						strcat(log.date,"\\");
						strcat(log.date,M);
						strcat(log.date,"\\");
						strcat(log.date,Y);
						char filename[15],bookname[20],mcode[10];
						int dummy;
						bdb.rtitle(bookname,dummy);
						strcpy(log.date,curd);
						strcpy(log.action,"User Returned \"");
						strcat(log.action,bookname);
						strcat(log.action,"\"                                                              ");
						mdb.rcode(mcode);
						strcpy(filename,mcode);
						strcat(filename,".txt");
						ofstream LOG(filename,ios::app);
						LOG.write((char*)&log,sizeof(log));
						LOG.close();
						Flag=1;
						break;
					}
				}
			}
		}
		hide_mouse();
	}
	while(Flag==0);
}

void settings()
{
	cleardevice();
	char nuser[20],npass[20],cuser[20],cpass[20],temp[5];
	nuser[0]='\0';
	npass[0]='\0';
	cuser[0]='\0';
	cpass[0]='\0';
	setbkcolor(7);
	setcolor(6);
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(1,0,5);
	outtextxy(250,10,"Settings");
	line(4,70,635,70);
	settextstyle(3,0,3);
	outtextxy(70,100,"Enter Current Details to Make Any Changes ");
	outtextxy(70,140,"Username ");
	outtextxy(70,180,"Password ");
	rectangle(50,96,600,230);
	setcolor(1);
	rectangle(200,140,550,170);
	rectangle(200,180,550,210);
	setcolor(6);
	rectangle(50,240,600,450);
	outtextxy(70,244,"Username ");
	outtextxy(70,284,"Password ");
	outtextxy(70,324,"Minumum Copies");
	outtextxy(70,364,"Fine Per Day");
	outtextxy(70,404,"No of Days to Loan");
	setcolor(1);
	rectangle(200,244,550,274);
	rectangle(200,284,550,314);
	rectangle(300,324,450,354);
	rectangle(300,364,450,394);
	rectangle(300,404,450,434);
	setcolor(6);
	outtextxy(500,324,"Update");
	outtextxy(500,364,"Reset");
	outtextxy(500,404,"Cancel");
	rectangle(496,324,580,354);
	rectangle(496,364,580,394);
	rectangle(496,404,580,434);
	//Printing Initial Values
	setcolor(1);
	numtostr(sdb.min_copies,temp,3);
	outtextxy(304,324,temp);
	numtostr(sdb.fine_per_day,temp,3);
	outtextxy(304,364,temp);
	numtostr(sdb.loan_days,temp,3);
	outtextxy(304,404,temp);
	setcolor(6);
	if(detect_mouse())
		show_mouse();
	for(;;)
	{
		test_mouse();
		if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>140)&&(mouse_y<170))
		{
			setcolor(1);
			rectangle(199,139,551,171);
		}
		else
		if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>180)&&(mouse_y<210))
		{
			setcolor(1);
			rectangle(199,179,551,211);
		}
		else
		if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>244)&&(mouse_y<274))
		{
			setcolor(1);
			rectangle(199,243,551,275);
		}
		else
		if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>284)&&(mouse_y<314))
		{
			setcolor(1);
			rectangle(199,283,551,315);
		}
		else
		if((mouse_x>300)&&(mouse_x<450)&&(mouse_y>324)&&(mouse_y<354))
		{
			setcolor(1);
			rectangle(299,323,451,355);
		}
		else
		if((mouse_x>300)&&(mouse_x<450)&&(mouse_y>364)&&(mouse_y<394))
		{
			setcolor(1);
			rectangle(299,363,451,395);
		}
		else
		if((mouse_x>300)&&(mouse_x<450)&&(mouse_y>404)&&(mouse_y<434))
		{
			setcolor(1);
			rectangle(299,403,451,435);
		}
		else
		if((mouse_x>496)&&(mouse_x<580)&&(mouse_y>324)&&(mouse_y<354))
		{
			setcolor(6);
			rectangle(497,325,581,355);
		}
		else
		if((mouse_x>496)&&(mouse_x<580)&&(mouse_y>364)&&(mouse_y<394))
		{
			setcolor(6);
			rectangle(497,363,581,395);
		}
		else
		if((mouse_x>496)&&(mouse_x<580)&&(mouse_y>404)&&(mouse_y<434))
		{
			setcolor(6);
			rectangle(497,405,581,435);
		}
		else
		{
			setcolor(7);
			rectangle(199,139,551,171);
			rectangle(199,179,551,211);
			rectangle(199,243,551,275);
			rectangle(199,283,551,315);
			rectangle(299,323,451,355);
			rectangle(299,363,451,395);
			rectangle(299,403,451,435);
			rectangle(497,325,581,355);
			rectangle(497,363,581,395);
			rectangle(497,405,581,435);
		}
		if(mouse_b & 1)
		{
			hide_mouse();
			if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>140)&&(mouse_y<170))
				inputstr(204,140,cuser);
			else
			if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>180)&&(mouse_y<210))
				inputpass(204,180,cpass);
			else
			if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>244)&&(mouse_y<274))
				inputstr(204,244,nuser);
			else
			if((mouse_x>200)&&(mouse_x<550)&&(mouse_y>284)&&(mouse_y<314))
				inputpass(204,284,npass);
			else
			if((mouse_x>300)&&(mouse_x<450)&&(mouse_y>324)&&(mouse_y<354))
			{
				char tnum[5];
				numtostr(sdb.min_copies,tnum,3);
				inputstr(304,324,tnum);
				sdb.min_copies=strtonum(tnum);
			}
			else
			if((mouse_x>300)&&(mouse_x<450)&&(mouse_y>364)&&(mouse_y<394))
			{
				char tnum[5];
				numtostr(sdb.fine_per_day,tnum,3);
				inputstr(304,364,tnum);
				sdb.fine_per_day=strtonum(tnum);
			}
			else
			if((mouse_x>300)&&(mouse_x<450)&&(mouse_y>404)&&(mouse_y<434))
			{
				char tnum[5];
				numtostr(sdb.loan_days,tnum,3);
				inputstr(304,404,tnum);
				sdb.loan_days=strtonum(tnum);
			}
			else
			if((mouse_x>496)&&(mouse_x<580)&&(mouse_y>324)&&(mouse_y<354))
			{
				if((strcmpi(cuser,sdb.username)==0)&&(strcmpi(cpass,sdb.password)==0))
				{
					strcpy(sdb.username,nuser);
					strcpy(sdb.password,npass);
					ofstream set("CONFIG.DAT");
					set.write((char*)&sdb,sizeof(sdb));
					set.close();
					break;
				}
				else
					outtextxy(150,445,"Invalid Username/Password!!!");
			}
			else
			if((mouse_x>496)&&(mouse_x<580)&&(mouse_y>364)&&(mouse_y<394))
			{
				if((strcmpi(cuser,sdb.username)==0)&&(strcmpi(cpass,sdb.password)==0))
				{
					strcpy(sdb.username,"");
					strcpy(sdb.password,"");
					sdb.total_books=0;
					sdb.total_members=0;
					sdb.loaned_books=0;
					sdb.loan_days=14;
					sdb.min_copies=5;
					sdb.fine_per_day=10;
					ofstream set("CONFIG.DAT");
					set.write((char*)&sdb,sizeof(sdb));
					set.close();
					set.open("BOOKS.DAT");
					set.close();
					set.open("MEMBER.DAT");
					set.close();
					remove("BOOKS.DAT");
					remove("MEMBER.DAT");
					break;
				}
				else
					outtextxy(150,445,"Invalid Username/Password!!!");
			}
			else
			if((mouse_x>496)&&(mouse_x<580)&&(mouse_y>404)&&(mouse_y<434))
				break;
			show_mouse();
		}
	}
	ifstream r("CONFIG.DAT");
	r.read((char*)&sdb,sizeof(sdb));
	r.close();
}

int tablebutton(int rec)
{
	settextstyle(2,0,5);
	if (detect_mouse())
		show_mouse();
	while (::quit==1)
	{
		test_mouse();
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>50)&&(mouse_y<70))
		{
			setcolor(1);
			rectangle(19,49,621,71);
		}
		else
		if((mouse_x>40)&&(mouse_x<230)&&(mouse_y>444)&&(mouse_y<463))
		{
			setcolor(6);
			rectangle(39,443,231,464);
		}
		else
		if((mouse_x>455)&&(mouse_x<533)&&(mouse_y>444)&&(mouse_y<463))
		{
			setcolor(6);
			rectangle(454,443,534,464);
		}
		else
		if((mouse_x>537)&&(mouse_x<615)&&(mouse_y>444)&&(mouse_y<463))
		{
			setcolor(6);
			rectangle(536,443,616,464);
		}
		else
		if((mouse_x>172)&&(mouse_x<255)&&(mouse_y>78)&&(mouse_y<97))
		{
			setcolor(1);
			rectangle(171,77,256,98);
		}
		else
		if((mouse_x>280)&&(mouse_x<353)&&(mouse_y>78)&&(mouse_y<97))
		{
			setcolor(1);
			rectangle(279,77,354,98);
		}
		else
		if((mouse_x>373)&&(mouse_x<445)&&(mouse_y>78)&&(mouse_y<97))
		{
			setcolor(1);
			rectangle(372,77,446,98);
		}
		else
		if((mouse_x>473)&&(mouse_x<575)&&(mouse_y>78)&&(mouse_y<97))
		{
			setcolor(1);
			rectangle(472,77,576,98);
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>124)&&(mouse_y<147)&&rec>0)
		{
			setcolor(6);
			outtextxy(10,124,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>148)&&(mouse_y<171)&&rec>1)
		{
			setcolor(6);
			outtextxy(10,148,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>172)&&(mouse_y<195)&&rec>2)
		{
			setcolor(6);
			outtextxy(10,172,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>196)&&(mouse_y<219)&&rec>3)
		{
			setcolor(6);
			outtextxy(10,196,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>220)&&(mouse_y<243)&&rec>4)
		{
			setcolor(6);
			outtextxy(10,220,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>244)&&(mouse_y<267)&&rec>5)
		{
			setcolor(6);
			outtextxy(10,244,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>268)&&(mouse_y<291)&&rec>6)
		{
			setcolor(6);
			outtextxy(10,268,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>292)&&(mouse_y<315)&&rec>7)
		{
			setcolor(6);
			outtextxy(10,292,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>316)&&(mouse_y<339)&&rec>8)
		{
			setcolor(6);
			outtextxy(10,316,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>340)&&(mouse_y<363)&&rec>9)
		{
			setcolor(6);
			outtextxy(10,340,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>364)&&(mouse_y<387)&&rec>10)
		{
			setcolor(6);
			outtextxy(10,364,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>388)&&(mouse_y<411)&&rec>11)
		{
			setcolor(6);
			outtextxy(10,388,"o");
		}
		else
		if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>412)&&(mouse_y<435)&&rec>12)
		{
			setcolor(6);
			outtextxy(10,412,"o");
		}
		else
		{
			setcolor(7);
			rectangle(19,49,621,71);
			rectangle(39,443,231,464);
			rectangle(454,443,534,464);
			rectangle(536,443,616,464);
			rectangle(171,77,256,98);
			rectangle(279,77,354,98);
			rectangle(372,77,446,98);
			rectangle(472,77,576,98);
			outtextxy(10,124,"o");
			outtextxy(10,148,"o");
			outtextxy(10,172,"o");
			outtextxy(10,196,"o");
			outtextxy(10,220,"o");
			outtextxy(10,244,"o");
			outtextxy(10,268,"o");
			outtextxy(10,292,"o");
			outtextxy(10,316,"o");
			outtextxy(10,340,"o");
			outtextxy(10,364,"o");
			outtextxy(10,388,"o");
			outtextxy(10,412,"o");
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>50)&&(mouse_y<70))
				return 1;
			else
			if((mouse_x>40)&&(mouse_x<230)&&(mouse_y>444)&&(mouse_y<463))
				return 0;
			else
			if((mouse_x>455)&&(mouse_x<533)&&(mouse_y>444)&&(mouse_y<463))
				return 51;
			else
			if((mouse_x>537)&&(mouse_x<615)&&(mouse_y>444)&&(mouse_y<463))
				return 52;
			else
			if((mouse_x>172)&&(mouse_x<255)&&(mouse_y>78)&&(mouse_y<97))
				return 11;
			else
			if((mouse_x>280)&&(mouse_x<353)&&(mouse_y>78)&&(mouse_y<97))
				return 12;
			else
			if((mouse_x>373)&&(mouse_x<445)&&(mouse_y>78)&&(mouse_y<97))
				return 13;
			else
			if((mouse_x>473)&&(mouse_x<575)&&(mouse_y>78)&&(mouse_y<97))
				return 14;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>124)&&(mouse_y<147)&&rec>0)
				return 101;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>148)&&(mouse_y<171)&&rec>1)
				return 102;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>172)&&(mouse_y<195)&&rec>2)
				return 103;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>196)&&(mouse_y<219)&&rec>3)
				return 104;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>220)&&(mouse_y<243)&&rec>4)
				return 105;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>244)&&(mouse_y<267)&&rec>5)
				return 106;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>268)&&(mouse_y<291)&&rec>6)
				return 107;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>292)&&(mouse_y<315)&&rec>7)
				return 108;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>316)&&(mouse_y<339)&&rec>8)
				return 109;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>340)&&(mouse_y<363)&&rec>9)
				return 110;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>364)&&(mouse_y<387)&&rec>10)
				return 111;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>388)&&(mouse_y<411)&&rec>11)
				return 112;
			else
			if((mouse_x>20)&&(mouse_x<620)&&(mouse_y>412)&&(mouse_y<435)&&rec>12)
				return 113;
			show_mouse();
		}
	}
	return 0;
}

int modify()
{
	cleardevice();
	setcolor(6);
	settextstyle(3,0,4);
	outtextxy(220,5,"Modify Database");
	rectangle(3,3,636,476);
	rectangle(4,4,635,475);
	settextstyle(3,0,2);
	outtextxy(100,100,"Book Database               Member Database");
	line(320,95,320,400);
	outtextxy(120,150,"Add                              Add");
	outtextxy(118,200,"Edit                              Edit");
	outtextxy(118,250,"Delete                            Delete");
	outtextxy(50,420,"< Back to Main");
	rectangle(115,150,180,180);
	rectangle(113,200,180,230);
	rectangle(113,250,180,280);
	rectangle(450,150,515,180);
	rectangle(448,200,515,230);
	rectangle(448,250,515,280);
	rectangle(45,420,210,450);
	if (detect_mouse())
		show_mouse();
	while (::quit==1)
	{
		test_mouse();
		if((mouse_x>115)&&(mouse_x<180)&&(mouse_y>150)&&(mouse_y<180))
		{
			setcolor(6);
			rectangle(114,149,181,181);
		}
		else
		if((mouse_x>113)&&(mouse_x<180)&&(mouse_y>200)&&(mouse_y<230))
		{
			setcolor(6);
			rectangle(112,199,181,231);
		}
		else
		if((mouse_x>113)&&(mouse_x<180)&&(mouse_y>250)&&(mouse_y<280))
		{
			setcolor(6);
			rectangle(112,249,181,281);
		}
		else
		if((mouse_x>450)&&(mouse_x<515)&&(mouse_y>150)&&(mouse_y<180))
		{
			setcolor(6);
			rectangle(449,149,516,181);
		}
		else
		if((mouse_x>448)&&(mouse_x<515)&&(mouse_y>200)&&(mouse_y<230))
		{
			setcolor(6);
			rectangle(447,199,516,231);
		}
		else
		if((mouse_x>448)&&(mouse_x<515)&&(mouse_y>250)&&(mouse_y<280))
		{
			setcolor(6);
			rectangle(447,249,516,281);
		}
		else
		if((mouse_x>45)&&(mouse_x<210)&&(mouse_y>420)&&(mouse_y<450))
		{
			setcolor(6);
			rectangle(44,419,211,451);
		}
		else
		{
			setcolor(7);
			rectangle(114,149,181,181);
			rectangle(112,199,181,231);
			rectangle(112,249,181,281);
			rectangle(449,149,516,181);
			rectangle(447,199,516,231);
			rectangle(447,249,516,281);
			rectangle(44,419,211,451);
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			if((mouse_x>45)&&(mouse_x<210)&&(mouse_y>420)&&(mouse_y<450))
				return 0;
			else
			if((mouse_x>115)&&(mouse_x<180)&&(mouse_y>150)&&(mouse_y<180))
				return 1;
			else
			if((mouse_x>113)&&(mouse_x<180)&&(mouse_y>200)&&(mouse_y<230))
				return 2;
			else
			if((mouse_x>113)&&(mouse_x<180)&&(mouse_y>250)&&(mouse_y<280))
				return 3;
			else
			if((mouse_x>450)&&(mouse_x<515)&&(mouse_y>150)&&(mouse_y<180))
				return 4;
			else
			if((mouse_x>448)&&(mouse_x<515)&&(mouse_y>200)&&(mouse_y<230))
				return 5;
			else
			if((mouse_x>448)&&(mouse_x<515)&&(mouse_y>250)&&(mouse_y<280))
				return 6;
			show_mouse();
		}
	}
	return 0;
}

int exit()
{
	setbkcolor(8);
	setfillstyle(SOLID_FILL,7);
	hide_mouse();
	bar(230,180,440,280);
	setcolor(4);
	settextstyle(2,0,5);
	rectangle(230,180,440,280);
	outtextxy(250,210,"Are You Sure You");
	outtextxy(350,220,"want Exit?");
	outtextxy(273,255,"Yes");
	outtextxy(388,255,"No");
	rectangle(270,255,300,270);
	rectangle(380,255,410,270);
	setcolor(1);
	show_mouse();
	for(;;)
	{
		test_mouse();
		setcolor(4);
		if((mouse_x>380)&&(mouse_x<410)&&(mouse_y>255)&&(mouse_y<270))
			rectangle(379,254,411,271);
		else
		if((mouse_x>270)&&(mouse_x<300)&&(mouse_y>255)&&(mouse_y<270))
			rectangle(269,254,301,271);
		else
		{
			setcolor(7);
			rectangle(379,254,411,271);
			rectangle(269,254,301,271);
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			if((mouse_x>380)&&(mouse_x<410)&&(mouse_y>255)&&(mouse_y<270))
				return 1;
			else
			if((mouse_x>270)&&(mouse_x<300)&&(mouse_y>255)&&(mouse_y<270))
				return 0;
			show_mouse();
		}
	}
}

void help()
{
	setbkcolor(8);
	setfillstyle(SOLID_FILL,7);
	hide_mouse();
	bar(230,180,440,280);
	setcolor(1);
	settextstyle(2,0,5);
	rectangle(230,180,440,280);
	outtextxy(235,188,"See Documentation included");
	outtextxy(265,200,"in Package for Help.");
	outtextxy(310,225,"Copyright");
	outtextxy(235,238," 2009 Fahim Imaduddin Dalvi ");
	outtextxy(328,255,"OK");
	rectangle(320,255,350,270);
	setcolor(1);
	show_mouse();
	for(;;)
	{
		test_mouse();
		setcolor(1);
		if((mouse_x>320)&&(mouse_x<350)&&(mouse_y>255)&&(mouse_y<270))
			rectangle(319,254,351,271);
		else
		{
			setcolor(7);
			rectangle(319,254,351,271);
		}
		if (mouse_b & 1)
		{
			hide_mouse();
			if((mouse_x>320)&&(mouse_x<350)&&(mouse_y>255)&&(mouse_y<270))
				break;
			show_mouse();
		}
	}
}

void calendar()
{
	struct dosdate_t d;
	settextstyle(2,0,5);
	_dos_getdate(&d);
	setcolor(1);
	rectangle(36,270,220,420);
	rectangle(35,269,221,421);
	line(36,293,220,293);
	line(36,294,220,294);
	line(36,315,220,315);
	line(36,400,220,400);
	line(36,401,220,401);
	char month[][12]={"January","February","March","April","May","June","July","August","September","October","November","December"};
	int date,day,max,x,y;
	if(d.month==1||d.month==3||d.month==5||d.month==7||d.month==8||d.month==10||d.month==12)
		max=31;
	else
	if(d.month==2)
	{
		if(d.year%4==0)
			max=29;
		else
			max=28;
	}
	else
		max=30;
	date=d.day;
	day=d.dayofweek;
	for(int i=date;i>0;i--)
	{
		if(day==0)
			day=6;
		else
			day--;
	}
	day++;
	outtextxy(50,272,month[d.month-1]);
	outnum(200,272,d.year);
	outtextxy(50,296,"Su Mo Tu We Th Fr Sa");
	x=55;
	y=310;
	for(i=day;i>0;i--)
	{
		outtextxy(x,y,"   ");
		x+=24;
	}
	for(i=1;i<=max;i++)
	{
		if(day==7)
		{
			day=0;
			y+=14;
			x=55;
		}
		outnum(x,y,i);
		x+=24;
		if(i==d.day-1)
			rectangle(x-10,y,x+10,y+15);
		day++;
	}
}

void infobox()
{
	setcolor(1);
	rectangle(36,100,220,220);
	rectangle(35,99,221,221);
	line(36,123,220,123);
	line(36,124,220,124);
	settextstyle(2,0,5);
	outtextxy(100,105,"Infobox");
	outtextxy(40,130," Books        : ");
	outtextxy(40,150," Members      : ");
	outtextxy(40,170," Loaned Books : ");
	outnum(180,130,sdb.total_books);
	outnum(180,150,sdb.total_members);
	outnum(180,170,sdb.loaned_books);
}

void utime(int color)
{
	static int ts=10001;
	if(ts>10000)
	{
		struct time t;
		gettime(&t);
		setcolor(1);
		setfillstyle(SOLID_FILL,color);
		bar(37,402,219,419);
		outnum(100,400,t.ti_hour);
		outtextxy(120,400,":");
		outnum(150,400,t.ti_min);
		outtextxy(170,400,":");
		outnum(200,400,t.ti_sec);
		ts++;
	}
	else
		ts=0;
}

void inputstr(int x,int y,char str[])
{
	char ch;
	int c=strlen(str);
	while(ch!='\r')
	{
		ch=getch();
		if(ch!='\r'&&ch!='\b')
		{
			str[c]=ch;
			str[c+1]='\0';
			c++;
		}
		if(ch=='\b'&&c!=0)
		{
			setcolor(0);
			outtextxy(x,y,str);
			setcolor(1);
			str[c-1]=str[c];
			c--;
		}
		outtextxy(x,y,str);
	}
}

void inputpass(int x,int y,char str[])
{
	char ch;
	int c=strlen(str);
	while(ch!='\r')
	{
		ch=getch();
		if(ch!='\r'&&ch!='\b')
		{
			str[c]=ch;
			str[c+1]='\0';
			c++;
		}
		if(ch=='\b'&&c!=0)
		{
			setcolor(0);
			outpassxy(x,y,str);
			setcolor(1);
			str[c-1]=str[c];
			c--;
		}
		outpassxy(x,y,str);
	}
}

void outpassxy(int x,int y,char pass[])
{
	int c;
	c=strlen(pass);
	char temp[20];
	for(int i=0;i<c;i++)
		temp[i]='*';
	temp[i]='\0';
	outtextxy(x,y,temp);
}

void numtostr(int number,char num[],int l)
{
	for(int i=l;i>=0;i--)
	{
		num[i]=(number%10+'0');
		number=number/10;
	}
	num[l+1]='\0';
}

int strtonum(char num[])
{
	int number=0;
	int l;
	l=strlen(num);
	for(int i=0;i<l;i++)
		number=(number*10)+(num[i]-'0');
	return number;
}

void outnum(int x,int y,int num,int spacing)
{
	int d,i=-spacing;
	if(num==0)
		outtextxy(x,y,"0");
	while(num!=0)
	{
		i+=spacing;
		d=num%10;
		if(d==1)
			outtextxy(x-i,y,"1");
		else
		if(d==2)
			outtextxy(x-i,y,"2");
		else
		if(d==3)
			outtextxy(x-i,y,"3");
		else
		if(d==4)
			outtextxy(x-i,y,"4");
		else
		if(d==5)
			outtextxy(x-i,y,"5");
		else
		if(d==6)
			outtextxy(x-i,y,"6");
		else
		if(d==7)
			outtextxy(x-i,y,"7");
		else
		if(d==8)
			outtextxy(x-i,y,"8");
		else
		if(d==9)
			outtextxy(x-i,y,"9");
		else
		if(d==0)
			outtextxy(x-i,y,"0");
		num=num/10;
	}
}

float gdtojd(char date[])
{
	char year[5],month[3],day[3];
	int y,m,d;
	float JD;
	day[0]=date[0];
	day[1]=date[1];
	day[2]='\0';
	month[0]=date[3];
	month[1]=date[4];
	month[2]='\0';
	year[0]='2';
	year[1]='0';
	year[2]=date[6];
	year[3]=date[7];
	year[4]='\0';
	y=strtonum(year);
	m=strtonum(month);
	d=strtonum(day);
	JD = 367*y-int((7*(y+int((m+9)/12)))/4)+int((275*m)/9)+d+1721013.5;
	JD=JD+720896;
	return JD;
}

void jdtogd(char date[],float jd)
{
int y,m,d;
jd=jd-1721119;
y=(4*jd-1)/146097;
jd=4*jd-1-146097*y;
d=jd/4;
jd=(4*d+3)/1461;
d=4*d+3-1461*jd;
d=(d+4)/4;
m=(5*d-3)/153;
d=5*d-3-153*m;
d=(d+5)/5;
y=100*y+jd;
if(m<10)
	m=m+3;
else
{
	m=m-9;
	y=y+1;
}
char day[3],month[3],year[3];
numtostr(d,day,1);
numtostr(m,month,1);
numtostr(y,year,1);
date[0]='\0';
strcat(date,day);
strcat(date,"\\");
strcat(date,month);
strcat(date,"\\");
strcat(date,year);
}
