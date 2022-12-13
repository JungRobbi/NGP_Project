#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

#include "Common.h"
#include "GameData.h"

#include "MSGFunc.h"
#include "PlayerInfoLobbyFunc.h"
#include "PlayerInforSceneFunc.h"
#include "AddBlock.h"
#include "CollideInfo.h"

#define SERVERPORT 9000


std::list<GameData*> MsgCommandQueue{};
std::list<SOCKET> ClientSockList;

HANDLE hClientEvent[4];
HANDLE hSendEvent;

CRITICAL_SECTION cs;
CRITICAL_SECTION socklist_cs;

DWORD WINAPI ClientThread(LPVOID arg)
{
	std::string m_Name;
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	EnterCriticalSection(&socklist_cs);
	ClientSockList.push_back(SOCKET{ client_sock });
	LeaveCriticalSection(&socklist_cs);
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;


	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1) {
		char buf[52];
		retval = recv(client_sock, buf, 52, 0);
		if (retval == SOCKET_ERROR)
			break;

		//retval = WaitForSingleObject(hSendEvent, INFINITE);
		//if (retval != WAIT_OBJECT_0) break;

		int msg;
		memcpy(&msg, buf, 4);

		EnterCriticalSection(&cs);

		if (msg == MSG_LEAVE) break;
		
		// �޼��� �ؼ� -> ������ �ޱ� -> �޼��� ť�� �Է�

		//GameData* gamedata{};

		GameData* data;
		switch (msg) // �޼��� �ؼ�
		{
		case MSG_PLAYER_INFO_LOBBY:  // ������ �ް� ť�� �Է�
			data = new PlayerInfoLobby;
			::ZeroMemory(data, sizeof(data));
			memcpy(((PlayerInfoLobby*)data), buf, sizeof(buf));
			MsgCommandQueue.push_front((PlayerInfoLobby*)data);
			break;
		case MSG_PLAYER_INFO_SCENE:
			data = new PlayerInfoScene;
			::ZeroMemory(data, sizeof(data));
			memcpy(((PlayerInfoScene*)data), buf, sizeof(buf));
			MsgCommandQueue.push_back((PlayerInfoScene*)data);
			break;
		case MSG_CHAT:
			break;
		case MSG_ADD_BLOCK:
			data = new AddBlock;
			::ZeroMemory(data, sizeof(data));
			memcpy(((AddBlock*)data), buf, sizeof(buf));
			MsgCommandQueue.push_back((AddBlock*)data);
			break;
		case MSG_COLLIDE:
			data = new S_Collide;
			::ZeroMemory(data, sizeof(data));
			memcpy(((S_Collide*)data), buf, sizeof(buf));
			MsgCommandQueue.push_front((S_Collide*)data);
			break;
		case MSG_LEAVE:
			std::cout << "MSG_LEAVE2 === " << std::endl;
			break;
		case MSG_GAMECLEAR:
			break;
		case MSG_PAUSE:
			break;
		default:
			//gamedata = new GameData();
			break;
		}
		LeaveCriticalSection(&cs);
		//// �޼��� ������
		////GAMEMSG TempMSG = MSG_PLAYER_INFO_LOBBY;
		//sendMSG(client_sock, recv_msg);

		//// ������ ������
		//char pc = 'R';
		//retval = sendPlayerInfoLobby(client_sock, PlayerInfoLobby{ recv_msg, (char*)m_Name.c_str(), pc });
		//if (retval == -1)
		//	break;
	}
	EnterCriticalSection(&socklist_cs);
	auto p = find(ClientSockList.begin(), ClientSockList.end(), client_sock);
	closesocket(*p);
	ClientSockList.remove(*p);
	LeaveCriticalSection(&socklist_cs);
	//ClientSockList.remove_if([&client_sock](SOCKET a) { return a == client_sock; });
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));

	return 0;
}

