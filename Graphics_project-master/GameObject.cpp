#include "GameObject.h"
#include "Transform3D.h"
#include "OtherPlayer.h"


extern int Pcolor;

GameObject::GameObject()
{
	Scene::scene->creationQueue.push(this);
	
}

void GameObject::render()
{
	if (!GetComponent<Transform3D>())
		return;
	auto tr = GetComponent<Transform3D>()->mat_Transform;

	if (GetComponent<OtherPlayer>()) {
		tr = glm::translate(tr, glm::vec3(0.0f, -0.9f, 0.0f));
		glUniform4f(Pcolor, GetComponent<OtherPlayer>()->color.x, GetComponent<OtherPlayer>()->color.y, GetComponent<OtherPlayer>()->color.z, 0.0);
	}
	else {
		tr = GetComponent<Transform3D>()->mat_Transform;
		glUniform4f(Pcolor, 0.0, 0.0, 0.0, 0.0);
	}
	
	glBindVertexArray(VAO);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tr));
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, num_index);
}