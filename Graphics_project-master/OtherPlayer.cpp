#include "OtherPlayer.h"

void OtherPlayer::start()
{

}

void OtherPlayer::update()
{
	gameObject->GetComponent<Transform3D>()->position = pos;
}