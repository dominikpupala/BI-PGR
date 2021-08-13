//----------------------------------------------------------------------------------------
/**
 * \file       VertexArray.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for vertex array.
 *
 *  Header file containing definitions for VertexArray class that is a wrapper around
 *  OpenGL object.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

/// Class that wraps OpenGL vertex array object.
/**
  This class contains context and functionality of wrapped
  OpenGL vertex array object.
*/
class VertexArray
{
private:
	GLuint _rendererID; ///< OpenGL ID handle
	GLuint _offset; ///< Attributes offset

public:
	/// Constructor
	/**
		Creates the OpenGL vertex array object and persists its context.

		\param[in] data		Collection of indices.
		\param[in] count	Count of indices in collection.
	*/
	VertexArray();
	/// Destructor
	/**
		Deletes the OpenGL vertex array object.
	*/
	~VertexArray();
	/// Add buffer.
	/**
		Adds given buffer and its layout to the vertex array context.

		\param[in] vb		Buffer with mesh data.
		\param[in] layout	Buffer layout of mesh data.
	*/
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	/// Bind vertex array.
	/**
		Binds the vertex array to the OpenGL context.
	*/
	void Bind() const;
	/// Unbind vertex array.
	/**
		Unbinds the vertex array from the OpenGL context.
	*/
	void Unbind() const;
};

