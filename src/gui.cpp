#include "gui.h"

#include "imgui.h"

CameraControlsUI::CameraControlsUI(float& yaw, float& pitch, float& radius, bool& ortho)
    : _yaw{ yaw }, _pitch{ pitch }, _radius{ radius }, _ortho{ ortho }
{
}

void CameraControlsUI::render()
{
    ImGui::Begin("Camera Controls");

    ImGui::SliderFloat("Yaw", &_yaw, 0.0f, 360.0f);
    ImGui::SliderFloat("Pitch", &_pitch, 0.1f, 179.9f);
    ImGui::SliderFloat("Radius", &_radius, 1.0f, 10.0f);

    ImGui::Checkbox("Orthographic", &_ortho);

    ImGui::End();
}

LightingControlsUI::LightingControlsUI(WorldPosition light_pos, bool& auto_movement)
    : _light_pos{light_pos}, _auto_movement{auto_movement}
{
}

void LightingControlsUI::render()
{
    ImGui::Begin("Lighting Controls");

    ImGui::InputFloat3("Light Position",  (float*)(&_light_pos));
    ImGui::Checkbox("Auto-Movement", &_auto_movement);

    ImGui::End();
}

RenderOptionsUI::RenderOptionsUI(bool& wireframe)
    : _wireframe{ wireframe }
{
}

void RenderOptionsUI::render()
{
    ImGui::Begin("Rendering Options");

    ImGui::Checkbox("Wireframe Mode", &_wireframe);

    ImGui::End();
}