//----------------------------------------------------------------------------------------
/**
 * \file       ElementBuffer.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for element buffer.
 *
 *  Header file containing definitions for ElementBuffer class that is a wrapper around
 *  OpenGL object.
 * 
*/
//----------------------------------------------------------------------------------------

#pragma once

#include "pgr.h"

/// Class that wraps OpenGL element array buffer object.
/**
  This class contains context and functionality of wrapped 
  OpenGL element array buffer object.
*/
class ElementBuffer
{
private:
	GLuint _rendererID; ///< OpenGL ID handle
	GLuint _count; ///< Count of indices

public:
	/// Constructor
	/**
		Creates the OpenGL element array buffer object 
		with given data and persists its context.

		\param[in] data		Collection of indices.
		\param[in] count	Count of indices in collection.
	*/
	ElementBuffer(const GLuint* data, GLuint count);
	/// Destructor
	/**
		Deletes the OpenGL element array buffer object.
	*/
	~ElementBuffer();
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
	/// Index count getter.
	/**
		Returns count of indices in the buffer.
	*/
	inline GLuint GetCount() const { return _count; }
};

