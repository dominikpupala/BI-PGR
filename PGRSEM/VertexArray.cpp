//----------------------------------------------------------------------------------------
/**
 * \file       VertexArray.cpp
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Source file for vertex array.
 *
 *  Header file containing declarations for VertexArray class that is a wrapper around
 *  OpenGL object.
 *
*/
//----------------------------------------------------------------------------------------

#include "VertexArray.h"

VertexArray::VertexArray() 
	: _offset(0)
{
	glGenVertexArrays(1, &_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();

	for (size_t i = 0, offset = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(_offset + i);
		glVertexAttribPointer(_offset + i, element.Count, element.Type, element.Normalized, layout.GetStride(), (const GLvoid*)offset);
		offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
	}

	_offset += elements.size();
}

void VertexArray::Bind() const
{
	glBindVertexArray(_rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
