#pragma once

#include "Common.h"
#include "GameData.h"


void sendGameClear(SOCKET sock, GameClear data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}


}

PlayerInfoScene recvGameClear(SOCKET sock)
{
	int retval;
	PlayerInfoScene data;
	retval = recv(sock, (char*)&data, sizeof(data), 0);
	return data;
}