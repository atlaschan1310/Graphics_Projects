#ifndef GLAPP_H
#define GLAPP_H

#include "GLIncludes.h"
#include <glm/vec2.hpp>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


namespace cgCourse
{
	class GLApp
	{
	public:
		GLApp(glm::uvec2 _windowSize, std::string _title, bool resizeable = true);
		virtual ~GLApp();

		bool run();

		// Default implementation of event handlers
		virtual void keyPressHandler(int key, int action, int mods);
		virtual void mouseClickHandler(int button, int action);
		virtual void mouseMoveHandler(glm::dvec2 position);

	protected:
		void stop();

		// Virtual methods for initialization, rendering and termination
		virtual bool init() = 0;
		virtual bool update() = 0;
		virtual bool render() = 0;
		virtual bool end() = 0;
		virtual void imgui();

		// Utility methods
		glm::uvec2 getFramebufferSize() const;
		bool checkGlError(const char * message) const;

		GLFWwindow * window_ = nullptr;
		std::string const title;
	};
}


#endif // GLAPP_H