DWORD WINAPI Cacul_Execute(LPVOID arg)
{
	bool b_connect = false;
	while (!b_connect) {
		EnterCriticalSection(&socklist_cs);
		if (ClientSockList.size() >= 3) {
			std::cout << " LOBBY DATA ���� " << std::endl;
			b_connect = true;

			EnterCriticalSection(&cs);
			while (!MsgCommandQueue.empty()) {
				GameData* data = MsgCommandQueue.front();

				for (auto p = ClientSockList.begin(); p != ClientSockList.end(); ++p) {
					std::cout << " SOCKET - " << *p << std::endl << std::endl;

					sendMSG(*p, MSG_PLAYER_INFO_LOBBY);

					// ������ ������
					int retval = sendPlayerInfoLobby(*p, PlayerInfoLobby{ data->GetMsg(), ((PlayerInfoLobby*)data)->GetID(), ((PlayerInfoLobby*)data)->GetReady() });
					if (retval == -1) {
						err_display("SendPlayerInfoLobby");
						break;
					}
				}

				MsgCommandQueue.pop_front();
			}
			LeaveCriticalSection(&cs);
		}
		LeaveCriticalSection(&socklist_cs);
	}

	while (b_connect) {
		EnterCriticalSection(&cs);

		while (!MsgCommandQueue.empty()) {
			EnterCriticalSection(&socklist_cs);
			if (ClientSockList.empty()) {
				MsgCommandQueue.clear();
				break;
			}

			//int retv = WaitForSingleObject(hClientEvent[0], INFINITE);
			//if (retv != WAIT_OBJECT_0) break;

			GameData* data = MsgCommandQueue.front();

			// ������ ó��
			switch (data->GetMsg())
			{
			case MSG_PLAYER_INFO_LOBBY:
				for (auto p = ClientSockList.begin(); p != ClientSockList.end(); ++p) {
					std::cout << " SOCKET - " << *p << std::endl << std::endl;

					sendMSG(*p, MSG_PLAYER_INFO_LOBBY);

					// ������ ������
					int retval = sendPlayerInfoLobby(*p, PlayerInfoLobby{ data->GetMsg(), ((PlayerInfoLobby*)data)->GetID(), ((PlayerInfoLobby*)data)->GetReady() });
					if (retval == -1) {
						err_display("SendPlayerInfoLobby");
						break;
					}
				}
				break;
			case MSG_PLAYER_INFO_SCENE:
				for (auto p = ClientSockList.begin(); p != ClientSockList.end(); ++p) {
					//std::cout << " SOCKET - " << *p << std::endl << std::endl;

					sendMSG(*p, MSG_PLAYER_INFO_SCENE);

					// ������ ������
					int retval = sendPlayerInfoScene(*p, PlayerInfoScene{ data->GetMsg(), ((PlayerInfoScene*)data)->GetPos(), ((PlayerInfoScene*)data)->GetID() });
					if (retval == -1) {
						err_display("SendPlayerInfoScene");
						break;
					}
				}
				break;
			case MSG_CHAT:
				break;
			case MSG_ADD_BLOCK:
				for (auto p = ClientSockList.begin(); p != ClientSockList.end(); ++p) {
					std::cout << " SOCKET - " << *p << std::endl << std::endl;

					sendMSG(*p, MSG_ADD_BLOCK);

					// ������ ������
					int retval = sendAddBlock(*p, AddBlock{ data->GetMsg(), ((AddBlock*)data)->GetPosition() });
					if (retval == -1) {
						err_display("SendAddBlock");
						break;
					}
				}
				break;
			case MSG_COLLIDE:
				for (auto p = ClientSockList.begin(); p != ClientSockList.end(); ++p) {
					//std::cout << " SOCKET - " << *p << std::endl << std::endl;

					sendMSG(*p, MSG_COLLIDE);

					// ������ ������
					int retval = sendCollideInfo(*p, S_Collide{ data->GetMsg(), ((S_Collide*)data)->GetItem_index() });
					if (retval == -1) {
						err_display("SendCollide");
						break;
					}
				}
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
			LeaveCriticalSection(&socklist_cs);
			MsgCommandQueue.pop_front();
		}
		LeaveCriticalSection(&cs);

	}

	return 0;
}


int main(int argc, char* argv[])
{
	hSendEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hClientEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);

	SetEvent(hSendEvent);

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

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&socklist_cs);

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
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
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
	DeleteCriticalSection(&socklist_cs);
	WSACleanup();
	return 0;
}