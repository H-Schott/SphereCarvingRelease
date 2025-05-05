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

		// Shapes combo
		{
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "SDF shapes");
			if (ImGui::BeginCombo("Shapes", sdf::shape_name_list[(int)m_sdf_shape].c_str())) {
				for (int shape_id = 0; shape_id < sdf::shape_name_list.size(); shape_id++) {
					bool is_selected = ((int)m_sdf_shape == shape_id);
					if (ImGui::Selectable(sdf::shape_name_list[shape_id].c_str(), is_selected)) {
						m_sdf_shape = (sdf::shape)shape_id;
						ResetSphereCarving();
						ReloadShaders();
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}

		ImGui::Separator();

		// Render modes radio buttons
		{
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Render mode");
			for (int rmode_id = 0; rmode_id < m_render_mode_name_list.size(); rmode_id++) {
				if (ImGui::RadioButton(m_render_mode_name_list[rmode_id].c_str(), (int*)&m_render_mode, rmode_id)) {
					m_render_mode = (Window::render_mode)rmode_id;
					ReloadShaders();
				}
			}
		}

		ImGui::Separator();

		{
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Sphere Carving");
			ImGui::Text("%d spheres & %i points & %i planes", m_sc.GetSpheresetSize(), m_sc.GetPointsetSize(), m_sc.GetConvexHullSize());
			if (ImGui::Button("Reset")) {
				ResetSphereCarving();
			}
			ImGui::SameLine();
			if (ImGui::Button("Iterate")) {
				m_sc.Iterate();
				LoadCarvedBuffer();
				LoadBoundBuffer();
			}
		}
	}
	ImGui::End();

}