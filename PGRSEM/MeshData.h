//----------------------------------------------------------------------------------------
/**
 * \file       MeshData.h
 * \author     Dominik Pupala
 * \date       2021/06/05
 * \brief      Header file for superobject.
 *
 *  Source file containing definitions for meshdata superobject.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

/// Struct that wraps basic object attributes.
/**
  This struct contains data context of basic object.
*/
struct MeshData
{
	// OpenGL context
	VertexArray* VAO;
	VertexBuffer* VB;
	ElementBuffer* EB;
	VertexBufferLayout* VBL;

	// Material context
	glm::vec3 Diffuse = glm::vec3(0.0f);
	glm::vec3 Ambient = glm::vec3(0.0f);
	glm::vec3 Specular = glm::vec3(0.0f);

	GLfloat Shininess;
	GLuint Texture;

	/// Destructor
	/**
		Deletes allocated memory.
	*/
	inline ~MeshData()
	{
		delete VAO;
		delete VB;
		delete EB;
		delete VBL;
	}
};