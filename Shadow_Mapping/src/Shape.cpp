#include "Shape.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cgCourse
{
	Shape::~Shape()
	{
		glDeleteBuffers(1, &posBufferID);
		glDeleteBuffers(1, &colorBufferID);
		glDeleteBuffers(1, &normalBufferID);
		glDeleteBuffers(1, &indexBufferID);
		glDeleteVertexArrays(1, &vaoID);
	}

	bool Shape::createVertexArray(GLuint posAttribLoc, GLuint colAttribLoc, GLuint normAttribLoc, GLuint texAttribLoc, GLuint tangentsAttribLoc)
	{
		// check if all buffer locations are somehow defined
		if( (posAttribLoc == GLuint(-1)) ||
			(colAttribLoc == GLuint(-1)) ||
			(normAttribLoc == GLuint(-1)) ||
			(texAttribLoc == GLuint(-1)) ||
			(tangentsAttribLoc == GLuint(-1)))
		{
			return false;
		}

		// Initialize Vertex Array Object
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		// Initialize buffer objects with geometry data
		// for positions
		glGenBuffers(1, &posBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttribLoc);

		// for colors
		glGenBuffers(1, &colorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(colAttribLoc);

		// for normals
		if(normals.size() != 0)
		{
			glGenBuffers(1, &normalBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(normAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(normAttribLoc);
		}

		if(texCoords.size() != 0)
		{
			glGenBuffers(1, &texCoordsBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
			glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(texAttribLoc);
		}

		// for texture coordinates
		if(tangents.size() != 0)
		{
			glGenBuffers(1, &tangentBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, tangentBufferID);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(tangentsAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(tangentsAttribLoc);
		}

		initIndexBuffer();

		// Reset state
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}

	void Shape::draw() const
	{
		glBindVertexArray(vaoID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Shape::setPosition(glm::vec3 _pos)
	{
		objectPosition = _pos;
		translationMatrix = glm::translate(glm::mat4(1), _pos);
		calculateModelMatrix();
	}

	void Shape::setRotation(float _angle, glm::vec3 _rot)
	{
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_angle), _rot);
		calculateModelMatrix();
	}

	void Shape::setScaling(glm::vec3 _scale)
	{
		scalingMatrix = glm::scale(glm::mat4(1), _scale);
		calculateModelMatrix();
	}

	void Shape::calculateModelMatrix()
	{
		modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
	}

	void Shape::initIndexBuffer()
	{
		glGenBuffers(1, &indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(glm::uvec3), faces.data(), GL_STATIC_DRAW);
	}

	const glm::mat4 & Shape::getModelMatrix() const
	{
		return modelMatrix;
	}

	const glm::vec3 & Shape::getPosition() const
	{
		return objectPosition;
	}
}

