#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <queue>


enum GAMEMSG
{
	MSG_PLAYER_INFO_LOBBY,
	MSG_PLAYER_INFO_SCENE,
	MSG_CHAT,
	MSG_ADD_BLOCK,
	MSG_COLLIDE,
	MSG_LEAVE,
	MSG_GAMECLEAR,
	MSG_PAUSE
};

struct Vector3
{
	float x;
	float y;
	float z;
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
	PlayerInfoLobby(int datamsg, char* id, char ready);
	~PlayerInfoLobby() {};
private:
	char ID[10];
	char ready_state;   // 0:white 1:R 2:G 3:B
	char padding[34];
};

class PlayerInfoScene : public GameData
{
public:
	PlayerInfoScene() {}
	PlayerInfoScene(int datasmg, Vector3 pos, char* id);
	~PlayerInfoScene() {}


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
private:
	Vector3 position;
	char padding[34];
};

class Collide : public GameData
{
public:
	Collide() {}
	Collide(int datamsg, int index);
	~Collide() {}
private:
	int item_index;
	char padding[42];
};
class Leave : public GameData
{
public:
	Leave(){}
	Leave(int datamsg, char* id);
	~Leave() {}
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
private:
	bool pause;
	char padding[44];
};


