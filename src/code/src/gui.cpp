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

		ImGui::Separator();

		// Shapes radio buttons
		{
			for (int shape_id = 0; shape_id < sdf::shape_list.size(); shape_id++) {
				if (ImGui::RadioButton(sdf::shape_name_list[shape_id].c_str(), (int*)&m_sdf_shape, shape_id)) {
					m_sdf_shape = sdf::shape_list[shape_id];
					ResetSphereCarving();
					ReloadShaders();
				}
			}
		}

		ImGui::Separator();

		// Render modes radio buttons
		{
			for (int rmode_id = 0; rmode_id < m_render_mode_list.size(); rmode_id++) {
				if (ImGui::RadioButton(m_render_mode_name_list[rmode_id].c_str(), (int*)&m_render_mode, rmode_id)) {
					m_render_mode = m_render_mode_list[rmode_id];
					ReloadShaders();
				}
			}
		}

		ImGui::Separator();

		{
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Sphere Carving");
			ImGui::Text("%d spheres & %i points", m_sc.GetSpheresetSize(), m_sc.GetPointsetSize());
			if (ImGui::Button("Reset")) {
				m_sc = SphereCarving(m_sdf_shape);
				LoadCarvedBuffer();
			}
			ImGui::SameLine();
			if (ImGui::Button("Iterate")) {
				m_sc.Iterate();
				LoadCarvedBuffer();
			}
		}
	}
	ImGui::End();

}