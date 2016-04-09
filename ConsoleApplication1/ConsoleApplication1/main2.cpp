#pragma once

#include "stdafx.h"

#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>

#include "src/SOIL.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Thing.h"
#include "myDebugDraw.h"
#include "RoomThing.h"
#include "Ball.h"

#include "btBulletDynamicsCommon.h"


//using namespace std;

#define M_PI        3.14159265358979323846264338327950288   /* pi */
#define DEG_TO_RAD	M_PI/180.0f

// Properties
GLuint screenWidth = 1400, screenHeight = 800;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

void DrawHere(Model* model, Shader* shader, btDiscreteDynamicsWorld* w);
void DrawAll(btDiscreteDynamicsWorld* world);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
bool key1[1024]; //keys that were pressed then released.
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


std::vector<Thing*> thingz;
std::vector<Ball*> ballz;
std::vector<RoomThing*> furnt;

// boolean draw variables 
bool drawCrate = false;
bool drawNano = false;
bool drawChair = false;
bool drawChest = false;
bool drawShelves = false;
bool drawTable = false;

RoomThing* roor;

btRigidBody* groundRigidBody;
btDefaultMotionState* groundMotionState;
myDebugDrawer* dbugr;


void DrawHere(Model* model, Shader* shader);
void DrawCrate(Model* model, Shader* shader);
void DrawChair(Model* model, Shader* shader);
void DrawNano(Model* model, Shader* shader);
void DrawChest(Model* model, Shader* shader);
void DrawShelves(Model* model, Shader* shader);
void DrawTable(Model* model, Shader* shader);
btDiscreteDynamicsWorld* dynamicsWorld;


//these functions make the skybox go
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(vector<const GLchar*> faces);

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	
	std::cout << "Welcome to the Bullet Physics project! Press Enter to throw Earth Balls. 1 to generate custom made crates. 2 to generate nanosuits. 3 to generate chairs. 4 to generate shelves and 5 for tables.";
	//Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
	

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader shader("../../Source/model_loading.vs", "../../Source/model_loading.frag");
	Shader shader2("../../Source/model_loading2.vs", "../../Source/model_loading2.frag");

	Shader shader3("../../Source/advanced.vs", "../../Source/advanced.frag");
	Shader skyboxShader("../../Source/skybox.vs", "../../Source/skybox.frag");

	// Load models
	Model ourModel("../../Source/theworld.obj"); //ball
	Model crate("../../Source/untitled.obj");
	Model nano("../../Source/nanosuit.obj");
	Model chair("../../Source/Wooden Chair.obj");
	Model chest("../../Source/chest.obj");
	Model shelves("../../Source/Shelves.obj");
	Model table("../../Source/Wooden Table 1.obj");
	

	#pragma region "object_initialization"
	// Set the object data (buffers, vertex attributes)
	GLfloat cubeVertices[] = {
		// Positions          // Texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	GLfloat skyboxVertices[] = {
		// Positions
		-10.0f, 10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f
	};

	for (int i = 0; i < sizeof(skyboxVertices)/sizeof(GLfloat); i++)
	{
		skyboxVertices[i] *= 10;
	}

	//for (int i = 0; i < sizeof(skyboxVertices) / sizeof(GLfloat); i++)
	//{
	//	skyboxVertices[i] *= 1000;
	//}

	// Setup cube VAO
	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Load textures
	GLuint cubeTexture = loadTexture("../../Source/container.jpg");
	#pragma endregion

	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("../../Source/right.jpg");
	faces.push_back("../../Source/left.jpg");
	faces.push_back("../../Source/top.jpg");
	faces.push_back("../../Source/bottom.jpg");
	faces.push_back("../../Source/back2.jpg");
	faces.push_back("../../Source/front2.jpg");
	GLuint cubemapTexture = loadCubemap(faces);

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));///Last argument is point on frame
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0.0, 0));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);

	dynamicsWorld->addRigidBody(groundRigidBody);
	

	dbugr = new myDebugDrawer();
	dbugr->setDebugMode(1);
	dynamicsWorld->setDebugDrawer(dbugr);
	
	
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();
		DrawHere(&ourModel, &shader, dynamicsWorld);

		DrawCrate(&crate, &shader2);
		DrawNano(&nano, &shader);
		DrawChair(&chair, &shader);
		DrawChest(&chest, &shader);
		DrawShelves(&shelves, &shader);
		DrawTable(&table, &shader);


		// Clear the colorbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region skyboxLoop

		// Draw skybox first
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader.Use();

		glm::mat4 sbox_transl;
		sbox_transl = glm::translate(sbox_transl, 0.9f*camera.Position);

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(sbox_transl));

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
		
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);


		//// Cubes
		//glBindVertexArray(cubeVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glUniform1i(glGetUniformLocation(shader3.Program, "texture_diffuse1"), 0);
		//glBindTexture(GL_TEXTURE_2D, cubeTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

