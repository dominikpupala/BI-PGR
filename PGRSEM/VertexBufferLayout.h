//----------------------------------------------------------------------------------------
/**
 * \file       VertexBufferLayout.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for element buffer.
 *
 *  Header file containing definitions and declarations for VertexBufferLayout and 
 *  VertexBufferElement classes that provides context for the VertexBuffer class.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <vector>

#include "pgr.h"

/// Struct that contains Vertex buffer layout data.
/**
	This struct contains data for Vertex buffer layout data.
*/
struct VertexBufferElement
{
	GLuint Count; ///< Number of vertices
	GLenum Type; ///< Datatype of vertices
	GLboolean Normalized; ///< Normalized vertices

	/// Datatype getter.
	/**
		Return size of given datatype.

		\param[in] type		Datatype to get size of.
	*/
	static GLuint GetSizeOfType(GLuint type)
	{
		switch (type)
		{
		case GL_FLOAT: return sizeof(GLfloat);
		case GL_UNSIGNED_INT: return sizeof(GLuint);
		case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		}

		return 0;
	}
};

/// Class that handles layout context for Vertex buffer object.
/**
  This class handles context and functionality of layout for
  Vertex buffer object.
*/
class VertexBufferLayout
{
private:
	GLsizei _stride; ///< Stride offset
	std::vector<VertexBufferElement> _elements; ///< Attribute collection

public:
	/// Constructor
	/**
		Creates empty context for layouts.
	*/
	VertexBufferLayout()
		: _stride(0) { }
	/// Push attribute.
	/**
		Pushes attribute of given count to the layout.

		\param[in] count	Number of vertices.
	*/
	template<typename T>
	void Push(GLuint count)
	{
		static_assert(false);
	}
	template<>
	void Push<GLfloat>(GLuint count)
	{
		_elements.push_back({ count, GL_FLOAT, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void Push<GLuint>(GLuint count)
	{
		_elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void Push<GLubyte>(GLuint count)
	{
		_elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
	/// Stride getter.
	/**
		Returns stride offset of the layout.
	*/
	inline GLuint GetStride() const { return _stride; }
	/// Elements getter.
	/**
		Returns elements of atrribute layout.
	*/
	inline const std::vector<VertexBufferElement>& GetElements() const { return _elements; }
};

