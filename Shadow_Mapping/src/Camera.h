#ifndef CAMERA_H
#define CAMERA_H

#include "GLIncludes.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cgCourse
{
	class Camera
	{
	public:
		Camera() = default;

		void create(const glm::uvec2 & _extent,
					const glm::vec3 & _position,
					const glm::vec3 & _origin,
					const glm::vec3 & _up
					);

		void setViewport(const glm::uvec2 & _extent);

		const glm::vec3 & getPosition() const;
		const glm::mat4 & getViewMatrix() const;
		const glm::mat4 & getProjectionMatrix() const;
		const glm::mat4 & getViewProjectionMatrix() const;

	private:
		void computeViewProjectionMatrix();
		void computeProjectionMatrix(const glm::uvec2 & _extent);
		void computeViewMatrix();

		glm::vec3 position;
		glm::vec3 origin;
		glm::vec3 up;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 viewProjectionMatrix;
	};
}

#endif // CAMERA_H

