#pragma once

#include <imgui/imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace ImGuiWrapper {

GLFWwindow* create_window(const char* title);
void        begin_frame();
void        end_frame(GLFWwindow* window);
void        shutdown(GLFWwindow* window);

} // namespace ImGuiWrapper