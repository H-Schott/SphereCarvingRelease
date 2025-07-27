#pragma once

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include "ImFileDialog.h"
#include "spherecarving.h"


#define SHADER_DIR (std::string(RESOURCE_DIR) + "/data/shaders").c_str()


class Window {
protected:
	static GLFWwindow* windowPtr;
	static int width, height;
	static float xscale, yscale;

	// Model
	inline static sdf::shape m_sdf_shape = sdf::shape::CAD_1;

	// OpenGL
	inline static GLuint m_sdf_shader = 0;
	inline static GLuint m_carved_shader = 0;
	inline static GLuint m_bound_shader = 0;
	inline static GLuint m_sdfbound_shader = 0;

	inline static GLuint m_sdf_vao = 0;
	inline static GLuint m_carved_texture = 0;
	inline static GLuint m_bound_texture = 0;

	enum class render_mode {SDF, CARVED, BOUND, SDF_BOUND};
	inline static render_mode m_render_mode = render_mode::SDF_BOUND;
	inline static const std::array<std::string, 4> m_render_mode_name_list = { "Sdf", "Carved", "Bound", "Sdf + Bound"};

	// Orbiter
	struct Orbiter {
		float radius = 4.f;
		float phi = 0.8f;
		float psi = 0.5f;
		glm::vec3 center = glm::vec3(0);
		glm::vec3 up = glm::vec3(0, 0, 1);
		float focal = 0.8f;

		Orbiter() {};
	};
	inline static Orbiter orbiter = Window::Orbiter();

	// Mouse
	inline static glm::ivec2 mouse_last_pos = glm::ivec2(-1);
	inline static bool mouse_pressed = false;

	// Sphere Carving
	inline static SphereCarving m_sc = SphereCarving(m_sdf_shape);

	static void InitGL();
	static void ProcessInputs();
	static void LoadCarvedData();
	static void LoadBoundData();
	static void RenderSDF();
	static void RenderCarved();
	static void RenderBound();
	static void RenderSdfBound();
	static void Render();

public:
	Window(const char* windowName, int w, int h);
	~Window();

	static bool Exit();
	static void Update();

	static void ReloadShaders();
	static void ResetSphereCarving();

	static bool GetKey(int key);
	static bool GetMousePressed(int mouse);
	static glm::vec2 GetMousePosition();
	static bool MouseOverGUI();

	static int GetWidth();
	static int GetHeight();
	static GLFWwindow* getPointer();

	static void ResetCamera();

	static void GUI();
};
