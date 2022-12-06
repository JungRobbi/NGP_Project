#pragma once

#include "Common.h"
#include "GameData.h"


int sendCollideInfo(SOCKET sock, S_Collide data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(S_Collide), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	return retval;


}

PlayerInfoScene recvCollideInfo(SOCKET sock)
{
	int retval;
	PlayerInfoScene data;
	retval = recv(sock, (char*)&data, sizeof(S_Collide), 0);
	return data;
}