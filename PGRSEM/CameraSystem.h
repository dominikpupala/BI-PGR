//----------------------------------------------------------------------------------------
/**
 * \file       CameraSystem.h
 * \author     Dominik Pupala
 * \date       2021/15/05
 * \brief      Header file for camera system.
 *
 *  Source file containing definitions for CameraSystem object used in application.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <vector>

#include "Camera.h"

/// Struct that handles camera context.
/**
  This struct contains context and functionality of camera context.
*/
struct CameraSystem
{
public:
	bool CanLook = false; ///< Free look attribute
	bool CanMove = false; ///< Free move attribute
	bool IsStatic = true; ///< Static camera validator
	float CurrentTime = 0.0f; ///< Camera runtime value

	///< static cameras
	std::vector<Camera> Cameras
	{
		Camera(glm::vec3(0.0f, 1.0f, 3.0f)),
		Camera(glm::vec3(-26.8f, 0.25f, 6.9f), -386.5f, 9.5f),
		Camera(glm::vec3(-6.9f, 9.27f, -19.3f), -303.2f, -22.75f)
	};
	Camera* Current = &Cameras[0]; ///< Active camera pointer

private:
	size_t _index = 0; ///< Current static camera index

public:
	/// Switch active camera.
	/**
		Switches active camera.

		\param[in] camera	Address of newly activated camera.
	*/
	inline void SwitchTo(Camera* camera)
	{
		Current = camera;
		Cameras =
		{
			Camera(glm::vec3(0.0f, 1.0f, 3.0f)),
			Camera(glm::vec3(-26.8f, 0.25f, 6.9f), -386.5f, 9.5f),
			Camera(glm::vec3(-6.9f, 9.27f, -19.3f), -303.2f, -22.75f)
		}; // reset camera positions
	}
	/// Cycle static cameras.
	/**
		Cycles through static cameras.

		\param[in] offset	Index offset of static cameras.
	*/
	inline void CycleTo(int32_t offset)
	{
		if (offset > 0)
			_index = (_index + offset) % Cameras.size();
		else if (offset < 0)
			_index = (_index + offset + Cameras.size()) % Cameras.size();
		SwitchTo(&Cameras[_index]);
	}
};