# DSA1-MiniProject : Messager

___
## Overview

We have made a very simple version of gmail using C++. We have used a doubly linked list to store user accounts and two singly linked list per user to store sent and received messages. In our program, an user can create an account, login and send a message to another existing user. Logged-in user can logout and create another account, login to another existing account, change password of an existing account or delete an existing account. User while logged-in in can view list of received, sent, deleted, starred messages and can also search for messages sent to/ received from an user. For every sent/ received message user can read it, delete it, mark it as important (star)/unstar. To display list of messages sent to/ received from an user or list of starred messages in the sent/ received singly linked list, we have used sequential search. Each time we find the message which satisfies the required condition it's reference gets stored in an vector. This vector helps us to read, delete or star/unstar a message in the list which we get as the result of sequential search. Refernces of deleted messages have been stored in a vector (trash). User can either permanently delete message from trash or simply view it. 

Limitations: 
  1. An user can send only a string (text) to another user at a time.
  2. An user can't login to multiple accounts at the same time. 
  3. Deleted messages in the trash cannot be retrieved.

Further scope: 
  1. The program can be extended to include more functionalities of gmail. 
  2. Limitations can be worked on.



___
 ### Team members:
- 2370 Ketaki Kothale
- 2372 Sejal Kshirsagar
- 2373 Akanksha Kulkarni
- 2369 Amruta Kotgire
