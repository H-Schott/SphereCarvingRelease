#include "window.h"
#include <iostream>


void Window::GUI() {
	// Menu bar
	static bool newScene = false;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Display")) {
			if (ImGui::MenuItem("Reset Camera")) {
				ResetCamera();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Menu", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		{
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Statistics");
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
		}

		{
			if (ImGui::RadioButton("Sphere", &m_shape_id, 0)) {
				m_sdf_shape = sdf::shape::SPHERE;
				ReloadShaders();
			}
			if (ImGui::RadioButton("Box", &m_shape_id, 1)) {
				m_sdf_shape = sdf::shape::BOX;
				ReloadShaders();
			}
		}
	}
	ImGui::End();

}