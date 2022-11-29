#include "OtherPlayer.h"

void OtherPlayer::start()
{
	
}

void OtherPlayer::update()
{
	gameObject->GetComponent<Transform3D>()->position = glm::vec3(pos.x,pos.y,pos.z);
}