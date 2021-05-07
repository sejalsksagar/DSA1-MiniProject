#include<iostream>
#include<iomanip>

using namespace std;

class msg
{
	public:
		bool star;
		bool sent;
		bool read;
		string to;
		string from;
		string text;
		msg *link;

		msg()
		{
			star = false;
			sent = false;
			read = true;
			to = "";
			from = "";
			text = "";
			link = NULL;
		}
};

class user
{
	public:
		bool logged_in;
		string username;
		string password;
		msg *headS; 	//sent msg sll head
		msg *headR; 	//received msg sll head
		msg *headT;		//trash msg sll head
		user *next;
		user *prev;
		friend class messager;
		user()
		{
			logged_in = false;
			username = "";
			password = "";
			headS = NULL;
			headR = NULL;
			headT = NULL;
			next = NULL;
			prev = NULL;
		}

};

class messager
{
		user *start;
	public:
		messager()
		{
			start = NULL;
		}

		bool is_empty();	//returns true if no user has created account yet
		user* accept();		//takes input required while creating a new account
		void create(); 		//creates new user account & adds it to user dll(sign-up)
		void login();  		//to login to an existing account
		void remove(user *ptr); 		//to delete your account

		void activity(user *ptr);

		void display_inbox(user *ptr);		//displays list of received msgs
		void inbox_options(user *ptr);
		void read_inbox_msg(user *ptr);		//to read an inbox msg

		msg* msg_sent();		//takes input to send msg, updates receiver's inbox & returns pointer to sent msg
		void send_msg(user *ptr);		//calls msg_sent() & updates user's sent msg sll
		void display_sent(user *ptr);  //similar to display_inbox(user *ptr);
		void view_sent_msg(user *ptr);		//similar to void read_inbox_msg(user *ptr);

		void del_msg();
		void display_trash();
		void star_msg(msg *m);
		void display_starred();


};

//returns true if no user has created account yet
bool messager::is_empty()
{
	if (start == NULL)
		return true;
	else
		return false;
}

//takes input required while creating a new account
user* messager::accept()
{
	int flag = 0;
	user *tmp = new user();
	user *ptr1;
	do
	{
		ptr1 = start;
		cout << "\nEnter username to create : ";
		cin >> tmp->username;
		while (ptr1 != NULL)
		{
			if (ptr1->username == tmp->username)
			{
				cout << "\nEntered username already exists.";
				flag = 1;
				break;
			}
			ptr1 = ptr1->next;
		}
		if (ptr1 == NULL)
			flag = 0;
	} while (flag == 1);
	cout << "\nCreate password: ";
	cin >> tmp->password;
	return tmp;
}

//creates new user account & adds it to user dll(sign-up)
void messager::create()
{
	user *tmp, *ptr;
	tmp = accept();
	if (is_empty())
		start = tmp;
	else
	{
		ptr = start;
		while (ptr->next != NULL)
			ptr = ptr->next;

		ptr->next = tmp;
		tmp->prev = ptr;
	}
	cout<< "\nYour account has successfully created!";
}

//to login to an existing account
void messager::login()
{
	string un, pw;
	cout << "\nEnter username: ";
	cin >> un;
	for (user *ptr = start; ptr != NULL; ptr = ptr->next)
	{
		if (ptr->username == un)
		{
			cout << "\nEnter password: ";
			cin >> pw;
			if (ptr->password == pw)
			{
				ptr->logged_in = true;
				cout << "\nSuccessfully logged in.";
				activity(ptr);
				return;
			}
			else
			{
				cout << "\nIncorrect password. Try again.";
				return;
			}
		}
	}
	cout << "Username not found.";
}

//to delete your account //removes ptr from user dll
//void messager :: remove(user *ptr)

//displays list of received msgs
void messager :: display_inbox(user *ptr)
{
	cout<<"\n************* INBOX **************";

	if (ptr->headR == NULL)
		cout << "\nYou haven't received any messages yet!\n";
	else
	{
		int i = 1;
		cout << "\n----------------------------------------------------------------";
		cout<<"\n"<<setw(5)<<"Sr no."<<setw(15)<<"From"<<setw(15)<<"Message"<<setw(10)<<"Read"<<setw(10)<<"Starred";
		cout << "\n----------------------------------------------------------------";

		msg *ptrR = ptr->headR;
		while (ptrR != NULL)
		{
			cout<<"\n"<<setw(5)<<i<<setw(15)<<ptrR->from<<setw(15)<<ptrR->text.substr(0, 10)<<"..."<<setw(10)<<ptrR->read<<setw(10)<<ptrR->star;
			cout << "\n----------------------------------------------------------------";
			ptrR = ptrR->link;
			i++;
		}
	}
}

