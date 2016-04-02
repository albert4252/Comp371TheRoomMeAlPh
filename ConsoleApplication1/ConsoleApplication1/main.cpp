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

#include "btBulletDynamicsCommon.h"


//using namespace std;

#define M_PI        3.14159265358979323846264338327950288   /* pi */
#define DEG_TO_RAD	M_PI/180.0f

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

void DrawHere(Model* model, Shader* shader);
void DrawAll();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
bool key1[1024]; //keys that were pressed then released.
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


std::vector<Thing*> thingz;



// The MAIN function, from here we start our application and run our Game loop
int main()
{
	//Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));


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
	Shader shader("model_loading.vs", "model_loading.frag");

	// Load models
	Model ourModel("mesa.obj");

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -200, 0)));




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
		DrawHere(&ourModel, &shader);


		// Clear the colorbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();   // <-- Don't forget this one!
		// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		//glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -50.0f)); // Translate it down a bit so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//ourModel.Draw(shader);
		

		DrawAll();

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

#pragma region "User input"

void DrawHere(Model* model, Shader* shader)
{
	if (key1[GLFW_KEY_ENTER])
	{
		key1[GLFW_KEY_ENTER] = false;
		thingz.push_back(new Thing(model, shader, camera, 150.0f, 0.004));
	}
}

void DrawAll()
{
	for (int i = 0; i < thingz.size(); i++)
	{
		thingz[i]->draw();
	}
}

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