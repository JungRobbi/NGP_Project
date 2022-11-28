#pragma once
#include "Common.h"
#include "GameData.h"

void sendMSG(SOCKET sock, GAMEMSG data)
{
	int retval;
	retval = send(sock, (char*)&data,sizeof(4), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}


}

GameData* recvMSG(SOCKET sock)
{
	int retval;
	GameData data;
	retval = recv(sock, (char*)&data, 52, 0);
	GameData* rdata = new GameData(data);
	return rdata;
	//if (retval == SOCKET_ERROR) {
	//	return ;
	//}
	//else if (retval == 0) 
	//	return ;
	//return ;
}