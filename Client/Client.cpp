#include <windows.networking.sockets.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void main()
{
	char ReceiveBuffer[128] = {};
	char SendBuffer[128] = { '"H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};

	//starts Winsock DLLs
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "ERROR: Failed to start WSA" << endl;
		return;
	}

	//Create client socket
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ClientSocket == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "ERROR: Failed to create ServerSocket" << endl;
		return;
	}

	//Create server socket address
	sockaddr_in CltAddr;
	CltAddr.sin_family = AF_INET;
	CltAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	CltAddr.sin_port = htons(27500);
	
	//Create server socket address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	SvrAddr.sin_port = htons(27000);

	if (bind(ClientSocket, (struct sockaddr*)&CltAddr, sizeof(CltAddr)) == SOCKET_ERROR)
	{
		closesocket(ClientSocket);
		WSACleanup();
		cout << "ERROR: Failed to bind ServerSocket" << endl;
		return;
	}

	sendto(ClientSocket, SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
	cout << "Message sent" << endl;

	int addr_len = sizeof(SvrAddr);
	recvfrom(ClientSocket, ReceiveBuffer, sizeof(ReceiveBuffer), 0, (struct sockaddr*)&SvrAddr, &addr_len);
	cout << "Message received: " << endl;
	cout << ReceiveBuffer << endl;

	closesocket(ClientSocket);
	WSACleanup();
	return;
}