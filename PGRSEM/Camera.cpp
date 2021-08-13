//----------------------------------------------------------------------------------------
/**
 * \file       Camera.cpp
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Source file for camera.
 *
 *  Source file containing declarations for Camera classes.
 *
*/
//----------------------------------------------------------------------------------------

#include "Camera.h"

Camera::Camera(const glm::vec3& position, float yaw, float pitch, const glm::vec3& front, const glm::vec3& vertical)
	: _yaw(yaw), _pitch(pitch), _front(front), _position(position), _vertical(vertical)
{
	if (fabs(_pitch) >= _PITCH_BOUND) _pitch = _PITCH;
	Update();
}

void Camera::ProcessPosition(Camera::Movement direction, float timeDelta)
{
	switch (direction)
	{
	case Camera::Movement::FORWARD:
		_position += _front * _SPEED * timeDelta;
		break;
	case Camera::Movement::BACKWARD:
		_position -= _front * _SPEED * timeDelta;
		break;
	case Camera::Movement::LEFT:
		_position -= _right * _SPEED * timeDelta;
		break;
	case Camera::Movement::RIGHT:
		_position += _right * _SPEED * timeDelta;
		break;
	case Camera::Movement::UPWARD:
		_position += _vertical * _SPEED * timeDelta;
		break;
	case Camera::Movement::DOWNWARD:
		_position -= _vertical * _SPEED * timeDelta;
		break;
	default:
		break;
	}
}

void Camera::ProcessMovement(float x, float y)
{
	_yaw += x * _SENSITIVITY;

	if (fabs(_pitch + y * _SENSITIVITY) < _PITCH_BOUND)
		_pitch += y * _SENSITIVITY;

	Update();
}

void Camera::SetPosition(const glm::vec3& position)
{
	_position = position;
}

void Camera::SetDirection(const glm::vec3& direction)
{
	_front = glm::normalize(direction);
	_right = glm::normalize(glm::cross(_front, _vertical));
	_up = glm::normalize(glm::cross(_right, _front));
}

void Camera::Update()
{
	_front = glm::normalize(glm::vec3(
		cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
		sin(glm::radians(_pitch)),
		sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
	));
	_right = glm::normalize(glm::cross(_front, _vertical));
	_up = glm::normalize(glm::cross(_right, _front));
}
