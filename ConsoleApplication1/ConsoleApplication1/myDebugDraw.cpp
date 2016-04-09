#pragma once
#include "stdafx.h"
#include <iostream>

#include "myDebugDraw.h"

myDebugDrawer::myDebugDrawer() : btIDebugDraw()
{
	//setDebugMode(DBG_DrawWireframe);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glBindVertexArray(0);
}

void myDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	//glBegin(GL_LINES);
	//glColor3f(color.getX(), color.getY(), color.getZ());
	//glVertex3f(from.getX(), from.getY(), from.getZ());
	//glVertex3f(to.getX(), to.getY(), to.getZ());
	//glEnd();


	//update array of vertex coords:
	vertices[0] = from.x();
	vertices[1] = from.y();
	vertices[2] = from.z();
	vertices[3] = to.x();
	vertices[4] = to.y();
	vertices[5] = to.z();


	//do some shit with VBO's and shit
	///we need to store an array/vector of floats

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, &vertices[0], GL_STATIC_DRAW);

	///double check online really how to draw just one line
	///ALSO brush up on how glDrawArrays works
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW);
	//glBindVertexArray(0);

}
void myDebugDrawer::drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
	// draws a line between two contact points
	btVector3 const startPoint = pointOnB;
	btVector3 const endPoint = pointOnB + normalOnB * distance;
	drawLine(startPoint, endPoint, color);
	
}

void myDebugDrawer::ToggleDebugFlag(int flag)
{
	if (m_debugMode & flag)
	// flag is enabled, so disable it
		m_debugMode = m_debugMode & (~flag);
	else
	// flag is disabled, so enable it
		m_debugMode |= flag;
}