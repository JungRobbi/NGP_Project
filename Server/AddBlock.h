#pragma once
#include "Common.h"
#include "GameData.h"

void sendAddBlock(SOCKET sock, AddBlock data)
{
	int retval;
	retval = send(sock, (char*)&data, sizeof(data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");

		return;
	}
}

AddBlock recvAddBlock(SOCKET sock)
{
	int retval;
	AddBlock data;
	retval = recv(sock, (char*)&data, sizeof(data), 0);
	return data;
}