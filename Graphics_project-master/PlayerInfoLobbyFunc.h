#pragma once
#include "Common.h"
#include "GameData.h"


int sendPlayerInfoLobby(SOCKET& sock, PlayerInfoLobby& data)
{
	int retval;
	retval = send(sock, (char*)&data, 52, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	return retval;

}

PlayerInfoLobby recvPlayerInfoLobby(SOCKET& sock)
{
	int retval;
	PlayerInfoLobby data;
	retval = recv(sock, (char*)&data, 52, 0);
	std::cout << retval << "Byte Get" << std::endl;
	return data;
}