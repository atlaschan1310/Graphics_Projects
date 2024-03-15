#ifndef MULTILINE_H
#define MULTILINE_H

#include "Shape.h"

namespace cgCourse
{
	class MultiLine : public Shape
	{
	public:
		MultiLine(	const std::vector<glm::vec3> & _vertices,
					const std::vector<glm::vec3> & _normals,
					const std::vector<glm::vec3> & _tangents,
					const std::vector<glm::vec2> & _texCoords
					);

		void draw() const override;
	};
}

#endif // MULTILINE_H

