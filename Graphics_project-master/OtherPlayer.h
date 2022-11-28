#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"

class OtherPlayer : public Component
{
public:
	Vector3 color;
	glm::vec3 pos;
	char ID[10];
public:

	void start();
	void update();
};

