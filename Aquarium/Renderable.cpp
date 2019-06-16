#include "Renderable.h"



std::mt19937 gen{ std::random_device{}() };

Renderable::Renderable()
{
	this->Model = glm::mat4(1.0f);

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;

	this->rx = 0.0f;
	this->ry = 0.0f;
	this->rz = 0.0f;

	this->sx = 1.0f;
	this->sy = 1.0f;
	this->sz = 1.0f;
};

Renderable::~Renderable()
{
	//
};
