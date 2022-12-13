#pragma once

#include "Common.h"
#include "GameData.h"


void sendPause(SOCKET sock, Pause data)
{
	int retval;
	retval = send(sock, (char*)&data, 52, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}


}

Pause recvPause(SOCKET sock)
{
	int retval;
	Pause data;
	retval = recv(sock, (char*)&data, 52, 0);
	return data;
}