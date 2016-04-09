#pragma once
#include <btBulletDynamicsCommon.h>
#include "Model.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class Ball
{
private:
	GLfloat angle;
	glm::vec3 axis;
	glm::quat glmQuat;
	glm::quat glmQuatPrev;
	btQuaternion btQuat;
	btQuaternion btQuatPrev;
	Model* model;
	Shader* shader;
	glm::vec3 offset;
	glm::vec3 r_prev;
	GLfloat scale;
	btScalar mass;
	btVector3 inertia;
	btDiscreteDynamicsWorld* world;
	GLfloat rad;
	GLfloat speed;
public:
	glm::vec3 transl;
	btRigidBody* fallRigidBody;
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	glm::mat4 mat_model;
	Ball(Model*, Shader*, Camera cam, GLfloat scale, btDiscreteDynamicsWorld* w, GLfloat _rad, GLfloat _speed);//z_diff is how far from camera object is spawned, scale is how much it is scaled
	void draw();
};