#pragma once
#include <btBulletDynamicsCommon.h>
#include "Model.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class RoomThing
{
private:
	GLfloat angle;
	glm::vec3 axis;
	glm::quat glmQuat;
	btQuaternion btQuat;
	Shader* shader;
	glm::vec3 offset;
	glm::vec3 r_prev;
	GLfloat scale;
	btScalar mass;
	btVector3 inertia;
	btDiscreteDynamicsWorld* world;
	glm::vec3 vtx_tlf;
	glm::vec3 vtx_brn;
public:
	Model* model;
	glm::vec3 transl;
	btRigidBody* fallRigidBody;
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	glm::mat4 mat_model;
	RoomThing(Model*, Shader*, glm::vec3 _vtx_tlf, glm::vec3 _vtx_brn, glm::vec3 center, GLfloat scale, btDiscreteDynamicsWorld* w);//z_diff is how far from camera object is spawned, scale is how much it is scaled
	void draw();
};