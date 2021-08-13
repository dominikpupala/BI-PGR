//----------------------------------------------------------------------------------------
/**
 * \file       VertexBuffer.cpp
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Source file for vertex buffer.
 *
 *  Header file containing declarations for VertexBuffer class that is a wrapper around
 *  OpenGL object.
 *
*/
//----------------------------------------------------------------------------------------

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size)
{
	glGenBuffers(1, &_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_rendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
