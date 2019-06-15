#ifndef FLOOR_H
#define FLOOR_H

#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "stb_image.h"

const int LENGTH = 10;
const int WIDTH = 10;
const unsigned int VERTEXCOUNT = 4;
const unsigned int INDEXCOUNT = 12;
const unsigned int TEXTURESCOUNT = 8;

class Floor
{
private:
	glm::mat4 Model;
	int Length; //X Coordinate
	int Width;	//Z Coordinate
	int VertexCount;
	int IndexCount;
	static float Vertices[];
	static float Colors[];
	static float TextureVertices[];
	static unsigned int Indexes[];
	unsigned int VAO, VBO[3], EBO;

	unsigned char *data;
	int Tex_width, Tex_height, nrChannels;
	unsigned int Texture;

public:
	Floor();	//default constructor
	~Floor();	//default destructor
	void draw(Shader *sp);
};

#endif