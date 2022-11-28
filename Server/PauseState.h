#pragma once
#include "Common.h"
#include "GameData.h"


void sendPauseState(SOCKET sock, Pause data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}


}

Pause recvPauseState(SOCKET sock)
{
	int retval;
	Pause data;
	retval = recv(sock, (char*)&data, sizeof(data), 0);
	return data;
}