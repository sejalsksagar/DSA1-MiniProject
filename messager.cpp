#include<iostream>
#include<iomanip>
#include <ctime>

using namespace std;

class msg
{
	public:
		bool star;
		bool sent;
		bool read;
		string dt; //date & time when msg was sent/received
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

		void msg_options(string title, msg *head);
		void display_msgs(string title, msg *head);
		void read_msg(msg *head);

};

void user::msg_options(string title, msg *head)
{
	int ch;
	do
	{
		display_msgs(title, head);
		if (head == NULL)
			return;
		cout << "\n********* " << title << " OPTIONS **********";
		cout << "\n0. Exit";
		cout << "\n1. Read a message";
		cout << "\n2. Delete a message";
		cout << "\n3. Star/Unstar a message";
		cout << "\nEnter your choice: ";
		cin >> ch;
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0: break;

			case 1: read_msg(head);
					break;

			case 2: //ak headR
					break;

			case 3: //ak
					break;
		}
	} while (ch != 0);
}

void user::display_msgs(string title, msg *head)
{
	string R[] = {"unread", "read"};
	string S[] = {"unstarred", "starred"};
	cout << "\n******************************* " << title << " *******************************";

	if (head == NULL)
		cout << "\nNo messages to display yet!\n";
	else
	{
		int i = 1;
		cout << "\n-------------------------------------------------------------------------------------------------";
		cout << "\n" << setw(5) << "No." << setw(15) << "From" << setw(15)
				<< "To" << setw(15) << "Message" << setw(14) << "When"
				<< setw(10) << "Read" << setw(14) << "Starred";
		cout << "\n-------------------------------------------------------------------------------------------------";

		msg *m = head;
		while (m != NULL)
		{
			cout << "\n" << setw(5) << i << setw(15) << m->from << setw(15) << m->to
					<< setw(15) << m->text.substr(0, 8) << "..." << setw(14) << m->dt.substr(4, 6)
					<< setw(10) << R[m->read] << setw(14) << S[m->star];
			cout << "\n-------------------------------------------------------------------------------------------------";
			m = m->link;
			i++;
		}
	}
}

void user::read_msg(msg *head)
{
	int no;
	if (head != NULL)
	{
		cout << "\n\nEnter message no. to read:";
		cin >> no;

		if (no < 1)
		{
			cout << "\nInvalid message no.";
			return;
		}
		msg *ptr = head;
		for (int i = 1; i < no; i++)
		{
			ptr = ptr->link;
			if (ptr == NULL)
			{
				cout << "\nInvalid message no.";
				return;
			}
		}
		cout << "\n..................................................................";
		cout << "\n************** MESSAGE " << no << " **************";
		cout << "\nFrom : " << ptr->from;
		cout << "\nTo : " << ptr->to;
		cout << "\nWhen : " << ptr->dt;
		cout << "\nMessage : \n" << ptr->text;
		cout << "\n...................................................................\n";
		ptr->read = true;
	}
}

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
		void create(); //creates new user account & adds it to user dll(sign-up)
		void login();  		//to login to an existing account
		void remove(); 		//to delete your account

		void activity(user *ptr);

		msg* msg_sent(); //takes input to send msg, updates receiver's inbox & returns pointer to sent msg
		void send_msg(user *ptr); //calls msg_sent() & updates user's sent msg sll

		void del_msg();
		void star_msg(msg *m);
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
	cout << "\nYour account has successfully created!";
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
void messager::remove()
{
	string userName;
	string passWord;
	cout << "\nEnter username: ";
	cin >> userName;
	cout << "\nEnter password: ";
	cin >> passWord;

	user *pre_node;
	user *current;

	if (start == NULL)
	{
		cout << "No users are present.\n";
		return;
	}

	// If the user is present at 1st position
	if (start->username == userName && start->password == passWord)
	{
		if (start->next != NULL)
		{
			start->next->prev = NULL;
			start = start->next;
			return;
		}
		else // only one user present
		{
			start = NULL;
			cout << "No users available now.\n";
			return;
		}
		cout << "User " << start->username << " deleted.\n";
	}
	else if (start->username
			!= userName&& start->password != passWord && start->next == NULL)
	{
		cout << "not found in the list\n" << userName << " " << passWord
				<< endl;
		return;
	}

	current = start;

	while (current->next != NULL && current->username != userName
			&& current->password != passWord)
	{ // traverse the list
		pre_node = current;
		current = current->next;
	}

	if (current->username == userName && current->password == passWord)
	{
		cout << "User " << current->username << " deleted.\n";
		pre_node->next = pre_node->next->next;

		if (pre_node->next != NULL)
		{          // link back
			pre_node->next->prev = pre_node;
		}
		free(current);
	}
	else
		cout << "Entered user not present.\n";
}

//takes input to send msg, updates receiver's inbox & returns pointer to sent msg
msg* messager::msg_sent()
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

				m->read = false;
				time_t now = time(0);	// current date/time based on current system
			    m->dt = ctime(&now);	// convert now to string form
				flag = 1;
				cout << "\nMessage sent successfully to @" << m->to;

				//insert new msg at beginning of ptrT's received msg sll
				m->link = ptrT->headR;
				ptrT->headR = m;
				return m;
			}
		}

		if (ptrT == NULL)
			cout << "\nEntered username doesn't exist.\n";

	} while (flag == 0);
	return m;
}

//calls msg_sent() & updates user's sent msg sll
void messager::send_msg(user *ptr)
{
	msg *ms = msg_sent(); //pointer to sent msg
	ms->from = ptr->username;

	//updating sender's (logged-in user's) sent msg sll
	msg *m = new msg();		//create new msg to update user's sent msgs sll
	m->sent = true;
	m->to = ms->to;
	m->from = ms->from;
	m->dt = ms->dt;
	m->text = ms->text;

	//insert sent msg at beginning of sent sll
	m->link = ptr->headS;
	ptr->headS = m;
}

void messager::activity(user *ptr)
{
	int ch;
	do
	{
		cout << "\n************* HELLO @" << ptr->username << " ! *************";
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
		cout << "\n--------------------------------------------\n";
		switch (ch)
		{
			case 0: ptr->logged_in = false;
					cout << "\nSuccessfully logged out.";
					return;

			case 1: ptr->msg_options("INBOX", ptr->headR);
					break;

			case 2: send_msg(ptr);
					break;

			case 3: ptr->msg_options("SENT", ptr->headS);
					break;

			case 4: //am
					break;

			case 5: //am
					break;

			case 6:
					break;

			default: cout << "\nInvalid choice";
		}

	} while (ch != 0);
}

int main()
{
	int ch;
	messager A;
	do
	{
		cout << "\n----------------------------------------";
		cout << "\n******** WELCOME TO MESSAGER **********";
		cout << "\n0. Exit application";
		cout << "\n1. Create new account";
		cout << "\n2. Login to your account";
		cout << "\n3. Delete an existing account";
		cout << "\nEnter your choice: ";
		cin >> ch;
		cout << "\n----------------------------------------";

		switch (ch)
		{
			case 0: cout << "\n********* PROGRAM ENDED **********";
					break;

			case 1: A.create();
					break;

			case 2: A.login();
					break;

			case 3: A.remove();
					break;

			default: cout << "\nInvalid choice";
		}

	} while (ch != 0);
	return 0;
}
