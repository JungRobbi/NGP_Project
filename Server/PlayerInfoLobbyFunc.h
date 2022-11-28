#pragma once
#include "Common.h"
#include "GameData.h"


int sendPlayerInfoLobby(SOCKET sock, PlayerInfoLobby data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	return retval;

}

PlayerInfoLobby recvPlayerInfoLobby(SOCKET sock)
{
	int retval;
	PlayerInfoLobby data;
	retval = recv(sock, (char*)&data, sizeof(data), 0);
	if (retval == 0) 
		data.SetMsg(MSG_LEAVE);
	return data;
}