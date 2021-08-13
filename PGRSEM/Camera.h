//----------------------------------------------------------------------------------------
/**
 * \file       Camera.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for camera.
 *
 *  Header file containing definitions for Camera class.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <iostream>

#include "pgr.h"

/// Class that handles camera object.
/**
  This class contains context and functionality of camera object.
*/
class Camera
{
public:
	/// Enum class that encapsulates states of the camer movement.
	/**
	  This enum class contains definitions of possible stats of the camera movement.
	*/
	enum class Movement { FORWARD, BACKWARD, LEFT, RIGHT, UPWARD, DOWNWARD };

private:
	static constexpr float _YAW = -90.0f; ///< Implicit yaw
	static constexpr float _PITCH = 0.0f; ///< Implicit pitch
	static constexpr float _SPEED = 5.0f; ///< Implicit speed
	static constexpr float _SENSITIVITY = 0.05f; ///< Implicit sensitivity
	static constexpr float _PITCH_BOUND = 90.0f; ///< Implicit pitch limit

	float _yaw; ///< Horizontal axis angle
	float _pitch; ///< Vertical axis angle

	glm::vec3 _up; ///< Up vector (local)
	glm::vec3 _right; ///< Right vector (local)
	glm::vec3 _front; ///< Front vector (local)
	glm::vec3 _position; ///< Position of camera (global)
	glm::vec3 _vertical; ///< Up vector (global)

public:
	/// Constructor
	/**
		Creates new camera object with given settings.

		\param[in] position		Position of the camera.
		\param[in] yaw			Horizontal axis angle.
		\param[in] pitch		Vertical axis angle.
		\param[in] front		Front vector (local).
		\param[in] vertical		Vertical vector (global).
	*/
	Camera(const glm::vec3& position, float yaw = _YAW, float pitch = _PITCH, const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& vertical = glm::vec3(0.0f, 1.0f, 0.0f));
	/// Process camera change in position.
	/**
		Updates camera position based on direction and elapsed time.

		\param[in] direction	Direction of the camera movement.
		\param[in] timeDelta	Elapsed time to be updated.
	*/
	void ProcessPosition(Camera::Movement direction, float timeDelta);
	/// Process camera look movement.
	/**
		Updates yaw and pitch based on x and y position.

		\param[in] x	Mouse position on x axis.
		\param[in] y	Mouse position on y axis.
	*/
	void ProcessMovement(float x, float y);
	/// Position setter.
	/**
		Updates position directly.

		\param[in] position		Position of the camera.
	*/
	void SetPosition(const glm::vec3& position);
	/// Direction setter.
	/**
		Updates direction directly.

		\param[in] direction	Direction of the camera.
	*/
	void SetDirection(const glm::vec3& direction);
	/// Position getter.
	/**
		Returns position of the camera.
	*/
	inline glm::vec3 GetPosition() const { return _position; }
	/// View matrix getter.
	/**
		Returns view matrix of the camera.
	*/
	inline glm::mat4 GetViewMatrix() const { return glm::lookAt(_position, _position + _front, _up); }
	/// Debug method.
	/**
		Prints out current position and yaw with pitch.
	*/
	inline void Print() const { std::cout << _position.x << ' ' << _position.y << ' ' << _position.z << std::endl; std::cout << _yaw << ' ' << _pitch << std::endl; }

private:
	/// Updates camera vectors.
	/**
		Calculates camera vectors and normalize them.
	*/
	void Update();
};

