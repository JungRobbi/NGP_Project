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

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (1) {
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' ���� ����
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// �޼��� ������
		GAMEMSG TempMSG = MSG_PLAYER_INFO_LOBBY;
		sendMSG(sock, TempMSG);

		// ������ ������
		char* pstr = (char*)"123";
		char pc = 'q';
		retval = sendPlayerInfoLobby(sock, PlayerInfoLobby{ TempMSG, pstr, pc });
		if (retval == -1)
			break;

		//// �޼��� �ޱ�
		//GAMEMSG recv_msg = recvMSG(sock);
		//printf("���� �޼��� : %d\n",recv_msg);

		//// ������ �ޱ�
		//retval = recv(sock, buf, retval, MSG_WAITALL);
		//if (retval == SOCKET_ERROR) {
		//	err_display("recv()");
		//	break;
		//}
		//else if (retval == 0)
		//	break;

		//// ���� ������ ���
		//buf[retval] = '\0';
		//printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		//printf("[���� ������] %s\n", buf);
	}

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
