#include "Torus.h"

#include <iostream>
#include <cmath>

namespace cgCourse
{
	// a simple 2D circle
	class Circle
	{
	public:
		Circle(float _radius, int _segments)
		{
			radius = _radius;
			segments = _segments;

			// generated equally distributed segments on this circle
			for (int j = 0; j < segments; j++)
			{
				float angle = (float) j * (360.0 / segments);
				float radians = Circle::toRadians(angle);
				vertices.push_back(glm::vec2(radius * std::sin(radians), radius * std::cos(radians)));
			}
		}

		const std::vector<glm::vec2> & getVertices() const
		{
			return vertices;
		}

		float getRadius() const
		{
			return radius;
		}

		int getSegments() const
		{
			return segments;
		}

		// Converting from degrees to radians
		// radians = (degrees * pi) / 180;
		static float toRadians(float _angle)
		{
			return _angle * M_PI / 180.0;
		}

	private:
		float radius;
		int segments;

		std::vector<glm::vec2> vertices;
	};


	Torus::Torus() : Shape()
	{
		float radiusX = 1.0;		// radius in xz-plane (main radius)
		float radiusY = 0.3;		// radius in xy-plane (radius of the ring segments)
		int segmentsInXZPlane = 50;	// subdivision in xz-plane (amount of ring segments)
		int segmentsInXYPlane = 30;	// subdivision in xy-plane

		std::vector<glm::vec3> midPoints;
		std::vector<float> midPointsLenght;

		Circle circleXZ(radiusX, segmentsInXZPlane);
		Circle circleXY(radiusY, segmentsInXYPlane);

		// calculate positions
		for (int j = 0; j < circleXZ.getSegments(); j++)
		{
			for (int i = 0; i < circleXY.getVertices().size(); i++)
			{
				glm::vec3 currentPoint(circleXZ.getVertices()[j].x, 0.0, circleXZ.getVertices()[j].y);
				glm::vec3 dirExtension = glm::normalize(currentPoint) * circleXY.getVertices()[i].x;
				currentPoint.y = circleXY.getVertices()[i].y;
				positions.push_back(currentPoint + dirExtension);
			}
		}

		// calculate faces
		for(int j = 0; j < circleXZ.getSegments(); j++)
		{
			int startIdx = j * circleXY.getSegments();
			if(j + 1 == circleXZ.getSegments())
			{
				// special case for connecting the last ring segments
				for(int i = 0; i < circleXY.getSegments(); i++)
				{
					int currIdx = startIdx + i;
					int firstSegIdx = i;
					if(i + 1 == circleXY.getSegments())
					{
						addTwoFaces(currIdx, firstSegIdx, startIdx, 0);
					}
					else
					{
						addTwoFaces(currIdx, firstSegIdx, currIdx + 1, firstSegIdx + 1);
					}
				}
			}
			else
			{
				// this parts connects all ring segments
				for (int i = 0; i < circleXY.getSegments(); i++)
				{
					int currIdx = startIdx + i;
					// special case for the last two faces of a single ring segment
					if(i + 1 == circleXY.getSegments())
					{
						addTwoFaces(currIdx, currIdx + circleXY.getSegments(), startIdx, startIdx + circleXY.getSegments());
					}
					else
					{
						addTwoFaces(currIdx, currIdx + circleXY.getSegments(), currIdx + 1, currIdx + 1 + circleXY.getSegments());
					}
				}
			}
		}

		// just color the object from black to green
		for(int i = 0; i < positions.size(); i++)
		{
			colors.push_back(glm::vec3(0.06, 0.55, 0.31));
		}

		// adding normals
		normals.resize(positions.size());
		for(const auto & f: faces)
		{
			glm::vec3 v1 = positions[f.y] - positions[f.x];
			glm::vec3 v2 = positions[f.z] - positions[f.x];
			glm::vec3 n = glm::normalize(glm::cross(v1, v2));
			normals[f.x] += n;
			normals[f.y] += n;
			normals[f.z] += n;
		}

		for(auto & n: normals)
			n = glm::normalize(n);

		// texture coordinates
		for(int j = 0; j < circleXZ.getSegments(); j++)
		{
			float sCoord = j * 1.0 / circleXZ.getSegments() * 4.0;
			for(int i = 0; i < circleXY.getSegments(); i++)
				texCoords.push_back({sCoord, i * 1.0 / circleXY.getSegments()});
		}

		// adding tangents
		tangents.resize(positions.size());
		for(const auto & f: faces)
		{
			for(int i = 0; i < 3; ++i)
			{
				const int & v0 = f[i];
				const int & v1 = f[(i + 1) % 3];
				const int & v2 = f[(i + 2) % 3];

				glm::vec3 edge1 = positions[v1] - positions[v0];
				glm::vec3 edge2 = positions[v2] - positions[v0];

				const glm::vec3 & normal = normals[v0];
				edge1 = edge1 - glm::dot(edge1, normal) * normal;
				edge2 = edge2 - glm::dot(edge2, normal) * normal;

				glm::vec2 duv1 = texCoords[v1] - texCoords[v0];
				glm::vec2 duv2 = texCoords[v2] - texCoords[v0];

				const float & det = 1.f / (duv1.x * duv2.y - duv2.x * duv1.y);

				glm::vec3 & tangent = tangents[v0];
				tangent.x = det * (duv2.y * edge1.x - duv1.y * edge2.x);
				tangent.y = det * (duv2.y * edge1.y - duv1.y * edge2.y);
				tangent.z = det * (duv2.y * edge1.z - duv1.y * edge2.z);

				tangent = normalize(tangent);
			}
		}
	}

	void Torus::addTwoFaces(unsigned int _idx0, unsigned int _idx1, unsigned int _idx2, unsigned int _idx3)
	{
		faces.push_back(glm::uvec3(_idx0, _idx2, _idx1));
		faces.push_back(glm::uvec3(_idx2, _idx3, _idx1));
	}
}

