// #include <quick_imgui/quick_imgui.hpp>
// #include "ImGuiWrapper.hpp"

// namespace quick_imgui {

// void loop(const char* window_name, std::function<void()> const& loop_callback)
// {
//     loop(window_name, []() {}, loop_callback);
// }

// void loop(const char* window_name, std::function<void()> const& init_callback, std::function<void()> const& loop_callback) // NOLINT(*easily-swappable-parameters)
// {
//     GLFWwindow* const window = ImGuiWrapper::create_window(window_name);
//     init_callback();
//     // Main loop
//     while (!glfwWindowShouldClose(window))
//     {
//         glfwPollEvents();
//         ImGuiWrapper::begin_frame();
//         loop_callback();
//         ImGuiWrapper::end_frame(window);
//     }

//     ImGuiWrapper::shutdown(window);
// }

// } // namespace quick_imgui
#include <quick_imgui/quick_imgui.hpp>
#include "ImGuiWrapper.hpp"

namespace quick_imgui {

namespace {
    WindowWrapper& GLFWwindow_as_wrapper(GLFWwindow* window)
    {
        return *static_cast<WindowWrapper*>(glfwGetWindowUserPointer(window));
    }
}

WindowWrapper::WindowWrapper(const char* window_name): WindowWrapper(window_name, []() {}) {}

WindowWrapper::WindowWrapper(const char* window_name, std::function<void()> const& init_callback): window (ImGuiWrapper::create_window(window_name)) {
    init_callback();

    // assign window user pointer to this wrapper
    glfwSetWindowUserPointer(window, static_cast<void*>(this));

    // bind callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (const auto& key_callback = GLFWwindow_as_wrapper(window).key_callback) {
            key_callback(key, scancode, action, mods);
        }
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        if (const auto& mouse_button_callback = GLFWwindow_as_wrapper(window).mouse_button_callback) {
            mouse_button_callback(button, action, mods);
        }
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        if (const auto& scroll_callback = GLFWwindow_as_wrapper(window).scroll_callback) {
            scroll_callback(xoffset, yoffset);
        }
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        if (const auto& cursor_position_callback = GLFWwindow_as_wrapper(window).cursor_position_callback) {
            cursor_position_callback(xpos, ypos);
        }
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        if (const auto& size_callback = GLFWwindow_as_wrapper(window).size_callback) {
            size_callback(width, height);
        }
    });
}

void WindowWrapper::run(std::function<void()> const& loop_callback) {
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGuiWrapper::begin_frame();
        loop_callback();
        ImGuiWrapper::end_frame(window);
    }
}

WindowWrapper::~WindowWrapper() {
    ImGuiWrapper::shutdown(window);
}

} // namespace quick_imgui