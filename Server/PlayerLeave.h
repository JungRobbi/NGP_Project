#pragma once
#include "Common.h"
#include "GameData.h"

void sendPlayerLeave(SOCKET sock, Leave data)
{
	int retval;
	retval = send(sock, (char*)&data, 52, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}

}

Leave recvPlayerLeave(SOCKET sock)
{
	int retval;
	Leave data;
	retval = recv(sock, (char*)&data, 52, 0);
	

	return data;
}