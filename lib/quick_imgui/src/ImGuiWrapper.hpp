#pragma once

#include <imgui/imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace ImGuiWrapper {

// callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void size_callback(GLFWwindow* window, int width, int height);

// basic setup
GLFWwindow* create_window(const char* title);
void        begin_frame();
void        end_frame(GLFWwindow* window);
void        shutdown(GLFWwindow* window);


} // namespace ImGuiWrapper