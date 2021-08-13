//----------------------------------------------------------------------------------------
/**
 * \file       VertexBuffer.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for element buffer.
 *
 *  Header file containing definitions for VertexBufer class that is a wrapper around
 *  OpenGL object.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include "pgr.h"

/// Class that wraps OpenGL element vertex buffer object.
/**
  This class contains context and functionality of wrapped
  OpenGL vertex buffer object.
*/
class VertexBuffer
{
private:
	GLuint _rendererID; ///< OpenGL ID handle

public:
	/// Constructor
	/**
		Creates the OpenGL vertex buffer object
		with given data and persists its context.

		\param[in] data		Collection of vertices.
		\param[in] size		Size of the collection.
	*/
	VertexBuffer(const GLvoid* data, GLsizeiptr size);
	/// Destructor
	/**
		Deletes the OpenGL vertex buffer object.
	*/
	~VertexBuffer();
	/// Bind buffer.
	/**
		Binds the buffer to the OpenGL context.
	*/
	void Bind() const;
	/// Unbind buffer.
	/**
		Unbinds the buffer from the OpenGL context.
	*/
	void Unbind() const;
};

