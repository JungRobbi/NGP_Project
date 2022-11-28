#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "../../Common.h"
#include "../../../Server/GameData.h"

#include "MSGFunc.h"
#include "PlayerInfoLobbyFunc.h"

#define SERVERPORT 9000
#define BUFSIZE    50


std::list<GameData*> MsgCommandQueue{};
std::vector<SOCKET*> ClientSockList{};
CRITICAL_SECTION cs;


DWORD WINAPI ClientThread(LPVOID arg)
{
	std::string m_Name;
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	ClientSockList.push_back(&client_sock);
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];


	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1) {
		// 메세지 받기
		GAMEMSG recv_msg = recvMSG(client_sock);

		// 메세지 해석 -> 데이터 받기 -> 메세지 큐에 입력

		EnterCriticalSection(&cs);
		switch (recv_msg) // 메세지 해석
		{
		case MSG_PLAYER_INFO_LOBBY:  // 데이터 받고 큐에 입력
			MsgCommandQueue.emplace_back(new PlayerInfoLobby{ recvPlayerInfoLobby(client_sock) });
			if (m_Name.empty())
				m_Name = ((PlayerInfoLobby*)MsgCommandQueue.back())->GetID();

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
		LeaveCriticalSection(&cs);
		

		//// 메세지 보내기
		////GAMEMSG TempMSG = MSG_PLAYER_INFO_LOBBY;
		//sendMSG(client_sock, recv_msg);

		//// 데이터 보내기
		//char pc = 'R';
		//retval = sendPlayerInfoLobby(client_sock, PlayerInfoLobby{ recv_msg, (char*)m_Name.c_str(), pc });
		//if (retval == -1)
		//	break;
	}

	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));

	return 0;
}

DWORD WINAPI Cacul_Execute(LPVOID arg)
{
	while (1) {
		if (MsgCommandQueue.empty() || ClientSockList.empty())
			continue;

		EnterCriticalSection(&cs);
		GameData* data = MsgCommandQueue.front();
		
		// 데이터 처리

		std::cout << data->GetMsg() << std::endl;

		switch (data->GetMsg())
		{
		case MSG_PLAYER_INFO_LOBBY:
			std::cout << "MSG_PLAYER_INFO_LOBBY" << std::endl;
			std::cout << ((PlayerInfoLobby*)data)->GetID() << std::endl;
			std::cout << ((PlayerInfoLobby*)data)->GetReady().x << std::endl << std::endl;

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

		switch (data->GetMsg())
		{
		case MSG_PLAYER_INFO_LOBBY:
			for (auto p = ClientSockList.begin(); p != ClientSockList.end(); ++p) {
				sendMSG(*(*p), MSG_PLAYER_INFO_LOBBY);

				// 데이터 보내기
				int retval = sendPlayerInfoLobby(*(*p), PlayerInfoLobby{ data->GetMsg(), ((PlayerInfoLobby*)data)->GetID(), ((PlayerInfoLobby*)data)->GetReady() });
				if (retval == -1)
					break;
			}
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
	
		MsgCommandQueue.pop_front();
		LeaveCriticalSection(&cs);
	}

	return 0;
}


int main(int argc, char* argv[])
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	InitializeCriticalSection(&cs);

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	int len;
	char buf[BUFSIZE + 1];
	HANDLE hThread;
	HANDLE hCacul_ExecuteThread;

	hCacul_ExecuteThread = CreateThread(NULL, 0, Cacul_Execute, NULL, 0, NULL);
	CloseHandle(hCacul_ExecuteThread);
	
	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL)
		{
			closesocket(client_sock);
		}
		else
		{
			CloseHandle(hThread);
		}

	}

	closesocket(listen_sock);

	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}