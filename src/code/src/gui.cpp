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
			for (int shape_id = 0; shape_id < sdf::shape_list.size(); shape_id++) {
				if (ImGui::RadioButton(sdf::shape_name_list[shape_id].c_str(), &m_shape_id, shape_id)) {
					m_sdf_shape = sdf::shape_list[shape_id];
					ReloadShaders();
				}
			}
		}
	}
	ImGui::End();

}