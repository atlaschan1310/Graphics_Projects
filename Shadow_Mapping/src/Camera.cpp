#include "Camera.h"

#include <iostream>
#include <cmath>

namespace cgCourse
{
	void Camera::create(const glm::uvec2 & _extent,
						const glm::vec3 & _position,
						const glm::vec3 & _origin,
						const glm::vec3 & _up)
	{
		position = _position;
		origin = _origin;
		up = _up;
		computeViewMatrix();
		setViewport(_extent);
	}

	void Camera::setViewport(const glm::uvec2 & _extent)
	{
		glViewport(0, 0, int(_extent.x), int(_extent.y));
		computeProjectionMatrix(_extent);
		computeViewProjectionMatrix();
	}

	void Camera::computeViewProjectionMatrix()
	{
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void Camera::computeViewMatrix()
	{
		viewMatrix = glm::lookAt(position, origin, up);
	}

	void Camera::computeProjectionMatrix(const glm::uvec2 & _extent)
	{
		const float & fieldOfView = 45.0f;
		const float & nearPlane = 0.1;
		const float & farPlane = 50.0;
		const float & aspect = float(_extent.x) / float(_extent.y);

		projectionMatrix = glm::perspective(fieldOfView, aspect, nearPlane, farPlane);
	}

	const glm::vec3 & Camera::getPosition() const
	{
		return position;
	}

	const glm::mat4 & Camera::getViewMatrix() const
	{
		return viewMatrix;
	}

	const glm::mat4 & Camera::getProjectionMatrix() const
	{
		return projectionMatrix;
	}

	const glm::mat4 & Camera::getViewProjectionMatrix() const
	{
		return viewProjectionMatrix;
	}
}

