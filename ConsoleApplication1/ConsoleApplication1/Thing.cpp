#pragma once
#include "stdafx.h"
#include "Thing.h"

Thing::Thing(Model* _model, Shader* _shader, Camera cam, GLfloat z_diff, GLfloat _scale, btDiscreteDynamicsWorld* w)
{
	world = w;
	scale = _scale;
	model = _model;
	shader = _shader;

	///UNTRANSFORMED coords
	//vtx_tlf = glm::vec4(-83.8f, -56.2f, -37.4f, 1.0f); //top left far (but not really)
	//vtx_brn = glm::vec4(71.9f, -1.0f, 37.2f, 1.0f); //bottom right near (but not really)
	vtx_tlf = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
	vtx_brn = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


	////shape = new
	transl = glm::vec3(cam.Position + z_diff*cam.Front);
	//vector <Mesh> meshez = model->getMeshes();
	mat_model = glm::translate(mat_model, transl);
	mat_model = glm::scale(mat_model, glm::vec3(scale));
	
	
	
	

	vtx_brn = mat_model*vtx_brn;
	vtx_tlf = mat_model*vtx_tlf;

	glm::vec3 half_xtnt((vtx_brn.x - vtx_tlf.x) / 2, (vtx_brn.y - vtx_tlf.y) / 2, (vtx_brn.z - vtx_tlf.z) / 2);
	shape = new btBoxShape(btVector3(half_xtnt.x, half_xtnt.y, half_xtnt.z));

	
	offset = glm::vec3(vtx_tlf.x, vtx_tlf.y, vtx_tlf.z) + half_xtnt;
	//offset = glm::vec3(0.0f, 0.0f, 0.0f);

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(offset.x, offset.y, offset.z)));
	//motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));



	mass = 1;
	inertia = btVector3(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, inertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);

	
	world->addRigidBody(fallRigidBody);
	
	//btTransform tr;
	//fallRigidBody->getMotionState()->getWorldTransform(tr);
	//tr.setOrigin(btVector3(transl.x, transl.y, transl.z));

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

void Thing::draw()
{
	//world->stepSimulation(1.0f / 60.0f, (int)1, btScalar(1.) / btScalar(60.));
	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);
	glm::vec3 r_now(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	
	btQuat = trans.getRotation();
	glmQuat = glm::quat(btQuat.getX(), btQuat.getY(), btQuat.getZ(), btQuat.getW());
	glm::vec3 delta_r = r_now - r_prev;
	

	
	axis = glm::axis(glmQuat);
	angle = glm::angle(glmQuat);

	mat_model = glm::translate(mat_model, -delta_r);
	mat_model = glm::rotate(mat_model, angle, axis);

	
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mat_model));
	model->Draw(*shader);
	r_prev = r_now;
	mat_model = glm::rotate(mat_model, -angle, axis);

}