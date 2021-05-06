#include<iostream>
#include<iomanip>

using namespace std;

class msg
{
	public:
		bool sent;
		bool read;
		string to;
		string from;
		string text;
		msg *link;

		msg()
		{
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
		msg *headS; //sent messages
		msg *headR; //received messages
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

		bool is_empty();
		user* accept(); //validation of password & username
		void create(); //sign-up
		void login();
		void activity(user *ptr);
		void display_inbox(user *ptr);
		void read_msg(user *ptr);
		msg* msg_sent();
		void send_msg(user *ptr);
		void sent_hist(user *ptr);
		void remove(); //
};

bool messager::is_empty()
{
	if (start == NULL)
		return true;
	else
		return false;
}

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

void messager :: display_inbox(user *ptr)
{
	cout<<"\n************* INBOX **************";

	if (ptr->headR == NULL)
		cout << "You haven't received any messages yet!\n";
	else
	{
		int i = 1;
		cout << "\n----------------------------------------------------------------";
		cout<<"\n"<<setw(5)<<"Sr no."<<setw(15)<<"From"<<setw(15)<<"Message"<<setw(10)<<"Status";
		cout << "\n----------------------------------------------------------------";

		msg *ptrR = ptr->headR;
		while (ptrR != NULL)
		{
			cout<<"\n"<<setw(5)<<i<<setw(15)<<ptrR->from<<setw(15)<<ptrR->text.substr(0, 10)<<setw(10)<<ptrR->read;
			cout << "\n----------------------------------------------------------------";
			ptrR = ptrR->link;
			i++;
		}
	}
}

void messager :: read_msg(user *ptr)
{
	int no;
	display_inbox(ptr);

	if (ptr->headR == NULL)
			cout << "You haven't received any messages yet!\n";
	else
	{
		cout<<"\n\nEnter message no. to read:";
		cin>>no;

		msg *mR = ptr->headR;
		for (int i=0; i<no; i++)
			mR = mR->link;

		cout << "\nFrom : " << mR->from;
		cout << "\nMessage : " << mR->text;
		cout << "\n-------------------------------\n";
		mR->read = true;
	}
}

msg* messager :: msg_sent()
{
	msg *m = new msg();
	user *ptrT; //pointer TO user to send message
	int flag = 0;
	do
	{
		cout << "Enter username of user to message : ";
		cin >> m->to;
		getchar(); //'\n'

		//updating receiver's received messages sll
		for (ptrT = start; ptrT != NULL; ptrT = ptrT->next)
		{
			if (ptrT->username == m->to)
			{
				cout << "\nEnter message you want to send to @" << m->to << " :\n";
				getline(cin, m->text);
				cout << "\nMessage sent successfully to @" << m->to;
				m->read = false;
				flag = 1;
				if (ptrT->headR == NULL)
					ptrT->headR = m;
				else
				{
					//pointer to ptrT's Received msgs sll
					msg *ptrTR = ptrT->headR;
					while (ptrTR->link != NULL)
						ptrTR = ptrTR->link;
					ptrTR->link = m;
				}
				return m;
			}
		}

		if (ptrT == NULL)
		{
			cout << "\nEntered username doesn't exist.";
		}
	} while (flag == 0);
	return m;
}

void messager :: send_msg(user *ptr)
{
	msg *ms = msg_sent();
	ms->from = ptr->username;

	//updating sender's sent messages sll
	msg *m = new msg();
	m->sent = true;
	m->to = ms->to;
	m->from = ms->from;
	m->text = ms->text;

	if (ptr->headS == NULL)
		ptr->headS = m;
	else
	{
		msg *ptrS = ptr->headS; //pointer to user's Sent sll
		while (ptrS->link != NULL)
			ptrS = ptrS->link;
		ptrS->link = m;
	}

}

void messager :: sent_hist(user *ptr)
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
		cout << "\n3. Show my sent history";
		cout << "\n4. Search messages sent by an user";
		cout << "\nEnter your choice: ";
		cin >> ch;

		switch (ch)
		{
			case 0: ptr->logged_in = false;
					cout << "\nSuccessfully logged out.";
					return;

			case 1: read_msg(ptr);
					break;

			case 2: send_msg(ptr);
					break;

			case 3: sent_hist(ptr);
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
		//cout<<"\n3. Delete an existing account";
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

			default: cout<<"\nInvalid choice";
		}

	} while (ch != 0);
	return 0;
}
