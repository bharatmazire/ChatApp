/*

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


*/


// including headers
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/msg.h>
 
// defining macros
#define MAX_TEXT 512
#define BUF_SIZE 512
 
// defining msg struct
struct MsgBody
{
	char name[256];
	char body[256];
};

struct Msg
{
        long int msg_type;
        struct MsgBody msg_body;
};
 
// main
int main()
{
        // declaring and initializing variables
        char alias_name[20],chat_name[20];
        char buffer[BUF_SIZE];
        long a_name,c_name;
        int a = 1;
        int num = 0;
        int alias_msgid, chat_msgid;
        int choice = 1;
        FILE *fptrA, *fptrC;
        
		struct Msg msg;
		
		// code
        printf("\tWelcome! \n");
        printf("\t\tEnter Your alias name : ");
        scanf("%s",&alias_name);
//        printf("Converting %s to long ...\n",alias_name);
 		for(int i = 0 ; alias_name[i] != '\0' ; i++)
		{	
			long n = alias_name[i] - 32;
			num = num + (n * a);
			a = a * 100;
		}
		a_name = num;
		
        while(choice != 4)
        {
                printf("\t\t\t1. Chat \n\t\t\t2. View your message \n\t\t\t3. View chat history \n\t\t\t4. EXIT \n\t\t\tYOUR CHOICE HERE : ");
                scanf("%d",&choice);
                switch(choice)
                {
				// chat 
                case 1: printf("Enter chat person name : ");
                        scanf("%s",&chat_name);
                        num = 0;
                        a = 1;
//                        printf("Converting %s to long ...\n",chat_name);
                        for(int i = 0 ; chat_name[i] != '\0' ; i++)
						{	
							long n = chat_name[i] - 32;
							num = num + (n * a);
							a = a * 100;
						}
						c_name = num;
                        
                        chat_msgid = msgget((key_t)c_name, 0666 | IPC_CREAT);
                        if(chat_msgid == -1)
                        {
                                printf("Error while creating msgid for : %s",chat_name);
                                exit(EXIT_FAILURE);
                        }
//                        printf("msgid for %s is : %d\n",chat_name,chat_msgid);
                        int type = 1;
                        
                        fptrC = fopen(chat_name,"a");
                        if(fptrC == NULL)
                        {
                        	printf("Error while creating chat file ");
                        	exit(EXIT_FAILURE);
                        }
                        fptrA = fopen(alias_name,"a");
                        if(fptrA == NULL)
                        {
                        	printf("Error while creating alias file");
                        	exit(EXIT_FAILURE);
                        }
                        
                        fprintf(fptrA,"____________________________\n");
           				fprintf(fptrA,"Message to : %s\n",chat_name);
           				fprintf(fptrA,"____________________________\n");
           				fprintf(fptrC,"____________________________\n");
           				fprintf(fptrC,"Message from : %s\n",alias_name);
           				fprintf(fptrC,"____________________________\n");
                        printf("Type your text :\n");
                        
                        while(type)
                        {
                                printf("%s : ",alias_name);
                                //fgets(buffer,BUF_SIZE,stdin);
                                scanf("%s",&buffer);
                                msg.msg_type = 1;
                                strcpy(msg.msg_body.name,alias_name);
                                strcpy(msg.msg_body.body,buffer);
                                if(msgsnd(chat_msgid,(void *)&msg,MAX_TEXT,0) == -1)
                                {
                                        printf("Message Send Failed\n");
                                        exit(EXIT_FAILURE);
                                }
                                fprintf(fptrA,"\t%s\n",buffer);
                                fprintf(fptrC,"\t%s\n",buffer);
                                if(strncmp(buffer,"bye",3) == 0)
                                {
                                        type = 0;
                                }
                        }
                        fclose(fptrA);
                        fclose(fptrC);
                        break;
                        
                case 2: printf("Viewing Message :  \n");
                        alias_msgid = msgget((key_t)a_name, 0666 | IPC_CREAT);
                        if(alias_msgid == -1)
                        {
                                printf("Error while creating msgid : %s",alias_name);
                                exit(EXIT_FAILURE);
                        }
//                        printf("msgid for %s is %d\n",alias_name,alias_msgid);
//                        printf("alias msgid successfully created!!\n");
                        int rcv = 1;
                        while(rcv)
                        {
//                        		printf("in msg rcv\n");
                                if(msgrcv(alias_msgid,(void *)&msg,BUF_SIZE,0,IPC_NOWAIT) == -1)
                                {
                                	printf("You have No Message \n");
                                	break;
                                }
                                printf("%s say's : %s\n",msg.msg_body.name,msg.msg_body.body);
                                if(strncmp(msg.msg_body.body,"bye",3) == 0)
                                {
                                        rcv = 0;
                                }
                        }
                        break;
                        
                case 3: fptrA = fopen(alias_name,"r");
                        if(fptrA == NULL)
                        {
                        	printf("History not present for %s File\n",alias_name);
                        	//exit(EXIT_FAILURE);
                        	break;
                        }
                        while(1)
                        {
                        	char c = fgetc(fptrA);
                        	if(feof(fptrA))				// check for end of fike
                        		break;
                        	printf("%c",c);
                        }
                        fclose(fptrA);
                        break;
                        
                case 4: printf("Exiting\n");
                        break;
                        
                default:printf("Please Enter Valid Choice \n");
                        break;
                }
        }
        return(0);
}


