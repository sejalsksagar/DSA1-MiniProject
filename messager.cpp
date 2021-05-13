#include<iostream>
#include<iomanip>
#include <ctime>
#include<vector>
#include <limits>

using namespace std;

//to deal with bad input
int input_num(string prompt)
{
	int ch;
	cout << prompt;
	cin >> ch;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "\nInvalid input. Try again. \n" << prompt;
		cin >> ch;
	}
	return ch;
}

//node for sent & received msg SLLs
class msg
{
	public:
		bool star;			//true if msg is starred
		bool sent;			//true if msg has been sent to whom the user wishes to
		bool read;			//true if msg has been read by the logged-in user
		string dt; 			//date & time when msg was sent/received
		string to;			//username of user to whom msg is sent
		string from;		//username of user from whom msg is sent
		string text;		//the actual message
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

//node for user DLL
class user
{
	public:
		bool logged_in;
		string username;
		string password;
		msg *headS; 			//sent msg SLL head
		msg *headR; 			//received msg SLL head
		vector<msg*> trash;		//vector of deleted msg
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

		void display_msgs(string title, msg *head);				//to display list of sent/inbox msg
		void msg_options(string title, msg **head);				//actions user can perform with displayed list of msg
		void read_msg(msg *head);								//to read a certain msg
		void del_msg(msg **head);								//to delete a certain msg
		void starUnstar_msg(msg *m);							//to mark an msg as important (star) or unstar
		void vec_read_msg(vector<msg*> results);				//to read msg from search results
		void vec_del_msg(vector<msg*> results, msg **head); 	//to delete msg from search results
		void vec_starUnstar(vector<msg*> results); 				//to star/unstar msg from search results
		void search_msg(string title, msg **head);				//to search msg sent to/ received from a user
		void starred_msg(string title, msg **head);				//displays list of starred msg
		void trash_options();									//actions to perform on deleted msg
		void del_permanently();									//to delete a msg from trash (permanently)
		void read_trashMsg();									//to read a msg in trash

};

//to display list of sent/inbox msg
void user::display_msgs(string title, msg *head)
{
	string R[] = { "unread", "read" };
	string S[] = { "unstarred", "starred" };
	cout <<"\n******************************* " <<title<< " *******************************";

	if (head == NULL)
		cout << "\nNo messages to display yet!\n";

	else
	{
		int i = 1;
		cout << "\n-------------------------------------------------------------------------------------------------";
		cout << "\n" << setw(5) << "No." << setw(15) << "From" << setw(15)
				<< "To" << setw(15) << "Message" << setw(14) << "When"
				<< setw(10) << "Status" << setw(14) << "Starred";
		cout << "\n-------------------------------------------------------------------------------------------------";

		msg *m = head;
		while (m != NULL)
		{
			cout << "\n" << setw(5) << i << setw(15) << m->from << setw(15)
					<< m->to << setw(15) << m->text.substr(0, 8) << "..."
					<< setw(14) << m->dt.substr(4, 6) << setw(10) << R[m->read]
					<< setw(14) << S[m->star];
			cout << "\n-------------------------------------------------------------------------------------------------";
			m = m->link;
			i++;
		}
	}
}

//actions user can perform with displayed list of msg
void user::msg_options(string title, msg **head)
{
	int ch;
	do
	{
		display_msgs(title, *head);
		if (*head == NULL)
			return;
		cout << "\n********* " << title << " OPTIONS **********";
		cout << "\n0. Exit";
		cout << "\n1. Read a message";
		cout << "\n2. Delete a message";
		cout << "\n3. Star/Unstar a message";
		ch = input_num("\nEnter your choice: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				read_msg(*head);
				break;

			case 2:
				del_msg(head);
				break;

			case 3:
				starUnstar_msg(*head);
				break;
		}
	} while (ch != 0);
}

//to read a certain msg
void user::read_msg(msg *head)
{
	int no;
	no = input_num("\nEnter message no. to read: ");
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

//to delete a certain msg //it adds the deleted msg to trash
void user::del_msg(msg **head)
{
	if (*head == NULL)
	{
		cout << "No messages found.\n";
		return;
	}

	int no = input_num("\nEnter message no. to delete: ");

	if (no < 1)
	{
		cout << "\nInvalid message no.";
		return;
	}

	msg *ptr = *head;
	msg *prev = *head;
	if (no == 1)
	{
		*head = (*head)->link;
		cout << "Message deleted successfully!!\n";
		trash.push_back(ptr);
		return;
	}

	for (int i = 1; i < no; i++)
	{
		prev = ptr;
		ptr = ptr->link;
		if (ptr == NULL)
		{
			cout << "Invalid message no.\n";
			return;
		}
	}
	prev->link = ptr->link;
	trash.push_back(ptr);
	cout << "Message deleted successfully!!\n";
}

//to mark an msg as important (star) or unstar
void user::starUnstar_msg(msg *head)
{
	int no = input_num("\nEnter message no. to star/unstar: ");

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
	if (ptr->star == false)
	{
		ptr->star = true;
		cout << "Message no. " << no << " has been starred.\n";
	}
	else
	{
		ptr->star = false;
		cout << "Message no. " << no << " has been unstarred.\n";
	}
}

//to read msg from search results or from starred msg list
void user::vec_read_msg(vector<msg*> results)
{
	unsigned int no = unsigned(input_num("\nEnter message no. to read: "));

	if (no < 1 || no > results.size())
	{
		cout << "\nInvalid message no.";
		return;
	}

	msg *ptr = results.at(no - 1);
	cout << "\n..................................................................";
	cout << "\n************** MESSAGE " << no << " **************";
	cout << "\nFrom : " << ptr->from;
	cout << "\nTo : " << ptr->to;
	cout << "\nWhen : " << ptr->dt;
	cout << "\nMessage : \n" << ptr->text;
	cout << "\n...................................................................\n";
	ptr->read = true;
}

//to delete msg from search results or from starred msg list
void user::vec_del_msg(vector<msg*> results, msg **head)
{
	unsigned int no = unsigned(input_num("\nEnter message no. to delete: "));

	if (no < 1 || no > results.size())
	{
		cout << "\nInvalid message no.";
		return;
	}

	msg *ptr = *head;
	msg *prev = *head;

	if (results.at(no - 1) == *head)
	{
		*head = (*head)->link;
		cout << "\nMessage deleted successfully!";
		trash.push_back(ptr);
		results.erase(results.begin() + no - 1);
		return;
	}
	for (ptr = *head; ptr != results.at(no - 1);)
	{
		prev = ptr;
		ptr = ptr->link;
		if (ptr == NULL)
		{
			cout << "Invalid message no.\n";
			return;
		}
	}
	prev->link = ptr->link;
	ptr = results.at(no - 1);
	trash.push_back(ptr);
	results.erase(results.begin() + no - 1);
	cout << "Message deleted successfully!!\n";
}

//to star/unstar msg from search results
void user::vec_starUnstar(vector<msg*> results)
{
	unsigned int no = unsigned(input_num("\nEnter message no. to star/unstar: "));

	if (no < 1 || no > results.size())
	{
		cout << "\nInvalid message no.";
		return;
	}

	msg *ptr = results.at(no - 1);
	if (ptr->star)
	{
		ptr->star = false;
		cout << "Message no. " << no << " has been unstarred.\n";
	}
	else
	{
		ptr->star = true;
		cout << "Message no. " << no << " has been starred.\n";
	}
}

//to search msg sent to/ received from a user
void user::search_msg(string title, msg **head)
{
	string un;
	cout << "\nEnter the username: ";
	cin >> un;

	bool found = false;
	msg *m = *head;
	if (*head == NULL)
	{
		cout << "\nNo messages to display yet!";
		return;
	}

	string cmp;

	string R[] =  { "unread", "read" };
	string S[] =  { "unstarred", "starred" };

	int ch, i;
	do
	{
		i = 0;
		found = false;
		vector<msg*> results;

		for (m = *head; m != NULL; m = m->link)
		{
			if (title == "SENT TO ")
				cmp = m->to;
			else //"RECEIVED FROM "
				cmp = m->from;

			if (cmp == un)
			{
				if (!found)
				{
					cout<<"\n**************************** MESSAGES " <<title<< un << " ****************************";
					cout<< "\n-------------------------------------------------------------------------------------------------";
					cout<< "\n" << setw(5) << "No." << setw(15) << "From"
							<< setw(15) << "To" << setw(15) << "Message"
							<< setw(14) << "When" << setw(10) << "Status"
							<< setw(14) << "Starred";
					cout<< "\n-------------------------------------------------------------------------------------------------";

				}
				i++;
				found = true;
				results.push_back(m);

				cout << "\n" << setw(5) << i << setw(15) << m->from << setw(15)
						<< m->to << setw(15) << m->text.substr(0, 8) << "..."
						<< setw(14) << m->dt.substr(4, 6) << setw(10)
						<< R[m->read] << setw(14) << S[m->star];
				cout << "\n-------------------------------------------------------------------------------------------------";
			}
		}

		if (m == NULL && !found)
		{
			cout << "\nNo messages found!\n";
			return;
		}

		cout << "\n********* MESSAGE OPTIONS **********";
		cout << "\n0. Exit";
		cout << "\n1. Read a message";
		cout << "\n2. Delete a message";
		cout << "\n3. Star/Unstar a message";
		ch = input_num("\nEnter your choice: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				vec_read_msg(results);
				break;

			case 2:
				vec_del_msg(results, head);
				break;

			case 3:
				vec_starUnstar(results);
				break;
		}
	} while (ch != 0);
}

//displays list of starred msg
void user::starred_msg(string title, msg **head)
{

	string R[] = { "unread", "read" };
	string S[] = { "unstarred", "starred" };

	msg *m = *head;
	if (head == NULL)
	{
		cout << "\nNo messages to display yet!";
		return;
	}

	int ch, i;
	bool found;
	vector<msg*> results;
	do
	{
		i = 0;
		found = false;
		vector<msg*> results;

		for (m = *head; m != NULL; m = m->link)
		{

			if (m->star == true)
			{

				if (!found)
				{
					cout << "\n**************************** STARRED MESSAGES IN "<< title << " ****************************";
					cout << "\n-------------------------------------------------------------------------------------------------";
					cout << "\n" << setw(5) << "No." << setw(15) << "From"
							<< setw(15) << "To" << setw(15) << "Message"
							<< setw(14) << "When" << setw(10) << "Status"
							<< setw(14) << "Starred";
					cout << "\n-------------------------------------------------------------------------------------------------";
				}
				i++;
				found = true;
				results.push_back(m);

				cout << "\n" << setw(5) << i << setw(15) << m->from << setw(15)
						<< m->to << setw(15) << m->text.substr(0, 8) << "..."
						<< setw(14) << m->dt.substr(4, 6) << setw(10)
						<< R[m->read] << setw(14) << S[m->star];
				cout << "\n-------------------------------------------------------------------------------------------------";
			}
		}

		if (m == NULL && !found)
		{
			cout << "\nNo messages found!\n";
			return;
		}

		cout << "\n********* MESSAGE OPTIONS **********";
		cout << "\n0. Exit";
		cout << "\n1. Read a message";
		cout << "\n2. Delete a message";
		cout << "\n3. Star/Unstar a message";
		ch = input_num("\nEnter your choice: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				vec_read_msg(results);
				break;

			case 2:
				vec_del_msg(results, head);
				break;

			case 3:
				vec_starUnstar(results);
				break;
		}
	} while (ch != 0);

}

//actions to perform on deleted msg
void user::trash_options()
{
	int ch;
	do
	{
		string R[] = { "unread", "read" };
		string S[] = { "unstarred", "starred" };
		if (this->trash.size() == 0)
		{
			cout << "Trash empty\n";
			return;
		}
		cout<< "\n******************************* TRASH *******************************";
		cout<< "\n-------------------------------------------------------------------------------------------------";
		cout << "\n" << setw(5) << "No." << setw(15) << "From" << setw(15) << "To"
				<< setw(15) << "Message" << setw(14) << "When" << setw(10)
				<< "Status" << setw(14) << "Starred";
		cout<< "\n-------------------------------------------------------------------------------------------------";

		for (unsigned int i = 0; i < this->trash.size(); i++)
		{
			msg *m = this->trash[i];
			cout << "\n" << setw(5) << i+1 << setw(15) << m->from << setw(15)
								<< m->to << setw(15) << m->text.substr(0, 8) << "..."
								<< setw(14) << m->dt.substr(4, 6) << setw(10) << R[m->read]<< setw(14) << S[m->star];
			cout << "\n-------------------------------------------------------------------------------------------------";
		}

		cout << "\n********* TRASH OPTIONS **********";
		cout << "\n0. Exit";
		cout << "\n1. Delete a message permanently";
		cout << "\n2. View a message";
		ch = input_num("\nEnter your choice: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				del_permanently();
				break;

			case 2:
				read_trashMsg();
				break;
		}
	} while (ch != 0);
}

//to delete a msg from trash (permanently)
void user::del_permanently()
{
	unsigned int no = unsigned(input_num("\nEnter message no. to delete: "));
	if (no > trash.size() || no < 1)
	{
		cout << "Invalid message no.\n";
		return;
	}

	msg *m = trash[no - 1];
	trash.erase(trash.begin() + no - 1);
	cout << "Message permanently deleted\n";
	delete m;
}

//to read a msg in trash
void user::read_trashMsg()
{
	unsigned int no = unsigned(input_num("\nEnter message no. to read: "));

	if (no < 1 || no > trash.size())
	{
		cout << "\nInvalid message no.";
		return;
	}

	cout << "\n..................................................................";
	cout << "\n************** MESSAGE " << no << " **************";
	cout << "\nFrom : " << trash[no - 1]->from;
	cout << "\nTo : " << trash[no - 1]->to;
	cout << "\nWhen : " << trash[no - 1]->dt;
	cout << "\nMessage : \n" << trash[no - 1]->text;
	cout << "\n...................................................................\n";
	trash[no - 1]->read = true;
}

class messager
{
		user *start;		//start pointer of user DLL
		user *last;			//pointer to last node of user DLL
	public:
		messager()
		{
			start = NULL;
			last = NULL;
		}

		bool is_empty();			//returns true if no user has created account yet
		user* accept();				//takes input required while creating a new account
		void create(); 				//creates new user account & adds it to user DLL(sign-up)
		void login();  				//to login to an existing account
		void remove(); 				//to delete your account
		void change_pw();			//to change current password
		void activity(user *ptr); 		//actions that user can perform while logged in
		msg* msg_sent(); 				//takes input to send msg, updates receiver's inbox & returns pointer to sent msg
		void send_msg(user *ptr); 		//calls msg_sent() & updates user's sent msg sll
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
	bool un_exist = false;
	user *tmp = new user();
	user *ptr;
	do
	{
		ptr = start;
		cout << "\nEnter username to create : ";
		cin >> tmp->username;
		while (ptr != NULL)
		{
			if (ptr->username == tmp->username)
			{
				cout << "\nEntered username already exists.";
				un_exist = true;
				break;
			}
			ptr = ptr->next;
		}
		if (ptr == NULL)
			un_exist = false;
	} while (un_exist);
	cout << "\nCreate password: ";
	cin >> tmp->password;
	return tmp;
}

//creates new user account & adds it to user dll(sign-up)
void messager::create()
{
	user *tmp = accept();
	if (is_empty())
	{
		start = tmp;
		last = tmp;
	}
	else
	{
		last->next = tmp;
		tmp->prev = last;
		last = tmp;
	}
	cout << "\nYour account has been created successfully!";
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
	cout << "\nUsername not found.";
}

//to delete your account //removes ptr from user dll
void messager::remove()
{
	char ch;
	string un, pw;

	cout << "\nEnter username: ";
	cin >> un;

	if (is_empty())
		cout << "\nCurrently no user accounts exist.";

	for (user *curr = start; curr != NULL; curr = curr->next)
	{
		if (curr->username == un)
		{
			cout << "\nEnter the password: ";
			cin >> pw;

			if (curr->password == pw)
			{
				do
				{
					cout << "Are you sure you want to delete your account?(Y/N): ";
					cin >> ch;
					if (ch == 'N') return;
					else if (ch == 'Y') break;
					else cout << "\nInvalid choice. Try again.";
				} while (ch != 'Y' || ch != 'N');

				//if start node
				if (curr == start)
					start = curr->next;

				//if curr is not last node
				if (curr->next != NULL)
					(curr->next)->prev = curr->prev;

				//if curr is not 1st node
				if (curr->prev != NULL)
					(curr->prev)->next = curr->next;

				delete curr;
				cout << "\nYour account has been deleted successfully!";
				return;
			}
			else
			{
				cout << "\nIncorrect password....please try again.";
				return;
			}
		}
	}
	cout << "\nUsername not found.";
}

//to change current password
void messager::change_pw()
{
	string un, pw, pw1;
	cout << "\nEnter username: ";
	cin >> un;

	for (user *ptr = start; ptr != NULL; ptr = ptr->next)
	{
		if (ptr->username == un)
		{
			do
			{
				cout << "\nEnter previous password: ";
				cin >> pw;
				if (ptr->password == pw)
				{
					cout << "\nEnter new password : ";
					cin >> pw1;
					ptr->password = pw1;

					cout << "\nYour password has been changed successfully!";
					return;
				}
				else
					cout << "\nIncorrect previous password.\n";
			} while (true);
		}
	}
	cout << "\nUsername not found.";
}

//actions that user can perform while logged in
void messager::activity(user *ptr)
{
	int ch;
	do
	{
		cout << "\n************* HELLO @"<< ptr->username<< " ! *************";
		cout << "\n0. Logout";
		cout << "\n1. Check inbox messages";
		cout << "\n2. Send a message";
		cout << "\n3. View sent messages";
		cout << "\n4. Search messages sent to an user";
		cout << "\n5. Search messages received from an user";
		cout << "\n6. View deleted messages";
		cout << "\n7. View starred messages in Inbox";
		cout << "\n8. View starred messages in Sentbox";
		ch = input_num("\nEnter your choice: ");
		cout << "\n------------------------------------------\n";

		switch (ch)
		{
			case 0:
				ptr->logged_in = false;
				cout << "\nSuccessfully logged out.";
				return;

			case 1:
				ptr->msg_options("INBOX", &ptr->headR);
				break;

			case 2:
				send_msg(ptr);
				break;

			case 3:
				ptr->msg_options("SENT", &ptr->headS);
				break;

			case 4:
				ptr->search_msg("SENT TO ", &ptr->headS);
				break;

			case 5:
				ptr->search_msg("RECEIVED FROM ", &ptr->headR);
				break;

			case 6:
				ptr->trash_options();
				break;

			case 7:
				ptr->starred_msg("INBOX ", &ptr->headR);
				break;

			case 8:
				ptr->starred_msg("SENTBOX ", &ptr->headS);
				break;

			default:
				cout << "\nInvalid choice";
		}

	} while (ch != 0);
}

//takes input to send msg, updates receiver's inbox & returns pointer to sent msg
msg* messager::msg_sent()
{
	msg *m = new msg();
	user *ptrT; 				//pointer To whom user is sending msg
	bool un_exist = false;
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
				cout << "\nEnter message you want to send to @" <<m->to<< " :\n";
				getline(cin, m->text);

				m->read = false;
				time_t now = time(0); 		// current date/time based on current system
				m->dt = ctime(&now);		// convert now to string form
				un_exist = true;
				cout << "\nMessage sent successfully to @" << m->to;

				//insert new msg at beginning of ptrT's received msg sll
				m->link = ptrT->headR;
				ptrT->headR = m;
				return m;
			}
		}

		if (ptrT == NULL) //!un_exist
			cout << "\nEntered username doesn't exist.\n";

	} while (un_exist == false);
	return m;
}

//calls msg_sent() & updates user's sent msg sll
void messager::send_msg(user *ptr)
{
	msg *ms = msg_sent(); 		//pointer to sent msg
	ms->from = ptr->username;

	//updating sender's (logged-in user's) sent msg sll
	msg *m = new msg();			//create new msg to update user's sent msgs sll
	m->sent = true;
	m->to = ms->to;
	m->from = ms->from;
	m->dt = ms->dt;
	m->text = ms->text;

	//insert sent msg at beginning of sent sll
	m->link = ptr->headS;
	ptr->headS = m;
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
		cout << "\n4. Change Password";
		ch = input_num("\nEnter your choice: ");
		cout << "\n----------------------------------------";

		switch (ch)
		{
			case 0:
				cout << "\n********* PROGRAM ENDED **********";
				break;

			case 1:
				A.create();
				break;

			case 2:
				A.login();
				break;

			case 3:
				A.remove();
				break;

			case 4:
				A.change_pw();
				break;
			default:
				cout << "\nInvalid choice";
		}

	} while (ch != 0);
	return 0;
}
