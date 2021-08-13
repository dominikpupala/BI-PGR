//----------------------------------------------------------------------------------------
/**
 * \file       Shader.cpp
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Source file for shader.
 *
 *  Source file containing declarations for Shader class that is a wrapper around
 *  OpenGL object.
 *
*/
//----------------------------------------------------------------------------------------

#include "Shader.h"

Shader::Shader(const std::string& vsPath, const std::string& fsPath)
	: _rendererID(pgr::createProgram({ pgr::createShaderFromFile(GL_VERTEX_SHADER, vsPath), pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fsPath) })) { }

Shader::Shader(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath)
	: _rendererID(pgr::createProgram({ pgr::createShaderFromFile(GL_VERTEX_SHADER, vsPath), pgr::createShaderFromFile(GL_GEOMETRY_SHADER, gsPath), pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fsPath) })) { }

Shader::~Shader()
{
	glDeleteProgram(_rendererID);
}

void Shader::Bind() const
{
	glUseProgram(_rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, GLint v0)
{
	glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1f(const std::string& name, GLfloat v0)
{
	glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform3fv(const std::string& name, GLsizei count, const GLfloat* value)
{
	glUniform3fv(GetUniformLocation(name), count, value);
}

void Shader::SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform4fv(const std::string& name, GLsizei count, const GLfloat* value)
{
	glUniform4fv(GetUniformLocation(name), count, value);
}

void Shader::SetUniformMatrix4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix4fv(GetUniformLocation(name), count, transpose, value);
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	if (_uniformCache.find(name) == _uniformCache.end())
		_uniformCache[name] = glGetUniformLocation(_rendererID, name.c_str());

	return _uniformCache[name];
}