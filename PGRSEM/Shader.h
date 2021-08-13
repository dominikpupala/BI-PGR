//----------------------------------------------------------------------------------------
/**
 * \file       Shader.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for shader.
 *
 *  Header file containing definitions for Shader class that is a wrapper around
 *  PGR framework wrapper around OpenGL object.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <unordered_map>

#include "pgr.h"

/// Class that wraps OpenGL shader object.
/**
  This class contains context and functionality of wrapped
  OpenGL shader object.
*/
class Shader
{
private:
	GLuint _rendererID; ///< OpenGL ID handle

	std::unordered_map<std::string, GLint> _uniformCache; ///< Uniform location cache

public:
	/// Constructor
	/**
		Creates and compiles the OpenGL shader object 
		from the given paths.

		\param[in] vsPath	Filepath to the vertex shader.
		\param[in] fsPath	Filepath to the fragment shader.
	*/
	Shader(const std::string& vsPath, const std::string& fsPath);
	/// Constructor
	/**
		Creates and compiles the OpenGL shader object
		from the given paths.

		\param[in] vsPath	Filepath to the vertex shader.
		\param[in] gsPath	Filepath to the geometry shader.
		\param[in] fsPath	Filepath to the fragment shader.
	*/
	Shader(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath);
	/// Destructor
	/**
		Deletes the OpenGL shader object.
	*/
	~Shader();
	/// Bind shader.
	/**
		Binds the shader to the OpenGL context.
	*/
	void Bind() const;
	/// Unbind shader.
	/**
		Unbinds the shader from the OpenGL context.
	*/
	void Unbind() const;
	/// Uniform attribute value setter.
	/**
		Sets the uniform attribute to given value.

		\param[in] name		Name of the attribute.
		\param[in] v0		The first value.
	*/
	void SetUniform1i(const std::string& name, GLint v0);
	/// Uniform attribute value setter.
	/**
		Sets the uniform attribute to given value.

		\param[in] name		Name of the attribute.
		\param[in] v0		The first value.
	*/
	void SetUniform1f(const std::string& name, GLfloat v0);
	/// Uniform attribute value setter.
	/**
		Sets the uniform attribute to given value.

		\param[in] name		Name of the attribute.
		\param[in] v0		The first value.
		\param[in] v1		The second value.
		\param[in] v2		The third value.
	*/
	void SetUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2);
	/// Uniform attribute value setter.
	/**
		Sets the uniform attribute to given value.

		\param[in] name		Name of the attribute.
		\param[in] count	Number of items in the collection.
		\param[in] value	Collection with data.
	*/
	void SetUniform3fv(const std::string& name, GLsizei count, const GLfloat* value);
	/// Uniform attribute value setter.
	/**
		Sets the uniform attribute to given value.

		\param[in] name		Name of the attribute.
		\param[in] v0		The first value.
		\param[in] v1		The second value.
		\param[in] v2		The third value.
		\param[in] v4		The forth value.
	*/
	void SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	/// Uniform attribute value setter.
	/**
		Sets the uniform attribute to given value.

		\param[in] name		Name of the attribute.
		\param[in] count	Number of items in the collection.
		\param[in] value	Collection with data.
	*/
	void SetUniform4fv(const std::string& name, GLsizei count, const GLfloat* value);
	/// Uniform attribute value setter.
	/**
		Sets the uniform attribute to given value.

		\param[in] name			Name of the attribute.
		\param[in] count		Number of items in the collection.
		\param[in] transpose	Transpose matrix data.
		\param[in] value		Collection with data.
	*/
	void SetUniformMatrix4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat* value);
private:
	/// Inner uniform location getter.
	/**
		Returns the location to the uniform attribute.

		\param[in] name		Name of the attribute.
	*/
	GLint GetUniformLocation(const std::string& name);
};

