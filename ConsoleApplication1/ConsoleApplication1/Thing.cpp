#pragma once
#include "stdafx.h"
#include "Thing.h"

Thing::Thing(Model* _model, Shader* _shader, Camera cam, GLfloat z_diff, GLfloat scale)
{
	model = _model;
	shader = _shader;
	glm::vec4 vtx_tlf(-83.8f, -56.2f, -37.4f, 1.0f); //top left far (but not really)
	glm::vec4 vtx_brn(71.9f, -1.0f, 37.2f, 1.0f); //bottom right near (but not really)

	////shape = new
	glm::vec3 transl(cam.Position + z_diff*cam.Front);
	vector <Mesh> meshez = model->getMeshes();
	mat_model = glm::scale(mat_model, glm::vec3(scale));
	mat_model = glm::translate(mat_model, transl);
	

	vtx_brn = mat_model*vtx_brn;
	vtx_tlf = mat_model*vtx_tlf;

	glm::vec3 half_xtnt((vtx_brn.x - vtx_tlf.x) / 2, (vtx_brn.y - vtx_tlf.y) / 2, (vtx_brn.z - vtx_tlf.z) / 2);
	shape = new btBoxShape(btVector3(half_xtnt.x, half_xtnt.y, half_xtnt.z));

	glm::vec3 offset;
	offset = glm::vec3(vtx_tlf.x, vtx_tlf.y, vtx_tlf.z) + half_xtnt;

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(offset.x, offset.y, offset.z)));

	//std::vector<Mesh> meshes = model->getMeshes();
	//for (int i = 0; i < meshes.size(); i++)
	//{
	//	for (int j = 0; j < meshes[i].vertices.size(); j++)
	//	{
	//		std::cout << meshes[i].vertices[i].Position.x << " " << meshes[i].vertices[i].Position.y << " " << meshes[i].vertices[i].Position.z << "\n";
	//	}
	//}
	//std::cout << "\n";

}

void Thing::draw()
{
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mat_model));
	model->Draw(*shader);
}