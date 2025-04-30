#pragma once

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include "ImFileDialog.h"


#define SHADER_DIR (std::string(RESOURCE_DIR) + "/data/shaders").c_str()


class Window {
protected:
	GLFWwindow* windowPtr;
	static int width, height;

	// OpenGL
	inline static GLuint m_sdf_shader = 0;
	inline static GLuint m_mesh_shader = 0;
	inline static GLuint m_sdf_vao = 0;

	// Orbiter
	struct Orbiter {
		float radius = 4.f;
		float phi = 0.8f;
		float psi = 0.5f;
		glm::vec3 center = glm::vec3(0);
		glm::vec3 up = glm::vec3(0, 0, 1);
		float focal = 0.8f;
	};
	inline static Orbiter orbiter = Window::Orbiter();

	// Mouse
	glm::ivec2 mouse_last_pos = glm::ivec2(-1);
	bool mouse_pressed = false;

	void InitGL();
	void ProcessInputs();

public:
	Window(const char* windowName, int w, int h);
	~Window();

	bool Exit() const;
	void Update();

	static void ReloadShaders();

	bool GetKey(int key) const;
	bool GetMousePressed(int mouse) const;
	glm::vec2 GetMousePosition() const;
	bool MouseOverGUI() const;

	int GetWidth() const;
	int GetHeight() const;
	GLFWwindow* getPointer();

	void ResetCamera();

	void Render();
	void GUI();
};
