#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GLIncludes.h"
#include <string>

namespace cgCourse
{
	class ShaderProgram
	{
	public:
		ShaderProgram(std::string _name);

		void deleteShaderProgramFromGPU();
		void bind() const;
		void unbind() const;

		GLuint getUniformLocation(const std::string & _uniformName);

	private:
		bool loadAndCompileShaderToGPU(GLuint _program, const std::string & _source);
		bool checkLinkStatusOfProgram(GLuint _program);
		std::string loadFile(const std::string & filename) const;

		GLuint program = 0;
	};
}

#endif // SHADERPROGRAM_H

