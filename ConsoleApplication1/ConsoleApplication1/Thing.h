#pragma once
#include "Model.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class Thing
{
private:
	Model* model;
	Shader* shader;
public:
	glm::mat4 mat_model;
	Thing(Model*, Shader*, Camera cam, GLfloat z_diff = 50.0f, GLfloat scale = 0.2f);//z_diff is how far from camera object is spawned, scale is how much it is scaled
	void draw();
};