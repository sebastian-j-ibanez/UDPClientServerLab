#include <windows.networking.sockets.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void main()
{
	char ReceiveBuffer[128] = {};
	char SendBuffer[128] = { '"H', 'e', 'l', 'l', 'o', ' ', 'C', 'l', 'i', 'e', 'n', 't'};

	// Starts Winsock DLLs
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "ERROR: Failed to start WSA" << endl;
		return;
	}

	// Create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ServerSocket == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "ERROR: Failed to create ServerSocket" << endl;
		return;
	}

	// Create client socket address
	sockaddr_in CltAddr;
	CltAddr.sin_family = AF_INET;
	CltAddr.sin_addr.s_addr = INADDR_ANY;
	CltAddr.sin_port = htons(27500);

	//Create server socket address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	SvrAddr.sin_port = htons(27000);

	// Bind server socket to client socket address
	if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(ServerSocket);
		WSACleanup();
		cout << "ERROR: Failed to bind ServerSocket" << std::endl;
		return;
	}

	// Receive message from client socket address
	int addr_len = sizeof(CltAddr);
	recvfrom(ServerSocket, ReceiveBuffer, sizeof(ReceiveBuffer), 0, (struct sockaddr*)&CltAddr, &addr_len);
	cout << ReceiveBuffer << endl;

	// Send response to client socket address
	sendto(ServerSocket, SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr*)&CltAddr, sizeof(CltAddr));

	// Closes server socket
	closesocket(ServerSocket); 
	WSACleanup();
	return;
}