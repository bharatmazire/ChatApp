# ChatApp
Chat Application using UNIX System V IPC message queue.

**Problem statement:**
Write a chat application using Unix system V IPC message queue.
The chat application should have following features:
 User should able to
1. Specify his name(chat alias) when your application runs
2. Chat with another user using user name. 
3. View his messages based your the name entered in step 1.
4. Should able to see messages received from all users.
5. Optional feature: Save chat history on disk in a file with user name and allow user to read it later through your program.


**Programming aspects:**
1. Message queue will help to keep sent messages in the system buffer, so that even if your application terminates, when you'll run your application later, you can see your unread(newly received) messages.

2. Code should have proper error handling. Especially while creating, using the message queue queue. 

3. You can read reply when you send a message or have a separate thread to monitor the queue for incoming messages(optional).

4. Single program should serve the purpose. No dedicated chat server is needed.
