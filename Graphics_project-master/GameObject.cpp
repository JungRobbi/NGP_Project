#include "GameObject.h"
#include "Transform3D.h"

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

	if (obj_num == 1) {
		glUniform4f(Pcolor, 1.0, 0.0, 0.0, 0.0);
	}
	else {
		glUniform4f(Pcolor, 0.0, 0.0, 0.0, 0.0);
	}

	glBindVertexArray(VAO);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tr));
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, num_index);


}