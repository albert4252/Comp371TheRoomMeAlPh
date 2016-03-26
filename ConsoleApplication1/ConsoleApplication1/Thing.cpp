#pragma once
#include "stdafx.h"
#include "Thing.h"

Thing::Thing(Model* _model, Shader* _shader, Camera cam, GLfloat z_diff, GLfloat scale)
{
	model = _model;
	shader = _shader;
	glm::vec3 difference;
	mat_model = glm::translate(mat_model, glm::vec3(cam.Position + z_diff*cam.Front));
	mat_model = glm::scale(mat_model, glm::vec3(scale));
}

void Thing::draw()
{
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mat_model));
	model->Draw(*shader);
}