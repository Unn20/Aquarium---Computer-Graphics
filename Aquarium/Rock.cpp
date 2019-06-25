#include "Renderable.h"
#include "stb_image.h"

Rock::Rock()
{
	Model = glm::mat4(1.0f);

	float Y = random(0.1f, 0.4f);

	x = random(-10.0f, 10.0f); y = Y/3; z = random(-10.0f, 10.0f);
	sx = random(0.1f, 0.5f);  sy =Y; sz = random(0.1f, 0.5f);

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	std::string path_obj = "Models/Rock01.obj";
	bool res = loadOBJ(path_obj.c_str(), vertices, uvs, normals);

	VerticesNumber = vertices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int textureNo = random(0, 1);
	std::string path_tex;
	if (textureNo == 0)
		path_tex = "Models/Rock01.jpg";
	else if (textureNo == 1)
		path_tex = "Models/Rock02.jpg";
	TextureData = stbi_load(path_tex.c_str(), &TextureWidth, &TextureHeight, &nrChannels, 0);

	if (TextureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(TextureData);
}


Rock::~Rock()
{
	//
}

void Rock::behave()
{
	//just be lol
}

void Rock::draw(Shader *sp)
{
	sp->use();
	this->Model = glm::mat4(1.0f);
	this->Model = glm::translate(this->Model, glm::vec3(x, y, z));
	this->Model = glm::scale(this->Model, glm::vec3(sx, sy, sz));
	sp->setMat4("model", this->Model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, VerticesNumber);
}