#pragma endregion

		shader.Use();   // <-- Don't forget this one!
		//Transformation matrices
		///glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);
		//glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		
		DrawAll(dynamicsWorld);
		dynamicsWorld->debugDrawWorld();
		// Swap the buffers
		glfwSwapBuffers(window);



	}

	glfwTerminate();



	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	return 0;
}


GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


// This function loads a texture from file. Note: texture loading functions like these are usually
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio).
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_C])
		ballz.clear();
	if (keys[GLFW_KEY_R])
		furnt.clear();
	if (keys[GLFW_KEY_1])
		drawCrate = true; 
	if (keys[GLFW_KEY_2])
		drawNano = true; 
	if (keys[GLFW_KEY_3])
		drawChair = true; 
	if (keys[GLFW_KEY_4])
		drawChest = true; 
	if (keys[GLFW_KEY_5])
		drawShelves = true; 
	if (keys[GLFW_KEY_6])
		drawTable = true; 
}

void DrawHere(Model* model, Shader* shader, btDiscreteDynamicsWorld* w)
{
	if (key1[GLFW_KEY_ENTER])
	{
		key1[GLFW_KEY_ENTER] = false;
		ballz.push_back(new Ball(model, shader, camera, 1.0f, w, 0.980785f, 10.0f));
	}
}

void DrawAll(btDiscreteDynamicsWorld* world)
{
	world->stepSimulation(1.0f / 60.0f, (int)1, btScalar(1.) / btScalar(60.));
	//roor->draw();
	for (int i = 0; i < thingz.size(); i++)
	{
		thingz[i]->draw();
	}
	for (int i = 0; i < ballz.size(); i++)
	{
		ballz[i]->draw();
	}
	for (int i = 0; i < furnt.size(); i++)
	{
		furnt[i]->draw();
	}
}

// old coordinates of crate: glm::vec3(-2.53264, -1.178047, -0.909964), glm::vec3(0.884774, 0.821953, 1.090037)
// old subdivided coordinates of crate:  glm::vec3(-1.253264, -0.5858047, -0.459964), glm::vec3(0.444774, 0.411953, 0.54590037)
void DrawCrate(Model* model, Shader* shader)//untitled.obj
{
	if (drawCrate == true)
	{
		furnt.push_back(new RoomThing(model, shader, glm::vec3(-1, -1, -1), glm::vec3(2, 2, 2), glm::vec3(camera.Position + 2.0f*camera.Front), 6.0f, dynamicsWorld));
		//furnt.back()->mat_model = glm::translate(furnt.back()->mat_model, glm::vec3(0.0, 0.0, 0.0));
		drawCrate = false;
	}
}


void DrawNano(Model* model, Shader* shader)
{
	if (drawNano == true)
	{
		furnt.push_back(new RoomThing(model, shader, glm::vec3(-2.532648, -1.178047, -0.909964), glm::vec3(0.884774, 0.821953, 1.090037), glm::vec3(camera.Position + 2.0f*camera.Front), 1.0f, dynamicsWorld));
		drawNano = false;
	}
}

void DrawChair(Model* model, Shader* shader)
{
	if (drawChair == true)
	{
		furnt.push_back(new RoomThing(model, shader, glm::vec3(0.1000002, 0.0, 0.1012184), glm::vec3(44.4, 44.718037, 56.201256), glm::vec3(camera.Position + 2.0f*camera.Front), 0.5f, dynamicsWorld));
		drawChair = false;
	}

}

void DrawChest(Model* model, Shader* shader)
{
	if (drawChest == true)
	{
		furnt.push_back(new RoomThing(model, shader, glm::vec3(0.268990, 1.215846, -1.001014), glm::vec3(0.414259, 0.821953, 1.465490), glm::vec3(camera.Position + 2.0f*camera.Front), 1.0f, dynamicsWorld));
		drawChest = false;
	}

}

void DrawShelves(Model* model, Shader* shader)//fix vertices here, onward
{
	if (drawShelves == true)
	{
		furnt.push_back(new RoomThing(model, shader, glm::vec3(-0.362920, -0.551440, -0.275706), glm::vec3(0.362920, 0.551382, 0.275706), glm::vec3(camera.Position + 1.0f*camera.Front), 20.0f, dynamicsWorld));
		drawShelves = false;
	}
}

void DrawTable(Model* model, Shader* shader)
{
	if (drawTable == true)
	{
		furnt.push_back(new RoomThing(model, shader, glm::vec3(0.00123382, 0.0, 13.274619), glm::vec3(149.99876, 72.3, 89.99848), glm::vec3(camera.Position + 2.0f*camera.Front), 1.0f, dynamicsWorld));
		drawTable = false;
	}
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
		key1[key] = true;
	}
		
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



