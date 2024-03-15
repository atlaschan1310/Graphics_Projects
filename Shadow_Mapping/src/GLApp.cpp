#include "GLApp.h"

#include <glm/vec2.hpp>
#include <iostream>
#include <fstream>

#ifndef GLFW_TRUE
	#define GLFW_TRUE GL_TRUE
#endif
#ifndef GLFW_FALSE
	#define GLFW_FALSE GL_FALSE
#endif

namespace cgCourse
{
	namespace
	{
		struct EventDispatcher
		{
			static void keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods)
			{
				GLApp* app = static_cast<GLApp*>(glfwGetWindowUserPointer(_window));
				if(app)
				{
					app->keyPressHandler(_key, _action, _mods);
				}
			}

			static void mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods)
			{
				GLApp* app = static_cast<GLApp*>(glfwGetWindowUserPointer(_window));
				if(app)
				{
					app->mouseClickHandler(_button, _action);
				}
			}

			static void cursorPosCallback(GLFWwindow * _window, double _xpos, double _ypos)
			{
				GLApp* app = static_cast<GLApp*>(glfwGetWindowUserPointer(_window));
				if(app)
				{
					app->mouseMoveHandler(glm::dvec2(_xpos, _ypos));
				}
			}
		};
	} // namespace


	GLApp::GLApp(glm::uvec2 _windowSize, std::string _title, bool _resizeable): title(_title)
	{
		// GLFW initialization
		if(!glfwInit())
		{
			std::cout << "Failed to initialize GLFW\n" << std::endl;
		}

		#ifdef __APPLE__
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, _resizeable ? GLFW_TRUE : GLFW_FALSE);

		// Window and OpenGL context creation
		window_ = glfwCreateWindow(int(_windowSize.x), int(_windowSize.y), title.c_str(), NULL, NULL);
		assert(window_);

		glfwSetWindowPos(window_, 64, 64);
		glfwSetWindowUserPointer(window_, this);
		glfwSetInputMode(window_, GLFW_STICKY_KEYS, 1);
		glfwSetKeyCallback(window_, EventDispatcher::keyCallback);
		glfwSetMouseButtonCallback(window_, EventDispatcher::mouseButtonCallback);
		glfwSetCursorPosCallback(window_, EventDispatcher::cursorPosCallback);

		// Load OpenGL core profile functions using gl3w
		glfwMakeContextCurrent(window_);

		// Initialize GLEW
		if(glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW\n" << std::endl;
		}

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		    
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void) io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window_, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	GLApp::~GLApp()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	bool GLApp::run()
	{
		// Call custom application initialization
		if(!init())
			return false;

		// Render loop: process input handlers and call custom rendering
		while(!glfwWindowShouldClose(window_))
		{
			glfwPollEvents();

			if(!render()) return false;


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			imgui();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(window_);
		}

		// Call custom application cleaning and termination
		return end();
	}

	void GLApp::keyPressHandler(int _key, int _action, int _mods)
	{
		if(_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
		{
			stop();
		}
	}

	void GLApp::mouseClickHandler(int _button, int _action)
	{
	}

	void GLApp::mouseMoveHandler(glm::dvec2 _position)
	{
	}

	void GLApp::stop()
	{
		glfwSetWindowShouldClose(window_, GL_TRUE);
	}

	void GLApp::imgui()
	{
		ImGui::ShowDemoWindow();
	}

	glm::uvec2 GLApp::getFramebufferSize() const
	{
		glm::ivec2 sizes(0);
		glfwGetFramebufferSize(window_, &sizes.x, &sizes.y);
		return glm::uvec2(sizes);
	}

	bool GLApp::checkGlError(const char * _message) const
	{
		int error = glGetError();

		if(error != GL_NO_ERROR)
		{
			std::string errorString;
			switch(error)
			{
			case GL_INVALID_ENUM:
				errorString = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				errorString = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				errorString = "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				errorString = "GL_OUT_OF_MEMORY";
				break;
			default:
				errorString = "UNKNOWN";
				break;
			}
			std::cerr << "[" << _message << "] OpenGL Error(" << "): " << errorString << std::endl;
		}
		return error == GL_NO_ERROR;
	}
}

