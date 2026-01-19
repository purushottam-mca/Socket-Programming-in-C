/* Author - Purushottam Kumar (MCA-1st Sem Regular)
  
  Servers side following thing occurs :
	1. Open a socket
	2. Bind to a IP address and port
	3. Listen for incoming connections
	4. Accept connections
	5. Read / Send  
 Winsock OS needs winsock2.h , whereas Linux needs socket.h , apra/inet.h , unistd.h etc. */

#include<stdio.h>
#include<string.h>
#include<math.h>
#include <time.h>

#ifdef _WIN32
#include<io.h>
#include<winsock2.h> // The header file for winsock function
#pragma warning(disable : 4996) // To avoid warnings in Visual STudio
#pragma comment(lib,"ws2_32.lib") // The Library file for winsock function
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for close
#include <errno.h>
#include <stdlib.h> // for exit, atoi

typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#endif
#define MAX 200
#define PORT 5050

int isArmstrong(int num);
int isPalindrome(int num);
int isPrime(int num);
int FindFactorial(int num);
int IsLeapYear(int year);
void ReplyToClient(SOCKET S);

int main(int argc, char *argv[])
{
#ifdef _WIN32
	WSADATA wsa;
#endif
	SOCKET sockfd, new_socket;    // socket descriptor
	struct sockaddr_in server, client;
#ifdef _WIN32
	int c;
#else
	socklen_t c;
#endif

#ifdef _WIN32
	printf("\nSERVER>>> Initialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("SERVER>>> Failed. Error Code : %d \n", WSAGetLastError());
		return 1;
	}
	printf("\nSERVER>>> Winsock Successfully Initialised.\n");
#endif

 // Now Creating a socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)	
	{	// Address Family : IPv4 , type = SOCK_STREAM (TCP protocol) , protocol = 0
#ifdef _WIN32
		printf("Could not create socket : %d \n", WSAGetLastError());
#else
		perror("Could not create socket");
#endif
		return 1;
	}
	printf("\nSERVER>>> Socket Successfully Created.\n");

 // Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);		// Bind socket to port  on localhost

 // bind( ) can be used to bind a socket to a particular address and port.
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
#ifdef _WIN32
		printf("SERVER>>> Bind failed with error code : %d", WSAGetLastError());
#else
		perror("SERVER>>> Bind failed");
#endif
		exit(1);
	}
	puts("\nSERVER>>> Binding Successfully done\n");
 // Now Listen ( ) is required 
	if ((listen(sockfd, 5)) != 0)
	{
		printf("SERVER>>> Listen failed...\n");
		return 0;
	}
 // Accept incoming connection
	puts("SERVER>>> Waiting for incoming connections....");
	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(sockfd, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) 
	{							 // While loop will run 24x7 so server will always be ready
		puts("\nSERVER>>> [ + New Client Connected + ] ");
		ReplyToClient(new_socket); // Handle all communication with this client
		puts("\nSERVER>>> [ - Client Session Ended - ]");
	}
	if (new_socket == INVALID_SOCKET)
	{
#ifdef _WIN32
		printf("SERVER>>> Accept failed with error code : %d", WSAGetLastError());
#else
		perror("SERVER>>> Accept failed");
#endif
	}
	getchar();
	closesocket(sockfd);  // Close Socket
#ifdef _WIN32
	WSACleanup();	// Release Pending threads in Memory
#endif
	return 0;
}

