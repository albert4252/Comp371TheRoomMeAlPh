#pragma once
#include "stdafx.h"

#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"

#include "LinearMath/btIDebugDraw.h"

class myDebugDrawer : public btIDebugDraw
{
protected:
	GLuint VAO;
	GLuint VBO;
	int m_debugMode;
	GLfloat vertices[6];
public:
	myDebugDrawer();
	virtual void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
	virtual int getDebugMode() const override { return m_debugMode; }
	void ToggleDebugFlag(int flag);
	virtual void  drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;
	virtual void  drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
//unused
	virtual void  reportErrorWarning(const char* warningString) override {}
	virtual void  draw3dText(const btVector3 &location, const char* textString) override {}
};