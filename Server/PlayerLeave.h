#pragma once
#include "Common.h"
#include "GameData.h"

void sendPlayerLeave(SOCKET sock, Leave data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}

}

void recvPlayerLeave(SOCKET sock)
{
	int retval;
	PlayerInfoScene data;
	retval = recv(sock, (char*)&data, sizeof(data), 0);
}