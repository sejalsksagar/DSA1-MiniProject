#include<iostream>

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
			read = false;
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
		void display();
		void read_msg();
		msg* send_msg();
		void remove();
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
		cout << "Enter username to create : ";
		cin >> tmp->username;
		while (ptr1 != NULL)
		{
			if (ptr1->username == tmp->username)
			{
				cout << "Username not available. Please select a different one"
						<< endl;
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
	cout
			<< "\nYour account has successfully created. Please login to continue.";
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