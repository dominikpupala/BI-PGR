//----------------------------------------------------------------------------------------
/**
 * \file       SpectateParameters.h
 * \author     Dominik Pupala
 * \date       2021/15/05
 * \brief      Header file for spectate parameters.
 *
 *  Source file containing definitions for the spectate camera parameters
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <vector>

#include "pgr.h"

std::vector<glm::vec3> SPECTATE_CONTROL_POINTS
{
	glm::vec3(-17.0634f, 7.93374f, 21.4099f),
	glm::vec3(-7.88448f, 3.76922f, -6.36621f),
	glm::vec3(14.4472f, 5.01844f, -15.4603f),
	glm::vec3(0.850481f, 1.97363f, 9.23971f),
	glm::vec3(-15.5271f, 2.7943f, -10.9226f),
	glm::vec3(-23.5874f, 2.42587f, 7.02977f),
}; ///< Control points for spectate camera

float SPECTATE_SPEED = 0.3f; ///< Camera speed for spectate camera
float SPECTATE_CR_PARAMETER = 0.5f; ///< Catmull-Rom basis matrix parameter
glm::vec3 SPECTATE_ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f); ///< Spectate point of origin