#include "window.h"
#include <iostream>


void Window::GUI() {
	// Menu bar
	static bool newScene = false;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Load DEM")) {
				ifd::FileDialog::Instance().Open("LoadDEM_Dialog", "Choose a DEM", "Image file (*.png;*.jpg;*.jpeg;){.png,.jpg,.jpeg,.bmp,.tga},.*", false, TERRAIN_DIR);
			}
			if (ImGui::MenuItem("Load Texture")) {
				ifd::FileDialog::Instance().Open("LoadTexture_Dialog", "Choose a Texture", "Image file (*.png;*.jpg;*.jpeg;){.png,.jpg,.jpeg,.bmp,.tga},.*", false, TERRAIN_DIR);
			}
			if (ImGui::MenuItem("Save DEM")) {
				ifd::FileDialog::Instance().Save("SaveDEM_Dialog", "Save current DEM", "", TERRAIN_DIR);
			}
			ImGui::EndMenu();
		}

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
			std::string size_stat = std::string("Current terrain size: ") + std::to_string(m_hf.GetSizeX()) + " x " + std::to_string(m_hf.GetSizeY());
			ImGui::Text(size_stat.c_str());
		}
	}
	ImGui::End();

	if (ifd::FileDialog::Instance().IsDone("LoadDEM_Dialog")) {
		if (ifd::FileDialog::Instance().HasResult()) {
			std::string res = ifd::FileDialog::Instance().GetResult().u8string();
			m_hf = HeightField(Box2D(glm::vec2(0), 10 * 1000), res.c_str(), 0., 3000.);
			LoadTerrain();
			ResetCamera();
		}
		ifd::FileDialog::Instance().Close();
	}

	if (ifd::FileDialog::Instance().IsDone("LoadTexture_Dialog")) {
		if (ifd::FileDialog::Instance().HasResult()) {
			std::string res = ifd::FileDialog::Instance().GetResult().u8string();

		}
		ifd::FileDialog::Instance().Close();
	}

	if (ifd::FileDialog::Instance().IsDone("SaveDEM_Dialog")) {
		if (ifd::FileDialog::Instance().HasResult()) {
			std::string res = ifd::FileDialog::Instance().GetResult().u8string();
			std::cout << res << std::endl;
			m_hf.Save((std::string(res) + ".png").c_str());
		}
		ifd::FileDialog::Instance().Close();
	}
}