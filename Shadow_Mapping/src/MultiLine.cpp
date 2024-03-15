#include "MultiLine.h"

#include <iostream>
#include <cmath>

namespace cgCourse
{
	MultiLine::MultiLine(const std::vector<glm::vec3> & _vertices,
						 const std::vector<glm::vec3> & _normals,
						 const std::vector<glm::vec3> & _tangents,
						 const std::vector<glm::vec2> & _texCoords
						 ) : Shape()
	{
		for(int i = 0; i < _vertices.size(); ++i)
		{
			positions.push_back(_vertices[i]);
			positions.push_back(_vertices[i]);

			normals.push_back(_normals[i]);
			normals.push_back(_normals[i]);

			tangents.push_back(_tangents[i]);
			tangents.push_back(_tangents[i]);

			texCoords.push_back(_texCoords[i]);
			texCoords.push_back(_texCoords[i]);

			colors.push_back(glm::vec3(1, 1, 0));
			colors.push_back(glm::vec3(1, 1, 0));
		}
	}

	void MultiLine::draw() const
	{
		glBindVertexArray(vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
		glDrawArrays(GL_LINES, 0, positions.size());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

