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
		// 메세지 받기
		recv_msg = recvMSG(sock);
		printf("받은 메세지 : %d\n", recv_msg);
		if (recv_msg == NULL) {
			std::cout << "recv_msg - NULL" << std::endl;
			break;
		}

		GameData* RecvData;

		switch (recv_msg) // 메세지 해석
		{
		case MSG_PLAYER_INFO_LOBBY:  // 데이터 받기
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
	

		// data를 여기서 처리해야 함

		// 받은 데이터 출력
		std::cout << "MSG - " << data << std::endl;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	std::cout << std::endl << " ======== Login ======== " << std::endl << std::endl;
	while (true) {
		std::cout << std::endl << "사용 할 닉네임을 입력해주세요. (10자 이내) : ";
		std::cin >> m_Name;
		if (m_Name.size() > 10)
			std::cout << "닉네임은 영문 10자 이상으로 할 수 없습니다." << std::endl;
		else
			break;
	}
	std::cout << std::endl << "접속 할 서버주소를 입력해주세요(ex 197.xxx.xxx.xxx) : ";
	std::string server_s;
	std::cin >> server_s;

	SERVERIP = new char[server_s.size() + 1];
	SERVERIP[server_s.size()] = '\0';
	strcpy(SERVERIP, server_s.c_str());

	// 소켓 생성
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


	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	int comm;
	// 서버와 데이터 통신
	while (1) {
		// 데이터 입력
		printf("\nData Send : ");
		std::cin >> comm;

		// 메세지 보내기
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

		// 데이터 보내기
		char pc = 'R';
		retval = sendPlayerInfoLobby(sock, PlayerInfoLobby{ TempMSG, (char*)m_Name.c_str(), pc});
		if (retval == -1)
			break;

		//// 메세지 받기
		//GAMEMSG recv_msg = recvMSG(sock);
		//printf("받은 메세지 : %d\n",recv_msg);

		//// 데이터 받기
		//PlayerInfoLobby tempInfo{ recvPlayerInfoLobby(sock) };

		//// 받은 데이터 출력
		//std::cout << "MSG - " << tempInfo.GetMsg() << std::endl;
		//std::cout << "ID - " << tempInfo.GetID() << std::endl;
		//std::cout << "READY - " << tempInfo.GetReady() << std::endl;
	}

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
