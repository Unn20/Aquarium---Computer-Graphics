#include "Lights.h"


DirLight::DirLight()
{
	ambient = glm::vec3(31.0f, 181.0f, 198.0f) * glm::vec3(1.0f/255.0f, 1.0f/255.0f, 1.0f/255.0f);
	diffuse = glm::vec3(13.0f, 56.0f, 71.0f) * glm::vec3(1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f);
	specular = glm::vec3(48.0f, 88.0f, 102.0f) * glm::vec3(1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f);

	direction = glm::vec3(0.0f, -10.0f, 0.2f);
}

DirLight::~DirLight()
{
	//
}

void DirLight::apply(Shader *sp)
{
	sp->setVec3("dirLight.direction", direction);
	sp->setVec3("dirLight.ambient", ambient);
	sp->setVec3("dirLight.diffuse", diffuse);
	sp->setVec3("dirLight.specular", specular);
}

//
//

SpotLight::SpotLight()
{
	position = glm::vec3(0.0f, 10.0f, 0.0f);
	direction = glm::vec3(0.0f, -1.0f, 0.0f);

	cutOff = glm::cos(glm::radians(10.5f));
	outerCutOff = glm::cos(glm::radians(12.0f));

	ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);

	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032;
}

SpotLight::~SpotLight()
{
	//
}

void SpotLight::apply(Shader *sp)
{
	sp->setVec3("spotLight.position", position);
	sp->setVec3("spotLight.direction", direction);
	sp->setVec3("spotLight.ambient", ambient);
	sp->setVec3("spotLight.diffuse", diffuse);
	sp->setVec3("spotLight.specular", specular);

	sp->setFloat("spotLight.cutOff", cutOff);
	sp->setFloat("spotLight.outerCutOff", outerCutOff);
	sp->setFloat("spotLight.constant", constant);
	sp->setFloat("spotLight.linear", linear);
	sp->setFloat("spotLight.quadratic", quadratic);
}