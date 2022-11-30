#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include "Vector3.h"


enum GAMEMSG
{
	MSG_PLAYER_INFO_LOBBY,
	MSG_PLAYER_INFO_SCENE,
	MSG_CHAT,
	MSG_ADD_BLOCK,
	MSG_COLLIDE,
	MSG_LEAVE,
	MSG_GAMECLEAR,
	MSG_PAUSE,
	MSG_NORMAL
};

class GameData
{
public:
	GameData() { msg = -1; }
	~GameData() {}

	int GetMsg() { return msg; }
protected:
	int msg;
private:

};





class PlayerInfoLobby : public GameData
{
public:
	PlayerInfoLobby() {}
	PlayerInfoLobby(int datamsg, char* id, Vector3 ready);
	~PlayerInfoLobby() {};

	char* GetID() { return ID; }
	Vector3 GetReady() { return ready_state; }

private:
	char ID[10];
	Vector3 ready_state;   // 0:white 1:R 2:G 3:B
	char padding[34];
};

class PlayerInfoScene : public GameData
{
public:
	PlayerInfoScene() {}
	PlayerInfoScene(int datasmg, Vector3 pos, char* id);
	~PlayerInfoScene() {}

	char* GetID() { return ID; }
	Vector3 GetPos() { return position; }

private:
	Vector3 position;
	char ID[10];
	char padding[24];

};

class Chat : public GameData
{
public:
	Chat() {}
	Chat(int datamsg, char* id, char* chatdata);
	~Chat() {}
	char* GetID() { return ID; }
	char* GetMessage() { return message; }
private:
	char ID[10];
	char message[30];
	char padding[6];
};

class AddBlock : public GameData
{
public:
	AddBlock() {}
	AddBlock(int datamsg, Vector3 pos);
	~AddBlock() {}
	Vector3 GetPosition() { return position; }

private:
	Vector3 position;
	char padding[34];
};

class S_Collide : public GameData
{
public:
	S_Collide() {}
	S_Collide(int datamsg, int index);
	~S_Collide() {}
	int GetItem_index() { return item_index; }
private:
	char ID[10];
	int item_index;
	char padding[32];
};
class Leave : public GameData
{
public:
	Leave(){}
	Leave(int datamsg, char* id);
	~Leave() {}
	char* GetID() { return ID; }

private:
	char ID[10];
	char padding[36];
};

class GameClear : public GameData
{
public:
	GameClear() {}
	GameClear(int datamsg);
	~GameClear() {}
private:
	char padding[46];
};

class Pause : public GameData
{
public:
	Pause() {}
	Pause(int datmsg, bool pause_enable);
	~Pause() {}
	bool GetPause() { return pause; }

private:
	bool pause;
	char padding[44];
};


