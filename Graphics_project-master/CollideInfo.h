#pragma once

#include "Common.h"
#include "GameData.h"


inline int sendCollideInfo(SOCKET& sock, S_Collide& data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(S_Collide), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	return retval;


}

inline S_Collide recvCollideInfo(SOCKET& sock)
{
	int retval;
	S_Collide data;
	retval = recv(sock, (char*)&data, sizeof(S_Collide), 0);
	return data;
}