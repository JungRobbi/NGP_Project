#include "GameData.h"
#include <iostream>


PlayerInfoLobby::PlayerInfoLobby(int datamsg, char* id, Vector3 ready)
{
	msg = datamsg;
	ready_state = ready;
	memset(ID, 0, sizeof(ID));
	memcpy(ID, id,sizeof(ID));
}

PlayerInfoScene::PlayerInfoScene(int datasmg, Vector3 pos, char* id)
{
	msg = datasmg;
	position = pos;
	memset(ID, 0, sizeof(ID));
	strcpy(ID, id);
}

Chat::Chat(int datamsg, char* id, char* chatdata)
{
	msg = datamsg;
	memset(ID, 0, sizeof(ID));
	strcpy(ID, id);
	memset(message, 0, sizeof(message));
	strcpy(message, chatdata);
}

AddBlock::AddBlock(int datamsg, Vector3 pos)
{
	msg = datamsg;
	position = pos;
}

S_Collide::S_Collide(int datamsg, int index)
{
	msg = datamsg;
	item_index = index;
}

Leave::Leave(int datamsg, char* id)
{
	msg = datamsg;
	memset(ID, 0, sizeof(ID));
	strcpy(ID, id);
}

GameClear::GameClear(int datamsg)
{
	msg = datamsg;
}

Pause::Pause(int datmsg, bool pause_enable)
{
	msg = datmsg;
	pause = pause_enable;
}
