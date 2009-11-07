#include<fstream.h>
#include<stdio.h>

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
}db;
void main()
{
ofstream file("CONFIG.DAT");
/*gets(db.username);
gets(db.password);
cin>>db.total_books>>db. total_members>>db. loaned_books;*/
db.username[0]='\0';
db.password[0]='\0';
db.total_books=db.total_members=db.loaned_books=0;
db.loan_days=14;
db.min_copies=5;
db.fine_per_day=10;
file.write((char*)&db,sizeof(db));
file.close();
file.open("BOOKS.DAT");
file.close();
remove("BOOKS.DAT");
file.open("MEMBER.DAT");
file.close();
remove("MEMBER.DAT");
}


