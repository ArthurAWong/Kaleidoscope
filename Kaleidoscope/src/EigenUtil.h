#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "Eigen/Eigen"

template <typename T>
Eigen::Transform<T, 3, Eigen::Affine> Perspective(T const& FoV, T const& aspect, T const& zNear, T const& zFar) {
	// Creating an Eigen equivalent of glm::perspective

	T const tanHalfFoV = std::tan(FoV / static_cast<T>(2));
	Eigen::Transform<T, 3, Eigen::Affine > result;
	memset(result.data(), 0, sizeof(result));

	result(0, 0) = static_cast<T>(1) / (aspect * tanHalfFoV);
	result(1, 1) = static_cast<T>(1) / tanHalfFoV;
	result(2, 2) = -(zFar + zNear) / (zFar - zNear);
	result(2, 3) = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
	result(3, 2) = -static_cast<T>(1);
	return result;

}

template <typename T>
Eigen::Transform <typename T::Scalar, 3, Eigen::Affine > LookAt(T const& eyePos, T const& target, T const& worldUp) {
	// Creating an Eigen equivalent of glm::lookAt

	Eigen::Transform<typename T::Scalar, 3, Eigen::Affine, Eigen::ColMajor > result;

	T const z = (eyePos - target).normalized();
	T const x = worldUp.cross(z).normalized(); // Normalization for bottom two required to reduce propagation of error from nearly parallel vectors
	T const y = z.cross(x).normalized();
	result(0, 0) = x(0);
	result(0, 1) = x(1);
	result(0, 2) = x(2);
	result(0, 3) = -x.dot(eyePos);
	result(1, 0) = y(0);
	result(1, 1) = y(1);
	result(1, 2) = y(2);
	result(1, 3) = -y.dot(eyePos);
	result(2, 0) = z(0);
	result(2, 1) = z(1);
	result(2, 2) = z(2);
	result(2, 3) = -z.dot(eyePos);
	result(3, 0) = 0;
	result(3, 1) = 0;
	result(3, 2) = 0;
	result(3, 3) = 1;

	return result;

}

template <typename T>
T Radians(T const& degrees) {
	return degrees * static_cast<T>(M_PI) / static_cast<T>(180);
}