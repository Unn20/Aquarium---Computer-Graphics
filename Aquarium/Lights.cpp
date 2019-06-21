#include "Lights.h"


DirLight::DirLight()
{
	ambient = glm::vec3(0.2f, 1.0f, 0.3f);;
	diffuse = glm::vec3(0.5f, 0.0f, 0.4f);;
	specular = glm::vec3(0.9f, 0.5f, 0.0f);;

	direction = glm::vec3(-0.2f, -1.0f, -0.3f);
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
	position = glm::vec3(0.0f, 20.0f, 0.0f);
	direction = glm::vec3(0.0f, -1.0f, 0.0f);

	cutOff = glm::cos(glm::radians(10.5f));
	outerCutOff = glm::cos(glm::radians(12.0f));

	ambient = glm::vec3(0.0f, 0.0f, 0.0f);
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
	sp->setVec3("spotLight.position", direction);
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