#pragma once
#include "../../Common.h"
#include "../../../Server/GameData.h"


void sendPlayerInfoLobby(SOCKET sock, PlayerInfoLobby data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}


}

PlayerInfoLobby recvPlayerInfoLobby(SOCKET sock)
{
	int retval;
	PlayerInfoLobby data;
	retval = recv(sock, (char*)&data, sizeof(data), 0);
	return data;
}