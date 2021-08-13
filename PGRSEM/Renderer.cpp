//----------------------------------------------------------------------------------------
/**
 * \file       Renderer.cpp
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Source file for OpenGL renderer.
 *
 *  Source file containing declarations for Renderer class.
 *
*/
//----------------------------------------------------------------------------------------

#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const ElementBuffer& eb, const Shader& shader, const GLenum& mode) const
{
	shader.Bind();
	va.Bind();
	eb.Bind();

	glDrawElements(mode, eb.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::Initialize(const glm::vec4& color, void (*func)())
{
	glClearColor(color.r, color.g, color.b, color.a);
	(*func)();
}

void Renderer::SetViewport(GLint x, GLint y, GLsizei w, GLsizei h)
{
	glViewport(x, y, w, h);
}
