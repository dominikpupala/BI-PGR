//----------------------------------------------------------------------------------------
/**
 * \file       Curve.cpp
 * \author     Dominik Pupala
 * \date       2021/14/05
 * \brief      Source file for Catmull-Rom curves.
 *
 *  Source file containing declarations for for Curve class.
 *
*/
//----------------------------------------------------------------------------------------

#include "Curve.h"

glm::mat4 Curve::AlignObject(const glm::vec3& position, const glm::vec3& direction)
{
	glm::vec3 z = -glm::normalize(direction);
	z = (!z.x && !z.y && !z.z) ? glm::vec3(0.0f, 0.0f, 1.0f) : z;

	glm::vec3 x = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), z));
	x = (!x.x && !x.y && !x.z) ? glm::vec3(1.0f, 0.0f, 0.0f) : x;

	glm::vec3 y = glm::cross(z, x);
	glm::mat4 matrix = glm::mat4(
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		position.x, position.y, position.z, 1.0f
	);

	return matrix;
}

glm::mat4 Curve::BasisMatrix(float s)
{
	return 2.0f * glm::mat4(
		glm::vec4(-s, 2 * s, -s, 0.0f),
		glm::vec4(2 - s, s - 3, 0.0f, 1.0f),
		glm::vec4(s - 2, 3 - 2 * s, s, 0.0f),
		glm::vec4(s, -s, 0.0f, 0.0f)
	);
}

glm::vec3 Curve::EvalCurve(const std::vector<glm::vec3>& points, const glm::mat4& baseMatrix, float t)
{
	float t0_n = fmod(t, (float)points.size());
	size_t i = int(t0_n);
	float t0_1 = t0_n - i;

	return EvalCurveSegment(points[(i - 1 + points.size()) % points.size()], points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()], baseMatrix, t0_1);
}

glm::vec3 Curve::EvalCurveSegment(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::mat4& baseMatrix, float t)
{
	return 0.5f * glm::vec4(t * t * t, t * t, t, 1) * baseMatrix * glm::transpose(glm::mat4x3(p0, p1, p2, p3));
}

glm::vec3 Curve::EvalCurveDerivate(const std::vector<glm::vec3>& points, const glm::mat4& baseMatrix, float t)
{
	float t0_n = fmod(t, (float)points.size());
	size_t i = int(t0_n);
	float t0_1 = t0_n - i;

	return EvalCurveDerivateSegment(points[(i - 1 + points.size()) % points.size()], points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()], baseMatrix, t0_1);
}

glm::vec3 Curve::EvalCurveDerivateSegment(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::mat4& baseMatrix, float t)
{
	return 0.5f * glm::vec4(3 * t * t, 2 * t, 1, 0) * baseMatrix * glm::transpose(glm::mat4x3(p0, p1, p2, p3));
}
