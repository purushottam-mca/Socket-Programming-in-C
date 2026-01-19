/* Author - Purushottam Kumar (MCA-1st Sem Regular)

	Normally Client side following thing occurs :
	1. Create a socket
	2. Connect to remote server
	3. Send some data
	4. Receive a reply

Initialise Winsock.h Because it is running on windows OS.  */

#include <stdio.h>

#ifdef _WIN32
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib") // The library file to use winsock functions.
#pragma warning(disable : 4996)   // To avoid warnings in Visual STudio
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for close
#include <errno.h>

// Define types and constants for Linux to match Windows names
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#endif
#define MAX 200
#define PORT 5050

int option();
void SendToServer(SOCKET S);

int main(int argc, char *argv[])
{
#ifdef _WIN32
	WSADATA wsa;
#endif
	SOCKET sockfd;    // socket descriptor value
	struct sockaddr_in server;  // sin_family = AF_INET or AF_INET6

#ifdef _WIN32
	printf("\nCLIENT>>> Initialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d \n", WSAGetLastError());
		return 1;
	}
	printf("\nCLIENT>>> WInsock Succesfully Initialised.\n");
#else
	// No initialization is needed on Linux
#endif

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{	// Address Family : (IPv4) and SOCK_STREAM (means TCP protocol), protocol = 0
		printf("Could not create socket : %d \n", WSAGetLastError());
		return 1;
	}
	printf("\nCLIENT>>> Socket Successfully Created.\n");

    // Connect to Server  ( IP address and port number )
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Ip address of localhost
	server.sin_family = AF_INET;  // IPv4
	server.sin_port = htons(PORT);  // Port No 8080
	if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("\nCLIENT>>> Server is not available or connection failed.\n");
#ifdef _WIN32
		printf("Error code: %d\n", WSAGetLastError());
#else
		perror("Connect error");
#endif
		return 1;
	}
	puts("\nCLIENT>>> Successfully Connected to Server.");
	SendToServer(sockfd); // Requesting Services from Server
	return 0;
}

int option()    // Services For Client..
{
	int choice;
	printf("\n |`````````````````````````````````|");
	printf("\n |   Press <1>  Addition           |");
	printf("\n |   Press <2>  Subtraction        |");
	printf("\n |   Press <3>  Multiplication     |");
	printf("\n |   Press <4>  Division           |");
	printf("\n |   Press <5>  Check Prime        |");
	printf("\n |   Press <6>  Check Armstrong    |");
	printf("\n |   Press <7>  Check Palindrome   |");
	printf("\n |   Press <8>  Check Leap Year    |");
	printf("\n |   Press <9>  Find Factorial     |");
	printf("\n |   Press <10> Check Date & Time  |");
	printf("\n |   Press <11> Disconnect         |");
	printf("\n |_________________________________|\n");
	printf("\nClient>>> Enter your choice : ");
	scanf("%d", &choice);
	return choice;
}

void SendToServer(SOCKET S)
{
	int choice;  // To Send and recieve data
	do{
		char msg[MAX];
		send(S, "50", 3, 0); // Just Confirmation only
		choice = option();   //display the menu content
		switch (choice)
		{
		case 1:  // Addition
			send(S, "001", 4, 0);
			printf("\nClient>>> Enter 1st number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			printf("\nClient>>> Enter 2nd number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			break;
		case 2: // Subtraction
			send(S, "002", 4, 0);
			printf("\nClient>>> Enter the 1st number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			printf("\nClient>>> Enter the 2nd number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			break;
		case 3:	// Multiply
			send(S, "003", 4, 0);
			printf("\nClient>>> Enter the 1st number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			printf("\nClient>>> Enter the 2nd number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			break;
		case 4: // Divide
			send(S, "004", 4, 0);
			printf("\nClient>>> Enter the 1st number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			printf("\nClient>>> Enter the 2nd number : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			break;
		case 5: // Check Plaindrome
			send(S, "005", 4, 0);
			printf("\nClient>>> Enter the Integer : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			break;
		case 6: // Check Armstrong
			send(S, "006", 4, 0);
			printf("\nClient>>> Enter the Integer : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
		break;
		case 7: // Check Prime
			send(S, "007", 4, 0);
			printf("\nClient>>> Enter the Integer : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
		break;
		case 8: // Check Leap Year
			send(S, "008", 4, 0);
			printf("\nClient>>> Enter the Year : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			break;
		case 9: // Find factorial
			send(S, "009", 4, 0);
			printf("\nClient>>> Enter the Integer : ");
			scanf(" %s", msg);
			send(S, msg, sizeof(msg), 0);
			break;
		case 10: // Find Time & Date
			send(S, "010", 4, 0);
			printf("\nClient>>> [ Request for Date & Time ]");
			break;
		case 11: // Exit program
			send(S, "011", 4, 0);
			closesocket(S);
#ifdef _WIN32
			WSACleanup();
#endif
			printf("\nClient>>> Disconnected\n\n");
			return;
			break;
		default:
			printf("Client>>> Invalid Choice\n");
		}
		recv(S, msg, sizeof(msg), 0);	 // Recieving Output
		printf("\nSERVER>>> %s\n",msg);  // Displying Output
		fflush(stdin);					 // Releases memory
	} while (choice != 11);
}
