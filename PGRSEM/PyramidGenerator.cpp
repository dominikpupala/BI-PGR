//----------------------------------------------------------------------------------------
/**
 * \file       PyramidGenerator.cpp
 * \author     Dominik Pupala
 * \date       2021/07/03
 * \brief      Source file for pyramid generator.
 *
 *  Source file containing declarations for PyramidGenerator class.
 *
*/
//----------------------------------------------------------------------------------------

#include "PyramidGenerator.h"

PyramidData PyramidGenerator::Generate(unsigned int layers)
{
	return { GenerateVertices(layers), GenerateTriangles(layers) };
}

std::vector<float> PyramidGenerator::GenerateVertices(unsigned int layers)
{
	std::vector<float> temp;

	/// step width/offset
	float width = 2.0f / ((layers * 2) - 1);
	/// height of step
	float height = 2.0f / ((layers * 2) - 1);

	// each layer is rectangle - eight vertices, each consists of three floats
	float offsetW, offsetH;
	for (size_t i = 0; i < layers; ++i)
	{
		offsetW = i * width;
		offsetH = i * height;
		temp.insert(temp.end(),
			{
				// 0														// 0
				-1.0f + offsetW, -1.0f + offsetH, -1.0f + offsetW,			 0.0f,  (i) ? 1.0f : -1.0f,  0.0f,	// 0  - lower face
				-1.0f + offsetW, -1.0f + offsetH, -1.0f + offsetW,			 0.0f,  0.0f, -1.0f,				// 1  - back face
				-1.0f + offsetW, -1.0f + offsetH, -1.0f + offsetW,			-1.0f,  0.0f,  0.0f,				// 2  - left face
				// 1														// 1
				-1.0f + offsetW, -1.0f + offsetH,  1.0f - offsetW,			 0.0f,  (i) ? 1.0f : -1.0f,  0.0f,	// 3  - lower face
				-1.0f + offsetW, -1.0f + offsetH,  1.0f - offsetW,			-1.0f,  0.0f,  0.0f,				// 4  - left face
				-1.0f + offsetW, -1.0f + offsetH,  1.0f - offsetW,			 0.0f,  0.0f,  1.0f,				// 5  - front face
				// 2														// 2
				-1.0f + offsetW, -1.0f + height + offsetH, -1.0f + offsetW,	 0.0f,  0.0f, -1.0f,				// 6  - back face
				-1.0f + offsetW, -1.0f + height + offsetH, -1.0f + offsetW,	-1.0f,  0.0f,  0.0f,				// 7  - left face
				-1.0f + offsetW, -1.0f + height + offsetH, -1.0f + offsetW,	 0.0f,  1.0f,  0.0f,				// 8  - upper face
				// 3														// 3
				-1.0f + offsetW, -1.0f + height + offsetH,  1.0f - offsetW,	-1.0f,  0.0f,  0.0f,				// 9  - left face
				-1.0f + offsetW, -1.0f + height + offsetH,  1.0f - offsetW,	 0.0f,  0.0f,  1.0f,				// 10 - front face
				-1.0f + offsetW, -1.0f + height + offsetH,  1.0f - offsetW,	 0.0f,  1.0f,  0.0f,				// 11 - upper face
				// 4														// 4
				 1.0f - offsetW, -1.0f + offsetH, -1.0f + offsetW,			 0.0f,  (i) ? 1.0f : -1.0f,  0.0f,	// 12 - lower face
				 1.0f - offsetW, -1.0f + offsetH, -1.0f + offsetW,			 0.0f,  0.0f, -1.0f,				// 13 - back face
				 1.0f - offsetW, -1.0f + offsetH, -1.0f + offsetW,			 1.0f,  0.0f,  0.0f,				// 14 - right face
				// 5														// 5
				 1.0f - offsetW, -1.0f + offsetH,  1.0f - offsetW,			 0.0f,  (i) ? 1.0f : -1.0f,  0.0f,	// 15 - lower face
				 1.0f - offsetW, -1.0f + offsetH,  1.0f - offsetW,			 0.0f,  0.0f,  1.0f,				// 16 - front face
				 1.0f - offsetW, -1.0f + offsetH,  1.0f - offsetW,			 1.0f,  0.0f,  0.0f,				// 17 - right face
				// 6														// 6
				 1.0f - offsetW, -1.0f + height + offsetH, -1.0f + offsetW,	 0.0f,  0.0f, -1.0f,				// 18 - back face
				 1.0f - offsetW, -1.0f + height + offsetH, -1.0f + offsetW,	 1.0f,  0.0f,  0.0f,				// 19 - right face
				 1.0f - offsetW, -1.0f + height + offsetH, -1.0f + offsetW,	 0.0f,  1.0f,  0.0f,				// 20 - upper face
				// 7														// 7
				 1.0f - offsetW, -1.0f + height + offsetH,  1.0f - offsetW,	 0.0f,  0.0f,  1.0f,				// 21 - front face
				 1.0f - offsetW, -1.0f + height + offsetH,  1.0f - offsetW,	 1.0f,  0.0f,  0.0f,				// 22 - right face
				 1.0f - offsetW, -1.0f + height + offsetH,  1.0f - offsetW,	 0.0f,  1.0f,  0.0f,				// 23 - upper face
			});
	}

	return temp;
}

std::vector<unsigned int> PyramidGenerator::GenerateTriangles(unsigned int layers)
{
	std::vector<unsigned int> temp;

	/// start with lower face
	size_t offset = 0;
	temp.insert(temp.end(),
		{
			offset + 0, offset + 12, offset + 3,
			offset + 3, offset + 12, offset + 15,
		});

	// side faces & connection faces
	for (size_t i = 0; i < layers - 1; ++i)
	{
		offset = i * 24;
		temp.insert(temp.end(),
			{
				offset + 1, offset + 6, offset + 13,
				offset + 13, offset + 6, offset + 18,
				offset + 2, offset + 4, offset + 7,
				offset + 7, offset + 4, offset + 9,
				offset + 5, offset + 16, offset + 10,
				offset + 10, offset + 16, offset + 21,
				offset + 17, offset + 14, offset + 22,
				offset + 22, offset + 14, offset + 19,

				offset + 8, offset + 24, offset + 20,
				offset + 20, offset + 24, offset + 36,
				offset + 8, offset + 11, offset + 24,
				offset + 24, offset + 11, offset + 27,
				offset + 11, offset + 23, offset + 27,
				offset + 27, offset + 23, offset + 39,
				offset + 23, offset + 20, offset + 39,
				offset + 39, offset + 20, offset + 36,
			});
	}

	// side faces & upper face
	offset = (layers - 1) * 24;
	temp.insert(temp.end(),
		{
			offset + 1, offset + 6, offset + 13,
			offset + 13, offset + 6, offset + 18,
			offset + 2, offset + 4, offset + 7,
			offset + 7, offset + 4, offset + 9,
			offset + 5, offset + 16, offset + 10,
			offset + 10, offset + 16, offset + 21,
			offset + 17, offset + 14, offset + 22,
			offset + 22, offset + 14, offset + 19,

			offset + 11, offset + 23, offset + 8,
			offset + 8, offset + 23, offset + 20,
		});

	return temp;
}
