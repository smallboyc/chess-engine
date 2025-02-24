cmake_minimum_required(VERSION 3.14)
project(glmax)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

# Add cpp files
add_library(stb_image third-party/stb_image.cpp)
add_library(glmax ${SRC_FILES})

# Add hpp files
target_include_directories(glmax PUBLIC src third-party)
target_link_libraries(glmax PRIVATE stb_image)

# Add glm
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        bf71a834948186f4097caa076cd2663c69a10e1e
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

# Add quick_imgui for glad
target_link_libraries(glmax PUBLIC quick_imgui)

# cmake_minimum_required(VERSION 3.14)
# project(glmax)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Recherche tous les fichiers source .cpp et .hpp dans /src et ses sous-dossiers
# file(GLOB_RECURSE SRC_FILES src/**/*.cpp, src/**/*.hpp)

# # Créer une bibliothèque pour stb_image
# add_library(stb_image third-party/stb_image.cpp)

# # Créer la bibliothèque glmax avec tous les fichiers source trouvés
# add_library(glmax ${SRC_FILES})

# # Inclure les répertoires src et third-party pour l'inclusion des fichiers header
# target_include_directories(glmax PUBLIC src third-party)

# # Lier la bibliothèque stb_image avec glmax
# target_link_libraries(glmax PRIVATE stb_image)

# # Ajouter glm via FetchContent
# include(FetchContent)
# FetchContent_Declare(
#     glm
#     GIT_REPOSITORY https://github.com/g-truc/glm.git
#     GIT_TAG        bf71a834948186f4097caa076cd2663c69a10e1e
# )
# FetchContent_MakeAvailable(glm)
# target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

# # Ajouter quick_imgui pour gérer le rendu avec ImGui
# target_link_libraries(glmax PUBLIC quick_imgui)
