#include "window.h"
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <gl_tools.h>
#include <set>
#include "ImFileDialog.h"
#define _USE_MATH_DEFINES
#include <math.h>


void GLAPIENTRY MessageCallback([[maybe_unused]] GLenum source,
	GLenum type,
	[[maybe_unused]] GLuint id,
	GLenum severity,
	[[maybe_unused]] GLsizei length,
	const GLchar* message,
	[[maybe_unused]] const void* userParam) {

	static std::set<std::string> log;
	if (log.insert(message).second == false)
		// le message a deja ete affiche, pas la peine de recommencer 60 fois par seconde.
		return;

	if (severity == GL_DEBUG_SEVERITY_HIGH)
		printf("[openGL error]\n%s\n", message);
	else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
		printf("[openGL warning]\n%s\n", message);
	else
		printf("[openGL message]\n%s\n", message);
}

int Window::width = 0;
int Window::height = 0;

Window::Window(const char* windowName, int w, int h) {
	windowPtr = nullptr;

	// Window
	Window::width = w;
	Window::height = h;

	if (!glfwInit()) {
		std::cerr << "GLFW failed to initialize" << std::endl;
		glfwTerminate();
		return;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (!monitor) {
		std::cerr << "GLFW failed to get primary monitor" << std::endl;
		glfwTerminate();
		return;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	Window::width = Window::width <= 0 ? mode->width : Window::width;
	Window::height = Window::height <= 0 ? mode->height : Window::height;

	//glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	/*
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	*/
	windowPtr = glfwCreateWindow(Window::width, Window::height, windowName, NULL, NULL);
	if (windowPtr == NULL)
	{
		std::cout << "GLFW failed to create window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(windowPtr);
	glfwSetWindowSizeCallback(windowPtr, [](GLFWwindow* win, int w, int h) { 
		Window::width = w;
		Window::height = h;
		glViewport(0, 0, w, h);
	});
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// OpenGL
	glewInit();
	glEnable(GL_DEPTH_TEST);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL failed to initialize" << std::endl;
		glfwTerminate();
		return;
	}

	// Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// ImFileDialog requires you to set the CreateTexture and DeleteTexture
	ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
		GLuint tex;

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return (void*)tex;
	};
	ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
		GLuint texID = (GLuint)((uintptr_t)tex);
		glDeleteTextures(1, &texID);
	};


	glfwGetFramebufferSize(windowPtr, &Window::width, &Window::height);
	glViewport(0, 0, Window::width, Window::height);

	// vsync
	glfwSwapInterval(1);

	std::cout << "OpenGL device information: Vendor: " << (const char*)glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL device information: Renderer: " << (const char*)glGetString(GL_RENDERER) << std::endl;
	std::cout << "Dear ImGui: " << ImGui::GetVersion() << std::endl;

	// glfw callbacks
	glfwSetScrollCallback(windowPtr, [](GLFWwindow* win, double xoffset, double yoffset) {
			Window::orbiter.radius -= 0.9f * float(yoffset);
			Window::orbiter.radius = std::clamp(Window::orbiter.radius, 0.1f, 30.f);
		}
	);

	glfwSetKeyCallback(windowPtr, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_R && action == GLFW_PRESS) {
				Window::ReloadShaders();
			}
		}
	);

	// setup opengl
	std::cout << "start init GL" << std::endl;
	InitGL();
}

Window::~Window()
{
	glDeleteVertexArrays(1, &m_sdf_vao);
	glDeleteProgram(m_sdf_shader);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Window::InitGL() {

	ReloadShaders();

	glGenVertexArrays(1, &m_sdf_vao);

}


void Window::ReloadShaders() {
	std::string v_filename = std::string(RESOURCE_DIR) + "/data/shaders/sdf.vert.glsl";
	std::string f_filename = std::string(RESOURCE_DIR) + "/data/shaders/sdf.frag.glsl";
	m_sdf_shader = gl_tools::compile_shaders(v_filename, f_filename);
}

void Window::ProcessInputs() {
	// MOUSE MOVEMENT TO ORBITER MOVEMENT
	if (glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_1)) {
		double x, y;
		glfwGetCursorPos(windowPtr, &x, &y);
		glm::ivec2 current_pos = glm::ivec2(int(x), int(y));

		if (mouse_pressed) {
			glm::ivec2 delta_pos = current_pos - mouse_last_pos;
			Window::orbiter.phi -= 0.01f * float(delta_pos[0]);
			Window::orbiter.phi += 2.f * float(M_PI);
			Window::orbiter.phi -= int(Window::orbiter.phi / (2.f * float(M_PI))) * 2.f * float(M_PI);
			Window::orbiter.psi += 0.01f * float(delta_pos[1]);
			Window::orbiter.psi = std::clamp(Window::orbiter.psi, -0.48f * float(M_PI), 0.48f * float(M_PI));
		}
		
		mouse_pressed = true;
		mouse_last_pos = current_pos;
	}
	else {
		mouse_pressed = false;
	}
}

bool Window::MouseOverGUI() const {
	if (!ImGui::GetCurrentContext())
		return false;
	return ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)
		|| ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)
		|| ImGui::IsAnyItemHovered();
}

void Window::Render() {
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_sdf_shader);

	glUniform2i(glGetUniformLocation(m_sdf_shader, "uResolution"), Window::width, Window::height);
	glUniform3f(glGetUniformLocation(m_sdf_shader, "center"), Window::orbiter.center[0], Window::orbiter.center[1], Window::orbiter.center[2]);
	glm::vec3 eye = std::cos(Window::orbiter.psi) * glm::vec3(std::cos(Window::orbiter.phi), std::sin(Window::orbiter.phi), 0.f) + glm::vec3(0.f, 0.f, std::sin(Window::orbiter.psi));
	eye = Window::orbiter.radius * normalize(eye);
	glUniform3f(glGetUniformLocation(m_sdf_shader, "eye"), eye[0], eye[1], eye[2]);
	glUniform1f(glGetUniformLocation(m_sdf_shader, "focal"), Window::orbiter.focal);
	glUniform3f(glGetUniformLocation(m_sdf_shader, "up"), Window::orbiter.up[0], Window::orbiter.up[1], Window::orbiter.up[2]);

	glBindVertexArray(m_sdf_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Window::Update() {

	ProcessInputs();

	// Main Render
	Render();
	
	// UI Rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	GUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	

	// Swap buffers
	glfwSwapBuffers(windowPtr);
	glfwPollEvents();
}

bool Window::GetKey(int key) const {
	return bool(glfwGetKey(windowPtr, key));
}

bool Window::GetMousePressed(int mouse) const {
	return bool(glfwGetMouseButton(windowPtr, mouse));
}

glm::vec2 Window::GetMousePosition() const {
	double x, y;
	glfwGetCursorPos(windowPtr, &x, &y);
	return glm::vec2(x, y);
}

bool Window::Exit() const {
	return glfwWindowShouldClose(windowPtr) || glfwGetKey(windowPtr, GLFW_KEY_ESCAPE);
}

int Window::GetWidth() const {
	return width;
}

int Window::GetHeight() const {
	return height;
}

GLFWwindow* Window::getPointer() {
	return windowPtr;
}

void Window::ResetCamera() {
	Window::orbiter = Window::Orbiter();
}