#pragma endregion



//GLFWwindow* window = 0x00;
//
//GLuint shader_program = 0;
//
//GLuint view_matrix_id = 0;
//GLuint model_matrix_id = 0;
//GLuint proj_matrix_id = 0;
//
//
/////Transformations
//glm::mat4 proj_matrix;
//glm::mat4 view_matrix;
//glm::mat4 model_matrix;
//
//
//GLuint VBO, VAO, EBO;
//
//GLfloat point_size = 3.0f;
//
//// An array of 3 vectors which represents 3 vertices
//static const GLfloat g_vertex_buffer_data[] = {
//	-1.0f, -1.0f, 0.0f,
//	1.0f, -1.0f, 0.0f,
//	0.0f, 1.0f, 0.0f,
//};
//
//
//
//// Initial position : on +Z
//glm::vec3 position = glm::vec3(0, 0, 5);
//// Initial horizontal angle : toward -Z
//float horizontalAngle = 3.14f;
//// Initial vertical angle : none
//float verticalAngle = 0.0f;
//// Initial Field of View
//float initialFoV = 45.0f;
//
//float speed = 3.0f; // 3 units / second
//float mouseSpeed = 0.005f;
//double xpos, ypos;
//int w_width = 800, w_height = 800;
//
//void centerCursor()
//{
//	xpos = w_width / 2;
//	ypos = w_height / 2;
//}
//
//void computeMatricesFromInputs(){
//
//	// glfwGetTime is called only once, the first time this function is called
//	static double lastTime = glfwGetTime();
//
//	// Compute time difference between current and last frame
//	double currentTime = glfwGetTime();
//	float deltaTime = float(currentTime - lastTime);
//
//	// Get mouse position
//	
//	if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
//	{
//		glfwGetCursorPos(window, &xpos, &ypos);
//	}
//
//	// Reset mouse position for next frame
//	glfwSetCursorPos(window, w_width / 2, w_height / 2);
//
//	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED))
//	{
//		centerCursor();
//	}
//	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//	{
//		centerCursor();
//	}
//
//	// Compute new orientation
//	horizontalAngle += mouseSpeed * float(w_width / 2 - xpos);
//	verticalAngle += mouseSpeed * float(w_height / 2 - ypos);
//
//	// Direction : Spherical coordinates to Cartesian coordinates conversion
//	glm::vec3 direction(
//		cos(verticalAngle) * sin(horizontalAngle),
//		sin(verticalAngle),
//		cos(verticalAngle) * cos(horizontalAngle)
//		);
//
//	// Right vector
//	glm::vec3 right = glm::vec3(
//		sin(horizontalAngle - M_PI / 2.0f),
//		0,
//		cos(horizontalAngle - M_PI / 2.0f)
//		);
//
//	// Up vector
//	glm::vec3 up = glm::cross(right, direction);
//
//	// Move forward
//	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
//		position += direction * deltaTime * speed;
//	}
//	// Move backward
//	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
//		position -= direction * deltaTime * speed;
//	}
//	// Strafe right
//	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
//		position += right * deltaTime * speed;
//	}
//	// Strafe left
//	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
//		position -= right * deltaTime * speed;
//	}
//
//	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
//
//	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//	proj_matrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
//	// Camera matrix
//	view_matrix = glm::lookAt(
//		position,           // Camera is here
//		position + direction, // and looks here : at the same position, plus "direction"
//		up                  // Head is up (set to 0,-1,0 to look upside-down)
//		);
//
//	// For the next frame, the "last time" will be "now"
//	lastTime = currentTime;
//}
//
//bool initialize() {
//	/// Initialize GL context and O/S window using the GLFW helper library
//	if (!glfwInit()) {
//		fprintf(stderr, "ERROR: could not start GLFW3\n");
//		return false;
//	}
//
//	/// Create a window of size 640x480 and with title "Lecture 2: First Triangle"
//	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
//	window = glfwCreateWindow(w_width, w_height, "COMP371: Assignment 1", NULL, NULL);
//	if (!window) {
//		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
//		glfwTerminate();
//		return false;
//	}
//
//	int w, h;
//	glfwGetWindowSize(window, &w, &h);
//
//
//	glfwMakeContextCurrent(window);
//
//	/// Initialize GLEW extension handler
//	glewExperimental = GL_TRUE;	///Needed to get the latest version of OpenGL
//	glewInit();
//
//	/// Get the current OpenGL version
//	const GLubyte* renderer = glGetString(GL_RENDERER); /// Get renderer string
//	const GLubyte* version = glGetString(GL_VERSION); /// Version as a string
//	printf("Renderer: %s\n", renderer);
//	printf("OpenGL version supported %s\n", version);
//
//	/// Enable the depth test i.e. draw a pixel if it's closer to the viewer
//	glEnable(GL_DEPTH_TEST); /// Enable depth-testing
//	glDepthFunc(GL_LESS);	/// The type of testing i.e. a smaller value as "closer"
//
//	return true;
//}
//
//bool cleanUp() {
//	glDisableVertexAttribArray(0);
//	//Properly de-allocate all resources once they've outlived their purpose
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
//
//	// Close GL context and any other GLFW resources
//	glfwTerminate();
//
//	return true;
//}
//
//GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path)	{
//	// Create the shaders
//	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
//	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
//
//	// Read the Vertex Shader code from the file
//	std::string VertexShaderCode;
//	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
//	if (VertexShaderStream.is_open()) {
//		std::string Line = "";
//		while (getline(VertexShaderStream, Line))
//			VertexShaderCode += "\n" + Line;
//		VertexShaderStream.close();
//	}
//	else {
//		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
//		getchar();
//		exit(-1);
//	}
//
//	// Read the Fragment Shader code from the file
//	std::string FragmentShaderCode;
//	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
//	if (FragmentShaderStream.is_open()) {
//		std::string Line = "";
//		while (getline(FragmentShaderStream, Line))
//			FragmentShaderCode += "\n" + Line;
//		FragmentShaderStream.close();
//	}
//
//	GLint Result = GL_FALSE;
//	int InfoLogLength;
//
//	// Compile Vertex Shader
//	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
//	char const * VertexSourcePointer = VertexShaderCode.c_str();
//	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
//	glCompileShader(VertexShaderID);
//
//	// Check Vertex Shader
//	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
//	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//	if (InfoLogLength > 0) {
//		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
//		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
//		printf("%s\n", &VertexShaderErrorMessage[0]);
//	}
//
//	// Compile Fragment Shader
//	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
//	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
//	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
//	glCompileShader(FragmentShaderID);
//
//	// Check Fragment Shader
//	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
//	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//	if (InfoLogLength > 0) {
//		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
//		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
//		printf("%s\n", &FragmentShaderErrorMessage[0]);
//	}
//
//	// Link the program
//	printf("Linking program\n");
//	GLuint ProgramID = glCreateProgram();
//	glAttachShader(ProgramID, VertexShaderID);
//	glAttachShader(ProgramID, FragmentShaderID);
//
//	glBindAttribLocation(ProgramID, 0, "in_Position");
//
//	//appearing in the vertex shader.
//	glBindAttribLocation(ProgramID, 1, "in_Color");
//
//	glLinkProgram(ProgramID);
//
//	// Check the program
//	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
//	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//	if (InfoLogLength > 0) {
//		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
//		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
//		printf("%s\n", &ProgramErrorMessage[0]);
//	}
//
//	glDeleteShader(VertexShaderID);
//	glDeleteShader(FragmentShaderID);
//
//	//The three variables below hold the id of each of the variables in the shader
//	//If you read the vertex shader file you'll see that the same variable names are used.
//	view_matrix_id = glGetUniformLocation(ProgramID, "view_matrix");
//	model_matrix_id = glGetUniformLocation(ProgramID, "model_matrix");
//	proj_matrix_id = glGetUniformLocation(ProgramID, "proj_matrix");
//
//	return ProgramID;
//}
//
//
//int main() {
//
//	initialize();
//
//	///Load the shaders
//	shader_program = loadShaders("../Source/COMP371_hw1.vs", "../Source/COMP371_hw1.fss");
//
//	// This will identify our vertex buffer
//	GLuint vertexbuffer;
//
//	// Generate 1 buffer, put the resulting identifier in vertexbuffer
//	glGenBuffers(1, &vertexbuffer);
//
//	// The following commands will talk about our 'vertexbuffer' buffer
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	// Give our vertices to OpenGL.
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(
//		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//		3,                  // size
//		GL_FLOAT,           // type
//		GL_FALSE,           // normalized?
//		0,                  // stride
//		(void*)0            // array buffer offset
//		);
//
//
//	glfwSetCursorPos(window, w_width / 2, w_height / 2);
//
//	while (!glfwWindowShouldClose(window)) {
//		// wipe the drawing surface clear
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
//		glPointSize(point_size);
//
//		glUseProgram(shader_program);
//
//		computeMatricesFromInputs();
//
//		model_matrix = glm::mat4(1.0f);
//
//		//Pass the values of the three matrices to the shaders
//		glUniformMatrix4fv(proj_matrix_id, 1, GL_FALSE, glm::value_ptr(proj_matrix));
//		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));
//		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, glm::value_ptr(model_matrix));
//
//		glBindVertexArray(VAO);
//		// Draw the triangle !
//		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//
//		glBindVertexArray(0);
//
//		// update other events like input handling
//		glfwPollEvents();
//		// put the stuff we've been drawing onto the display
//		glfwSwapBuffers(window);
//	}
//
//	cleanUp();
//	return 0;
//}