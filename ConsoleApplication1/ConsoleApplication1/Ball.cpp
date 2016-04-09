#pragma once
#include "stdafx.h"
#include "Ball.h"

Ball::Ball(Model* _model, Shader* _shader, Camera cam, GLfloat _scale, btDiscreteDynamicsWorld* w, GLfloat _rad, GLfloat _speed)
{
	world = w;
	scale = _scale;
	model = _model;
	shader = _shader;
	rad = _rad;
	speed = _speed;
	transl = cam.Position+ (10.0f*(cam.Front));
	mat_model = glm::translate(mat_model, transl);
	mat_model = glm::scale(mat_model, glm::vec3(scale));



	shape = new btSphereShape(btScalar(scale*rad));

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(transl.x, transl.y, transl.z)));

	mass = 1;
	inertia = btVector3(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, inertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	glm::vec3 camfront = cam.Front;
	float camx = (camfront).x;
	float camy = (camfront).y;
	float camz = (camfront).z;
	btVector3 vel = btVector3(camx, camy, camz);
	vel *= speed;
	fallRigidBody->setLinearVelocity(vel);

	world->addRigidBody(fallRigidBody);


	r_prev = transl;
	mat_model = glm::rotate(mat_model, 3.1415926535897932384f, glm::vec3(0, 0, -1.0f));

	//btTransform trans;
	//fallRigidBody->getMotionState()->getWorldTransform(trans);
	//btQuatPrev = trans.getRotation();

}

void Ball::draw()
{
	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);
	glm::vec3 r_now(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

	btQuat = trans.getRotation();
	glmQuat = glm::quat(btQuat.getX(), btQuat.getY(), btQuat.getZ(), btQuat.getW());
	//glmQuat = glm::quat(btQuat.getX() - btQuatPrev.getX(), btQuat.getY() - btQuatPrev.getY(), btQuat.getZ() - btQuatPrev.getZ(), btQuat.getW() - btQuatPrev.getW());
	glm::vec3 delta_r = r_now - r_prev;



	axis = glm::axis(glmQuat);
	angle = glm::angle(glmQuat);
	mat_model = glm::rotate(mat_model, angle, axis);
	mat_model = glm::translate(mat_model, delta_r);
	
	

	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mat_model));
	model->Draw(*shader);
	r_prev = r_now;
	//btQuatPrev = btQuat;
	mat_model = glm::rotate(mat_model, -angle, axis);
	

}