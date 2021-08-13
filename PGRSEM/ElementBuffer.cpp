//----------------------------------------------------------------------------------------
/**
 * \file       ElementBuffer.cpp
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Source file for element buffer.
 *
 *  Header file containing declarations for ElementBuffer class that is a wrapper around
 *  OpenGL object.
 *
*/
//----------------------------------------------------------------------------------------

#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const GLuint* data, GLuint count) 
	: _count(count)
{
	glGenBuffers(1, &_rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &_rendererID);
}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
}

void ElementBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
