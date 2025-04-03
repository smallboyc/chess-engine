cmake_minimum_required(VERSION 3.20)
project(ChessGame)

# Crée l'exécutable principal
add_executable(${PROJECT_NAME})

# Choix du standard C++
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Options de compilation
if(MSVC)
    # target_compile_options(${PROJECT_NAME} PRIVATE /WX /W4)
else()
    # target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors -Wimplicit-fallthrough)
endif()

# Définition du dossier de sortie des exécutables
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Désactivation des extensions spécifiques au compilateur
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_EXTENSIONS OFF)

# Ajout des fichiers source
file(GLOB_RECURSE MY_SOURCES src/*.cpp src/*.hpp src/**/*.cpp src/**/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${MY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Ajout de la bibliothèque quick_imgui
add_subdirectory(lib/quick_imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE quick_imgui::quick_imgui)

# include(FetchContent)
# FetchContent_Declare(
#     quick_imgui
#     GIT_REPOSITORY https://github.com/dsmtE/quick_imgui
#     GIT_TAG 462db5f449a38194e263fe3881e0348d204ea7f2
# )
# FetchContent_MakeAvailable(quick_imgui)
# target_link_libraries(${PROJECT_NAME} PRIVATE quick_imgui::quick_imgui)

# Ajout de la bibliothèque glmax
add_subdirectory(lib/glmax)
target_link_libraries(${PROJECT_NAME} PRIVATE glmax)
