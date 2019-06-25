#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float radius) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), Radius(RADIUS)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	Radius = radius;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float radius) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), Radius(RADIUS)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	Radius = radius;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	if (EnableCamera)
	{
		Yaw += xoffset;
		Pitch += yoffset;
	}

	if (constrainPitch)
	{
		if (Pitch >= 89.0f)
			Pitch = 89.0f;
		if (Pitch <= 1.0f)
			Pitch = 1.0f;
		if (Yaw >= 360.0f || Yaw <= -360.f)
			Yaw = 0.0f;
	}


	updateCameraVectors();
	
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	Position.x = Radius * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Position.y = Radius * sin(glm::radians(Pitch));
	Position.z = Radius * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.x = -Position.x / Radius;
	front.y = -Position.y / Radius;
	front.z = -Position.z / Radius;
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

float Camera::getZoom()
{
	return this->Zoom;

}
glm::vec3 Camera::getPosition()
{
	return this->Position;
}
