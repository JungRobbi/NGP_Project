#include <iostream>
#include <string>
#include <queue>

#include "../../Common.h"
#include "../../../Server/GameData.h"

#include "MSGFunc.h"
#include "PlayerInfoLobbyFunc.h"

char* SERVERIP;
std::string m_Name;

#define SERVERPORT 9000
#define BUFSIZE    50

DWORD WINAPI RecvMSGThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char data[52];
	GAMEMSG recv_msg;
	while (true) {
		// �޼��� �ޱ�
		recv_msg = recvMSG(sock);
		printf("���� �޼��� : %d\n", recv_msg);
		if (recv_msg == NULL) {
			std::cout << "recv_msg - NULL" << std::endl;
			break;
		}

		GameData* RecvData;

		switch (recv_msg) // �޼��� �ؼ�
		{
		case MSG_PLAYER_INFO_LOBBY:  // ������ �ޱ�
			RecvData = new PlayerInfoLobby{ recvPlayerInfoLobby(sock) };
			break;
		case MSG_PLAYER_INFO_SCENE:
			break;
		case MSG_CHAT:
			break;
		case MSG_ADD_BLOCK:
			break;
		case MSG_COLLIDE:
			break;
		case MSG_LEAVE:
			break;
		case MSG_GAMECLEAR:
			break;
		case MSG_PAUSE:
			break;
		default:
			break;
		}
	

		// data�� ���⼭ ó���ؾ� ��

		// ���� ������ ���
		std::cout << "MSG - " << data << std::endl;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	// ����� �μ��� ������ IP �ּҷ� ���
	if (argc > 1) SERVERIP = argv[1];

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	std::cout << std::endl << " ======== Login ======== " << std::endl << std::endl;
	while (true) {
		std::cout << std::endl << "��� �� �г����� �Է����ּ���. (10�� �̳�) : ";
		std::cin >> m_Name;
		if (m_Name.size() > 10)
			std::cout << "�г����� ���� 10�� �̻����� �� �� �����ϴ�." << std::endl;
		else
			break;
	}
	std::cout << std::endl << "���� �� �����ּҸ� �Է����ּ���(ex 197.xxx.xxx.xxx) : ";
	std::string server_s;
	std::cin >> server_s;

	SERVERIP = new char[server_s.size() + 1];
	SERVERIP[server_s.size()] = '\0';
	strcpy(SERVERIP, server_s.c_str());

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	HANDLE hThread;

	hThread = CreateThread(NULL, 0, RecvMSGThread, (LPVOID)sock, 0, NULL);
	CloseHandle(hThread);


	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	int comm;
	// ������ ������ ���
	while (1) {
		// ������ �Է�
		printf("\nData Send : ");
		std::cin >> comm;

		// �޼��� ������
		GAMEMSG TempMSG;

		switch (comm)
		{
		case MSG_PLAYER_INFO_LOBBY:
			TempMSG = MSG_PLAYER_INFO_LOBBY;
			break;
		case MSG_PLAYER_INFO_SCENE:
			TempMSG = MSG_PLAYER_INFO_SCENE;
			break;
		case MSG_CHAT:
			TempMSG = MSG_CHAT;
			break;
		case MSG_ADD_BLOCK:
			TempMSG = MSG_ADD_BLOCK;
			break;
		case MSG_COLLIDE:
			TempMSG = MSG_COLLIDE;
			break;
		case MSG_LEAVE:
			TempMSG = MSG_LEAVE;
			break;
		case MSG_GAMECLEAR:
			TempMSG = MSG_GAMECLEAR;
			break;
		case MSG_PAUSE:
			TempMSG = MSG_PAUSE;
			break;
		default:
			break;
		}
		sendMSG(sock, TempMSG);

		// ������ ������
		char pc = 'R';
		retval = sendPlayerInfoLobby(sock, PlayerInfoLobby{ TempMSG, (char*)m_Name.c_str(), pc});
		if (retval == -1)
			break;

		//// �޼��� �ޱ�
		//GAMEMSG recv_msg = recvMSG(sock);
		//printf("���� �޼��� : %d\n",recv_msg);

		//// ������ �ޱ�
		//PlayerInfoLobby tempInfo{ recvPlayerInfoLobby(sock) };

		//// ���� ������ ���
		//std::cout << "MSG - " << tempInfo.GetMsg() << std::endl;
		//std::cout << "ID - " << tempInfo.GetID() << std::endl;
		//std::cout << "READY - " << tempInfo.GetReady() << std::endl;
	}

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
