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
const float MAX_Y = 6.5f;
const float MAX_Z = 16.0f;


class Renderable
{
protected:
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

	template<class T>
		T random(T min, T max);	//Works for both int and float values

public:
	Renderable();
	virtual ~Renderable() = 0;
	virtual void draw(Shader *sp) = 0;
	virtual void behave() = 0;
	virtual bool loadOBJ(const char * path,
		std::vector < glm::vec3 > & out_vertices,
		std::vector < glm::vec2 > & out_uvs,
		std::vector < glm::vec3 > & out_normals);

	
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
private:
	float Velocity;
	unsigned int VAO, VBO[2];

	glm::vec3 wantToGo; //for move
	//int rsteps;
	int steps; //for move

public:
	Fish(std::string _s = "norandom", glm::vec3 initialPosition = glm::vec3(1.0f, 1.0f, 1.0f), 
		glm::vec3 initialRotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 initialScaling = glm::vec3(1.0f, 1.0f, 1.0f));
	~Fish();
	void draw(Shader *sp);
	void behave();
	void move(glm::vec3 coordinates);
};






#endif