void messager :: inbox_options(user *ptr)
{
	int ch;
	do{
		display_inbox(ptr);
		cout << "\n********* INBOX OPTIONS **********";
		cout << "\n0. Exit";
		cout << "\n1. Read a message";
		cout << "\n2. Delete a message";
		cout << "\n3. Star/Unstar a message";
		cout << "\nEnter your choice: ";
		cin >> ch;

		switch(ch)
			{
			case 0: cout<<"\n------------------------------------";
					break;

			case 1: read_inbox_msg(ptr);
					break;

			case 2: //ak headR
					break;

			case 3: //ak
					break;
			}
	}while(ch!=0);
}

//to read an inbox msg
void messager :: read_inbox_msg(user *ptr)
{
	int no;
	if (ptr->headR != NULL)
	{
		cout<<"\n\nEnter message no. to read:";
		cin>>no;

		msg *mR = ptr->headR;
		for (int i=1; i<no; i++)
		{
			mR = mR->link;
			if(mR == NULL)
			{
				cout<<"\nInvalid message no.";
				return;
			}
		}


		cout << "\n******* MESSAGE "<<no<<" ********";
		cout << "\nFrom : " << mR->from;
		cout << "\nMessage : " << mR->text;
		cout << "\n-------------------------------\n";
		mR->read = true;
	}
}

//takes input to send msg, updates receiver's inbox & returns pointer to sent msg
msg* messager :: msg_sent()
{
	msg *m = new msg();
	user *ptrT; //pointer To whom user is sending msg
	int flag = 0;
	do
	{
		cout << "Enter username of user to message : ";
		cin >> m->to;
		getchar(); //'\n'

		//updating receiver's received msg sll
		for (ptrT = start; ptrT != NULL; ptrT = ptrT->next)
		{
			if (ptrT->username == m->to)
			{
				cout << "\nEnter message you want to send to @" << m->to << " :\n";
				getline(cin, m->text);
				cout << "\nMessage sent successfully to @" << m->to;
				m->read = false;
				flag = 1;

				//insert new msg at beginning of ptrT's received msg sll
				m->link = ptrT->headR;
				ptrT->headR = m;
				return m;
			}
		}

		if (ptrT == NULL)
			cout << "\nEntered username doesn't exist.";

	} while (flag == 0);
	return m;
}

//calls msg_sent() & updates user's sent msg sll
void messager :: send_msg(user *ptr)
{
	msg *ms = msg_sent(); //pointer to sent msg
	ms->from = ptr->username;

	//updating sender's (logged-in user's) sent msg sll
	msg *m = new msg();		//create new msg to update user's sent msgs sll
	m->sent = true;
	m->to = ms->to;
	m->from = ms->from;
	m->text = ms->text;

	//insert sent msg at beginning of sent sll
	m->link = ptr->headS;
	ptr->headS = m;
}

void messager :: view_sent_msg(user *ptr)
{
	if (ptr->headS == NULL)
		cout << "You haven't sent any messages yet!\n";
	else
	{
		msg *ptrS = ptr->headS;
		while (ptrS != NULL)
		{
			cout << "\nTo : " << ptrS->to;
			cout << "\nMessage : " << ptrS->text;
			cout << "\n-------------------------------\n";
			ptrS = ptrS->link;
		}
	}
}

void messager :: activity(user *ptr)
{
	int ch;
	do
	{
		cout << "\n********** HELLO " << ptr->username << " ! **********";
		cout << "\n0. Logout";
		cout << "\n1. Check inbox messages";
		cout << "\n2. Send a message";
		cout << "\n3. View sent messages";
		cout << "\n4. Search messages sent by an user";
		cout << "\n5. Search messages received by an user";
		cout << "\n6. View starred messages";
		cout << "\n7. View deleted messages";
		cout << "\nEnter your choice: ";
		cin >> ch;

		switch (ch)
		{
			case 0: ptr->logged_in = false;
					cout << "\nSuccessfully logged out.";
					return;

			case 1: inbox_options(ptr);
					break;

			case 2: send_msg(ptr);
					break;

			case 3: view_sent_msg(ptr); //ke
					break;

			case 4: //am
					break;

			case 5: //am
					break;

			case 6:
					break;

			default: cout<<"\nInvalid choice";
		}

	} while (ch != 0);
}

int main()
{
	int ch;
	messager A;
	do
	{
		cout<<"\n******** WELCOME TO MESSAGER **********";
		cout<<"\n0. Exit application";
		cout<<"\n1. Create new account";
		cout<<"\n2. Login to your account";
		cout<<"\n3. Delete an existing account";
		cout<<"\nEnter your choice: ";
		cin>> ch;
		cout<<"\n------------------------------------";

		switch (ch)
		{
			case 0: cout<<"\n********* PROGRAM ENDED **********";
					break;

			case 1: A.create();
					break;

			case 2: A.login();
					break;

			case 3: //A.remove(); //am
					break;

			default: cout<<"\nInvalid choice";
		}

	} while (ch != 0);
	return 0;
}
