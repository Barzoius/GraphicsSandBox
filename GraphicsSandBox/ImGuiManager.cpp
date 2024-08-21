#include "ImGuiManager.h"
#include "ImGui/imgui.h"

ImGuiManager::ImGuiManager()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
}

ImGuiManager::~ImGuiManager()
{
    ImGui::DestroyContext();
}