void ReplyToClient(SOCKET S)
{
	int choice, num1, num2, result;
	char client_query[MAX],cur_time[128];
	time_t t;
	struct tm timeinfo;  
	float division;
	// Loop to handle multiple requests from the same client until they disconnect.
	while (1)
	{
		// The recv function returns 0 if the client disconnects gracefully,
		// and -1 (SOCKET_ERROR) on failure. We check for <= 0.
		if ((recv(S, client_query, sizeof(client_query), 0)) <= 0)
		{
			printf("\nSERVER>>> Client disconnected unexpectedly or recv failed.\n");
			return; // Exit function to end session.
		}

		if ((recv(S, client_query, sizeof(client_query), 0)) <= 0)
		{								// Read What option User has selected
			printf("\nSERVER>>> Client disconnected unexpectedly or recv failed.\n");
			return;
		}
		choice = atoi(client_query);		 // Store Option Value in integer Variable choice
		if (choice >= 1 && choice <= 4)		 // Two operands are required
		{
			recv(S, client_query, sizeof(client_query), 0); // Read First Number
			num1 = atoi(client_query);	   // Store in integer Variable num1
			recv(S, client_query, sizeof(client_query), 0); // Read Second Number
			num2 = atoi(client_query);	   // Store in integer Variable num2
		}
		else if (choice >= 5 && choice <= 9)  // Only one operand is required
		{
			recv(S, client_query, sizeof(client_query), 0); // Read One Number
			num1 = atoi(client_query);	   // Store in integer Variable num1
		}
		else if (choice == 10); // if Date & Time Then No input Required

		else if (choice == 11) // Client Disconnect
		{
			// The client has chosen to disconnect. We will exit this function.
			return;
		}
		else  // Other Wrong Data
		{
			printf("\nSERVER>>> Invalid data from client. Closing connection.\n");
			return;
		}
		// ----- Now Actual Processing Data -----
		switch (choice)
		{
		case 1:  // Addition
			printf("\nCLIENT>>> [Query] -> ADD %d %d\n", num1, num2);
			result = num1 + num2;
			printf("SERVER>>> [Query Solved] :: %d + %d = %d\n", num1, num2, result);
			sprintf(client_query, "%d", result);
			break;
		case 2: // Subtraction
			printf("\nCLIENT>>> [Query] -> SUBTRACT %d %d\n", num1, num2);
			result = num1 - num2;
			printf("SERVER>>> [Query Solved] :: %d - %d = %d\n", num1, num2, result);
			sprintf(client_query, "%d", result);
			break;
		case 3:	// Multiply
			printf("\nCLIENT>>> [Query] -> MULTIPLY %d %d\n", num1, num2);
			result = num1 * num2;
			printf("SERVER>>> [Query Solved] :: %d x %d = %d\n", num1, num2, result);
			sprintf(client_query, "%d", result);
			break;
		case 4: // Divide
			printf("\nCLIENT>>> [Query] -> DIVIDE %d %d\n", num1, num2);
			division = (float)num1 / num2;
			printf("SERVER>>> [Query Solved] :: %d / %d = %.2f\n", num1, num2, division);
			sprintf(client_query, "%.2f", division);
			break;
		case 5: // Check Prime
			printf("\nCLIENT>>> [Query] -> Check Prime (%d)\n", num1);
			if (isPrime(num1) == 1)
				strcpy(client_query, "Yes. It is a Prime Number");
			else
				strcpy(client_query, "No. It is not a Prime Number");
			printf("SERVER>>> [Query Solved] :: %s\n", client_query);
			break;
		case 6: // Check Armstrong
			printf("\nCLIENT>>> [Query] -> Check Armstrong (%d)\n", num1);
			if (isArmstrong(num1) == 1)
				strcpy(client_query, "Yes. It is an Armstrong Number");
			else
				strcpy(client_query, "No. It is not an Armstrong Number");
			printf("SERVER>>> [Query Solved] :: %s\n", client_query);
			break;
		case 7: // Check Plaindrome
			printf("\nCLIENT>>> [Query] -> Check Palindrome (%d)\n", num1);
			if (isPalindrome(num1) == 1)
				strcpy(client_query, "Yes. It is a Palindrome");
			else
				strcpy(client_query, "No. It is not a Palindrome");
			printf("SERVER>>> [Query Solved] :: %s\n", client_query);
			break;
		case 8: // Leap Year
			printf("\nCLIENT>>> [Query] -> Check Leap Year (%d)\n", num1);
			if (IsLeapYear(num1) == 1)
				strcpy(client_query, "Yes. It is a Leap Year");
			else
				strcpy(client_query, "No. It is not a Leap Year");
			printf("SERVER>>> [Query Solved] :: %s\n", client_query);
			break;
		case 9: // Factorial
			printf("\nCLIENT>>> [Query] -> Find Factorial (%d)\n", num1);
			num2 = FindFactorial(num1);
			printf("SERVER>>> [Query Solved] :: Factrial(%d) = %d\n", num1, num2);
			sprintf(client_query, "%d", num2);
			break;
		case 10: // Date & Time
			printf("\nCLIENT>>> [Query] -> Date & Time\n");
			t = time(NULL);
#ifdef _WIN32
			localtime_s(&timeinfo, &t);
#else
			localtime_r(&t, &timeinfo);
#endif
			strftime(cur_time, 128, "%d-%b-%Y %H:%M:%S", &timeinfo);
			strcpy(client_query, cur_time);
			printf("SERVER>>> [Query Solved] :: %s\n", client_query);
			break;
		}
		send(S, client_query, sizeof(client_query), 0); // Send Response To Client
	}
}

int isPrime(int num)  // Checking Prime
{
	int i;
	if (num < 2) return 0;
	for (i = 2; i <= sqrt(num); i++)
		if (num % i == 0) return 0;
	return 1;
}

int isPalindrome(int num) // Checking Palindrome
{
	int temp = num, sum = 0, rem;
	while (num > 0)
	{
		rem = num % 10;
		sum = sum * 10 + rem;
		num = num / 10;
	}
	if (sum == temp)
		return 1;
	else
		return 0;
}

int isArmstrong(int num) // Checking Armstrong
{
	int temp = num, sum = 0, rem;
	while (num > 0)
	{
		rem = num % 10;
		sum += (rem * rem * rem);
		num /= 10;
	}
	if (sum == temp)
		return 1;
	else
		return 0;
}

int IsLeapYear(int num1)
{
	if (num1 % 4 == 0)
	  {
		if (num1 % 100 == 0 && num1 % 400 != 0)
			return 0;
		else
			return 1;
	  }
	else
		return 0;
}

int FindFactorial(int num)
{
	if (num <=1)
		return 1;
	return (num * FindFactorial(num - 1));
}