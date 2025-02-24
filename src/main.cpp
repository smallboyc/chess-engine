#include <imgui.h>
#include "glad/glad.h"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    quick_imgui::loop(
        "Chess",
        /* init: */
        [&]() {
        },
        /* loop: */
        [&]() {
            glClearColor(0.847f, 0.82f, 0.929f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    );
}