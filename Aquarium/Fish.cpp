#include "Renderable.h"
#include "stb_image.h"


std::string my_fishes[15] = {
	"TropicalFish01", "TropicalFish02", "TropicalFish03", "TropicalFish04",
		"TropicalFish05", "TropicalFish06", "TropicalFish07", "TropicalFish08",
		"TropicalFish09", "TropicalFish10", "TropicalFish11", "TropicalFish12",
		"TropicalFish13", "TropicalFish14", "TropicalFish15"};

Fish::Fish(std::string _s, glm::vec3 initialPosition, glm::vec3 initialRotation, glm::vec3 initialScaling)
{
	int number_of_model = random(0,14);
	Model = glm::mat4(1.0f);
	if (_s == "norandom")
	{
		x = initialPosition.x; y = initialPosition.y; z = initialPosition.z;
		rx = initialRotation.x; ry = initialRotation.y, rz = initialRotation.z;
		sx = initialScaling.x; sy = initialScaling.y; sz = initialScaling.z;
		Velocity = 0.01f;
		RotateVelocity = 10.0f;
		
	}
	else if (_s == "random")
	{
		x = random(-MAX_X, MAX_X); y = random(1.0f, MAX_Y); z = random(-MAX_Z, MAX_Z);
		rx = random(0.0f, 360.0f); ry = random(0.0f, 360.0f); rz = 0.0f;
		float scale = random(0.1f, 0.5f);
		sx = scale;  sy = scale; sz = scale;
		Velocity = 0.01f;
		RotateVelocity = 10.0f;
	}
	steps = 0;

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	std::string path_obj = "Models/" + my_fishes[number_of_model] + ".obj";
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
	std::string path_tex = "Models/" + my_fishes[number_of_model] + ".jpg";
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


Fish::~Fish()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);
}

void Fish::draw(Shader *sp)
{
	sp->use();
	this->Model = glm::mat4(1.0f);
	this->Model = glm::scale(this->Model, glm::vec3(sx, sy, sz));
	this->Model = glm::translate(this->Model, glm::vec3(x, y, z));
	this->Model = glm::rotate(this->Model, glm::radians(this->rx), glm::vec3(1.0f, 0.0f, 0.0f));
	this->Model = glm::rotate(this->Model, glm::radians(this->ry), glm::vec3(0.0f, 1.0f, 0.0f));
	this->Model = glm::rotate(this->Model, glm::radians(this->rz), glm::vec3(0.0f, 0.0f, 1.0f));
	sp->setMat4("model", this->Model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, VerticesNumber);
}

void Fish::behave()
{
	if (steps == 0)
	{
		wantToGo = glm::vec3(random(-MAX_X, MAX_X), random(MIN_Y, MAX_Y), random(-MAX_Z, MAX_Z));
		glm::vec3 directionToGo = wantToGo - glm::vec3(x, y, z);
		steps = round(distance(wantToGo, glm::vec3(x, y, z)) / Velocity);

		rx = glm::degrees(AnglesBeetwen(glm::vec3(x, y, z), wantToGo).x);
		ry = glm::degrees(AnglesBeetwen(glm::vec3(x, y, z), wantToGo).y);
	}
	else
	{
		move(wantToGo);
		steps--;
	}
	
}

void Fish::move(glm::vec3 coordinates)
{
	float dist = distance(coordinates, glm::vec3(x, y, z));
	x += (coordinates.x - x) / dist * Velocity;
	y += (coordinates.y - y) / dist * Velocity;
	z += (coordinates.z - z) / dist * Velocity;

}

glm::vec2 Fish::AnglesBeetwen(glm::vec3 v1, glm::vec3 v2)
{
	float distXZ = glm::distance(glm::vec2(v1.x, v1.z), glm::vec2(v2.x, v2.z));
	float dist = glm::distance(v1, v2);
	float rx = glm::acos(distXZ / dist);
	if (v2.y < v1.y)
		rx = -rx;
	float ry = glm::acos((v2.z - v1.z) / glm::distance(glm::vec2(v1.x, v1.z), glm::vec2(v2.x, v2.z)));

	if (v2.x < v1.x)
		ry = -ry;

	return glm::vec2(rx, ry);
}
