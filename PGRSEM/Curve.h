//----------------------------------------------------------------------------------------
/**
 * \file       Curve.h
 * \author     Dominik Pupala
 * \date       2021/14/05
 * \brief      Header file for Catmull-Rom curves.
 *
 *  Header file containing definitions for Curve class.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <vector>

#include "pgr.h"

/// Static class that handles curves calculations.
/**
  This static class contains functionality for Catmull-Rom curves.
*/
class Curve
{
public:
	/// Aligns object.
	/**
		Aligns object via creating the model matrix from position and direction vectors.

		\param[in] position		Position of object.
		\param[in] direction	Direction of object.
	*/
	static glm::mat4 AlignObject(const glm::vec3& position, const glm::vec3& direction);
	/// Parametrizable Catmull-Rom basis matrix.
	/**
		Returns the Catmull-Rom basis from the given parameter. Matrix will be multiplied by factor of 2.

		\param[in] s	Catmull-Rom basis parameter.
	*/
	static glm::mat4 BasisMatrix(float s);
	/// Evaluates the position in appropiate segment.
	/**
		Calculates appropiate segment and returns the position on the curve.

		\param[in] points		Control points of the curve.
		\param[in] baseMatrix	Catmull-Rom basis matrix for the curve.
		\param[in] t			Time factor on the curve.
	*/
	static glm::vec3 EvalCurve(const std::vector<glm::vec3>& points, const glm::mat4& baseMatrix, float t);
	/// Calculates the position in the given segment.
	/**
		Calculates the position on the curve.

		\param[in] p0			First control point of the curve.
		\param[in] p1			Second control point of the curve.
		\param[in] p2			Third control point of the curve.
		\param[in] p3			Fourth control point of the curve.
		\param[in] baseMatrix	Catmull-Rom basis matrix for the curve.
		\param[in] t			Time factor on the curve.
	*/
	static glm::vec3 EvalCurveSegment(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::mat4& baseMatrix, float t);
	/// Evaluates the first derivate in appropiate segment.
	/**
		Calculates appropiate segment and returns the first derivate on the curve.

		\param[in] points		Control points of the curve.
		\param[in] baseMatrix	Catmull-Rom basis matrix for the curve.
		\param[in] t			Time factor on the curve.
	*/
	static glm::vec3 EvalCurveDerivate(const std::vector<glm::vec3>& points, const glm::mat4& baseMatrix, float t);
	/// Calculates the first derivate in the given segment.
	/**
		Calculates the first derivate on the curve.

		\param[in] p0			First control point of the curve.
		\param[in] p1			Second control point of the curve.
		\param[in] p2			Third control point of the curve.
		\param[in] p3			Fourth control point of the curve.
		\param[in] baseMatrix	Catmull-Rom basis matrix for the curve.
		\param[in] t			Time factor on the curve.
	*/
	static glm::vec3 EvalCurveDerivateSegment(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::mat4& baseMatrix, float t);

private:
	/// Disabled constructor
	/**
		Constructor created so the default one isn't created.
		This class is meant to be static.
	*/
	Curve() {};
};

