#include "Floor.h"

#include "stb_image.h"


Floor::Floor() : Length(LENGTH), Width(WIDTH)
{
	Model = glm::mat4(1.0f);
	Model = glm::scale(Model, glm::vec3((float)Length, 0.0f, (float)Width));

	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, VertexCount * 3 * sizeof(float), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, VertexCount * 3 * sizeof(float), Colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, VertexCount * 2 * sizeof(float), TextureVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(int), Indexes, GL_STATIC_DRAW);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Choose texture of floor which may be loaded
	Texture_Number TextureNo = sand;
	switch (TextureNo)
	{
	case sand:
		data = stbi_load("Textures/sand.jpg", &TextureWidth, &TextureHeight, &nrChannels, 0);
		break;
	case sand2:
		data = stbi_load("Textures/gravel.jpg", &TextureWidth, &TextureHeight, &nrChannels, 0);
		break;
	default:
		break;
	}

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
};

Floor::~Floor()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(1, &EBO);
};

void Floor::draw(Shader *sp)
{
	sp->use();
	Model = glm::mat4(1.0f);
	Model = glm::scale(Model, glm::vec3((float)Length, 1.0f, (float)Width));
	sp->setMat4("model", Model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Floor::Vertices[] =
{
	-0.5f, 0.0f, -0.5f,
	-0.5f, 0.0f, 0.5f,
	0.5f, 0.0f, 0.5f,
	0.5f, 0.0f, -0.5f,
};

float Floor::Colors[] =
{
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f , 1.0f, 0.0f,
};

unsigned int Floor::Indexes[] =
{
	0,1,3,	1,2,3
};

float Floor::TextureVertices[] =
{
	/*0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f*/

	0.0f, 0.0f,
	0.0f, 40.0f,
	40.0f, 40.0f,
	40.0f, 0.0f
};

int Floor::VertexCount = VERTEXCOUNT;
int Floor::IndexCount = INDEXCOUNT;