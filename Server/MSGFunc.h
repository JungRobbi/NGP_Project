#pragma once
#include "Common.h"
#include "GameData.h"

void sendMSG(SOCKET sock, GAMEMSG data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}


}

GAMEMSG recvMSG(SOCKET sock)
{
	int retval;
	GAMEMSG data;
	retval = recv(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		return MSG_LEAVE;
	}
	else if (retval == 0) 
		return MSG_LEAVE;
	return data;
}