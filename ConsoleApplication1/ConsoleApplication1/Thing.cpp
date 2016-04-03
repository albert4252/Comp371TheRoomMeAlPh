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
	mat_model = glm::translate(mat_model, transl);
	mat_model = glm::scale(mat_model, glm::vec3(scale));
	
	

	vtx_brn = mat_model*vtx_brn;
	vtx_tlf = mat_model*vtx_tlf;

	glm::vec3 half_xtnt((vtx_brn.x - vtx_tlf.x) / 2, (vtx_brn.y - vtx_tlf.y) / 2, (vtx_brn.z - vtx_tlf.z) / 2);
	shape = new btBoxShape(btVector3(half_xtnt.x, half_xtnt.y, half_xtnt.z));

	
	offset = glm::vec3(vtx_tlf.x, vtx_tlf.y, vtx_tlf.z) + half_xtnt;

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(offset.x, offset.y, offset.z)));
	shape->calculateLocalInertia(btScalar(1), btVector3(0, 0, 0));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(btScalar(1), motionState, shape, btVector3(0, 0, 0));
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	r_prev = offset;

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

void Thing::draw(btDiscreteDynamicsWorld* world)
{
	world->stepSimulation(1 / 60.f, 10);
	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);
	glm::vec3 r_now(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
	glm::vec3 delta_r = r_now - r_prev;
	//delta_r = delta_r - offset;
	mat_model = glm::translate(mat_model, delta_r);
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mat_model));
	model->Draw(*shader);
	r_prev = r_now;
}