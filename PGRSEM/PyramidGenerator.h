//----------------------------------------------------------------------------------------
/**
 * \file       PyramidGenerator.h
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Header file for pyramid generator.
 *
 *  Header file containing definitions for PyramidGenerator and PyramidData classes.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <vector>

/// Struct that contains Pyramid render data.
/**
	This struct contains Pyramid data for render.
*/
struct PyramidData
{
	std::vector<float> Vertices; ///< Positions
	std::vector<unsigned int> Triangles; ///< Faces
};

/// Static class that generates pyramid objects.
/**
  This static class procedually generates pyramid objects.
*/
class PyramidGenerator
{
public:
	/// Procedually generates pyramid object.
	/**
		Procedually generates normalized pyramid object based on number of layers.

		\param[in] layers	Number of pyramid layers.
	*/
	static PyramidData Generate(unsigned int layers);

private:
	/// Disabled constructor
	/**
		Constructor created so the default one isn't created.
		This class is meant to be static.
	*/
	PyramidGenerator() { }
	/// Generate vertices.
	/**
		Procedually generates normalized vertex positions based on number of layers.

		\param[in] layers	Number of pyramid layers.
	*/
	static std::vector<float> GenerateVertices(unsigned int layers);
	/// Generate triangles.
	/**
		Procedually generates faces by triangulating vertices.

		\param[in] layers	Number of pyramid layers.
	*/
	static std::vector<unsigned int> GenerateTriangles(unsigned int layers);
};