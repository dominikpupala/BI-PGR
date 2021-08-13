//----------------------------------------------------------------------------------------
/**
 * \file       Renderer.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for OpenGL renderer.
 *
 *  Header file containing definitions for Renderer class.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "ElementBuffer.h"

/// Class that handles OpenGL renderer.
/**
  This class contains context and functionality of OpenGL renderer.
*/
class Renderer
{
public:
	/// Draw data.
	/**
		Draws given data. 

		\param[in] va		Object data.
		\param[in] eb		Object face indices.
		\param[in] shader	Shader program.
		\param[in] mode		Drawing mode.
	*/
	void Draw(const VertexArray& va, const ElementBuffer& eb, const Shader& shader, const GLenum& mode = GL_TRIANGLES) const;
	/// Clear screen.
	/**
		Clears screen.
	*/
	void Clear() const;
	/// Initialize screen.
	/**
		Initializes screen with given color and executes given function.

		\param[in] color	Underlying color.
		\param[in] func		Additional operations.
	*/
	static void Initialize(const glm::vec4& color, void(*func)());
	/// Set screen viewport.
	/**
		Sets screen viewport.

		\param[in] x	X position.
		\param[in] y	Y position.
		\param[in] w	Width.
		\param[in] H	Height.
	*/
	static void SetViewport(GLint x, GLint y, GLsizei w, GLsizei h);
};

