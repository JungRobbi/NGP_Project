#include "OtherPlayer.h"

void OtherPlayer::start()
{
	color = { 0.0f, 1.0f, 0.0f };
	pos = { 0.0f, 0.0f, 0.0f };
}

void OtherPlayer::update()
{
	gameObject->GetComponent<Transform3D>()->position = pos;
}