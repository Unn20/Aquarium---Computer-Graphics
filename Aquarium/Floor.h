#ifndef FLOOR_H
#define FLOOR_H

#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

const int LENGTH = 17;	//Length of floor object
const int WIDTH = 17;	//Width of floor object

class Floor
{
private:
	glm::mat4 Model;	//Model matrix
	int Length; //X Coordinate
	int Width;	//Z Coordinate
	static float Vertices[];
	static float Normals[];
	static float TextureVertices[];
	static unsigned int Indexes[];

	unsigned int VAO, VBO[3], EBO;

	unsigned char *data;
	int TextureWidth, TextureHeight, nrChannels;
	unsigned int Texture;

	enum Texture_Number
	{
		sand,
		sand2
	};

public:
	Floor();	//default constructor
	~Floor();	//default destructor
	void draw(Shader *sp);	//draw Floor in loop function
};

#endif