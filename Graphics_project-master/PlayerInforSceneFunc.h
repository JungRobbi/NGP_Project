#pragma once
#include "Common.h"
#include "GameData.h"


inline void sendPlayerInfoScene(SOCKET& sock, PlayerInfoScene& data)
{
	int retval;
	retval = send(sock, (char*)&data, 52, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}


}

inline PlayerInfoScene recvPlayerInfoScene(SOCKET& sock)
{
	int retval;
	PlayerInfoScene data;
	retval = recv(sock, (char*)&data, 52, 0);
	return data;
}