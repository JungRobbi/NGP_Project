#pragma once
#include "../../Common.h"
#include "../../../Server/GameData.h"


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
	return data;
}