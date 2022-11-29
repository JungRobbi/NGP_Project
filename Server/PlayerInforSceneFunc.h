#pragma once
#include "Common.h"
#include "GameData.h"


int sendPlayerInfoScene(SOCKET sock, PlayerInfoScene data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(PlayerInfoScene), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	return retval;

}

PlayerInfoScene recvPlayerInfoScene(SOCKET sock)
{
	int retval;
	PlayerInfoScene data;
	retval = recv(sock, (char*)&data, sizeof(PlayerInfoScene), 0);
	return data;
}