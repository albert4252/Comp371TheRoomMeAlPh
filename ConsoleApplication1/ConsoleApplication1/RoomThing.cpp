#pragma once
#include "stdafx.h"
#include "RoomThing.h"

RoomThing::RoomThing(Model* _model, Shader* _shader, glm::vec3 _vtx_tlf, glm::vec3 _vtx_brn, glm::vec3 center, GLfloat _scale, btDiscreteDynamicsWorld* w)
{
	world = w;
	scale = _scale;
	model = _model;
	shader = _shader;

	///UNTRANSFORMED coords
	vtx_tlf = _vtx_tlf;
	vtx_brn = _vtx_brn;

	////shape = new
	transl = center;
	mat_model = glm::scale(mat_model, glm::vec3(scale));
	mat_model = glm::translate(mat_model, transl);
	
	
	



	vtx_brn = glm::vec3((mat_model*glm::vec4(vtx_brn, 1.0f)).x, (mat_model*glm::vec4(vtx_brn, 1.0f)).y, (mat_model*glm::vec4(vtx_brn, 1.0f)).z);
	vtx_tlf = glm::vec3((mat_model*glm::vec4(vtx_tlf, 1.0f)).x, (mat_model*glm::vec4(vtx_tlf, 1.0f)).y, (mat_model*glm::vec4(vtx_tlf, 1.0f)).z);

	glm::vec3 half_xtnt((vtx_brn.x - vtx_tlf.x) / 2, (vtx_brn.y - vtx_tlf.y) / 2, (vtx_brn.z - vtx_tlf.z) / 2);
	shape = new btBoxShape(btVector3(half_xtnt.x, half_xtnt.y, half_xtnt.z));


	offset = glm::vec3(vtx_tlf.x, vtx_tlf.y, vtx_tlf.z) + half_xtnt;

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), /*btVector3(transl.x, transl.y, transl.z) +*/ btVector3(offset.x, offset.y, offset.z)));



	mass = 1.0f;
	inertia = btVector3(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, inertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);


	world->addRigidBody(fallRigidBody);

	//btTransform tr;
	//fallRigidBody->getMotionState()->getWorldTransform(tr);
	//tr.setOrigin(btVector3(transl.x, transl.y, transl.z));



	//btTransform trans;
	//fallRigidBody->getMotionState()->getWorldTransform(trans);
	//glm::vec3 r_now(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	r_prev = offset;


	mat_model = glm::rotate(mat_model, 3.1415926535897932384f, glm::vec3(0, 0, -1.0f));
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

void RoomThing::draw()
{
	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);
	glm::vec3 r_now(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

	btQuat = trans.getRotation();
	glmQuat = glm::quat(btQuat.getX(), btQuat.getY(), btQuat.getZ(), btQuat.getW());
	glm::vec3 delta_r = r_now - r_prev;



	axis = glm::axis(glmQuat);
	angle = glm::angle(glmQuat);
	mat_model = glm::rotate(mat_model, angle, axis);
	mat_model = glm::translate(mat_model, delta_r);
	


	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mat_model));
	model->Draw(*shader);
	r_prev = r_now;
	mat_model = glm::rotate(mat_model, -angle, axis);

}