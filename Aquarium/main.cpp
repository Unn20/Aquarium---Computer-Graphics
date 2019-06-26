/*	Poznan University of Technology, 26.06.2019
	Authors:
	Maciej Leszczyk
	Beata Baczyńska		*/

#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "Lights.h"
#include "Shader.h"
#include "Camera.h"
#include "Floor.h"
#include "Renderable.h"
#include "stb_image.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const int FRAMES_PER_SECOND = 120;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
glm::vec3 CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void mouseCallback(GLFWwindow *window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

Camera camera(glm::vec3(0.0f, 0.1f, 3.0f));
Shader *lightPosShader, *dShader;
Floor *podloga_w;
Renderable *object;
std::vector<Renderable*> table;
DirLight *lightDir;
SpotLight *lightSpot;

int fishNum = 100;
int rockNum = 20;

int objNum = fishNum + rockNum;


bool enableF = 1, enableD = 1, enableS = 1;
float KEY_PRESS = 0.0;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



void initOpenGLProgram(GLFWwindow *window)
{
	glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // tell GLFW to capture our mouse
}

void freeOpenGLProgram(GLFWwindow *window)
{
	glfwDestroyWindow(window);
	for (int i = 0; i < objNum; i++)
	{ 
		Renderable *r = table.back();
		table.pop_back();
		delete r;
	}
}

void drawScene(GLFWwindow *window)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dShader->use();
	dShader->setVec3("viewPos", camera.getPosition());
	dShader->setFloat("material.shininess", 32.0f);

	lightDir->apply(dShader);
	lightSpot->apply(dShader);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	dShader->setMat4("view", view);
	dShader->setMat4("projection", projection);
	dShader->setMat4("model", model);
	dShader->setVec3("FogColor", CLEAR_COLOR);

	dShader->setBool("enableFog", enableF);
	dShader->setBool("enableDirLight", enableD);
	dShader->setBool("enableSpotLight", enableS);

	podloga_w->draw(dShader);

	for (int i = 0; i < objNum; i++)
	{
		object = table[i];
		object->behave();
		object->draw(dShader);
	}
	// creating a treasure 
	object = table[objNum];
	object->draw(dShader);
	glfwSwapBuffers(window);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Aquarium", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	initOpenGLProgram(window);

	Shader lightShader("normal.vs", "normal.fs");
	Shader defaultShader("lightEffects.vs", "lightEffects.fs");
	lightPosShader = &lightShader;
	dShader = &defaultShader;

	Floor podloga;
	podloga_w = &podloga;

	DirLight light123;
	lightDir = &light123;

	SpotLight light321;
	lightSpot = &light321;


	for (int i = 0; i < fishNum; i++)
	{
		Fish *rryba = new Fish("random");
		table.push_back(rryba);
	}

	for (int i = 0; i < rockNum; i++)
	{
		Rock *sskala = new Rock();
		table.push_back(sskala);
	}

	Other *treasure = new Other("treasure", glm::vec3(1.0f, 1.0f, 1.0f));
	table.push_back(treasure);

	DWORD next_game_tick = GetTickCount();
	int sleep_time = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		drawScene(window);

		glfwPollEvents();

		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - GetTickCount();
		if (sleep_time >= 0) 
		{
			Sleep(sleep_time);
		}
	}
	freeOpenGLProgram(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetTime() - KEY_PRESS > 0.35)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		{
			enableF = !enableF; KEY_PRESS = glfwGetTime();
		}
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		{
			enableS = !enableS; KEY_PRESS = glfwGetTime();
		}
		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		{
			enableD = !enableD; KEY_PRESS = glfwGetTime();
		}
	}

		//for lightning debug
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			lightSpot->position.y += 0.1f;
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			lightSpot->position.y -= 0.1f;
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			lightSpot->cutOff += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			lightSpot->cutOff -= 0.01f;
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			lightSpot->outerCutOff += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			lightSpot->outerCutOff -= 0.01f;

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			lightDir->ambient.r += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			lightDir->ambient.r -= 0.01f;
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			lightDir->ambient.g += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			lightDir->ambient.g -= 0.01f;
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			lightDir->ambient.b += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			lightDir->ambient.b -= 0.01f;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse )
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		camera.EnableCamera = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		camera.EnableCamera = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
