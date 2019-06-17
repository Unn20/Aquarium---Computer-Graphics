#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <random>

#include "Shader.h"

extern std::mt19937 gen;

//Maximum area where "renderables" may move
const float MAX_X = 16.0f;
const float MAX_Y = 4.5f;
const float MAX_Z = 16.0f;


class Renderable
{
public:
	glm::mat4 Model;
	float x, y, z;	//position of object
	float rx, ry, rz;	//rotation angle of object
	float sx, sy, sz;	//scale of object

	static float Vertices[];
	static float Colors[];
	static float TextureVertices[];
	static unsigned int Indexes[];

	unsigned char *TextureData;
	int TextureWidth, TextureHeight, nrChannels;
	unsigned int Texture;

public:
	Renderable();
	virtual ~Renderable() = 0;
	virtual void draw(Shader *sp) = 0;
	virtual void behave() = 0;

	template<class T>
		T random(T min, T max);	//Works for both int and float values
};

template<class T>
	T Renderable::random(T min, T max)
{
	using dist = std::conditional_t<
		std::is_integral<T>::value,
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>
	>;
	return dist{ min, max }(gen);
}


class Fish : public Renderable
{
public:
	float Velocity;
	unsigned int VAO, VBO;

	glm::vec3 wantToGo; //for move
	//int rsteps;
	int steps; //for move

	//Fish();
	Fish(std::string _s = "norandom", glm::vec3 initialPosition = glm::vec3(1.0f, 1.0f, 1.0f), 
		glm::vec3 initialRotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 initialScaling = glm::vec3(1.0f, 1.0f, 1.0f));
	~Fish();

	void draw(Shader *sp);
	void behave();
	void move(glm::vec3 coordinates);
};






#endif