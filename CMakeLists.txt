cmake_minimum_required(VERSION 3.11)

# Project setup
project(glm_tinyobjloader_cimg_example)

# Ensure at least C++17 compatibility
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# FetchContent to download GLM, tinyobjloader, CImg, and stb
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e # refs/tags/1.0.1
)
FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader
    GIT_TAG v2.0.0rc13 # Replace with the desired version or branch
)
FetchContent_Declare(
    CImg
    GIT_REPOSITORY https://github.com/GreycLab/CImg
    GIT_TAG v.3.5.3 # Replace with the desired version or branch
)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb
    GIT_TAG master # Replace with the desired branch or commit if needed
)
FetchContent_MakeAvailable(glm tinyobjloader CImg stb)

if(APPLE)
    include_directories(/opt/X11/include)
endif()

# Add the executable
add_executable(main main.cpp code/Transformation.cpp code/Object.cpp code/Scene.cpp code/Graphics.cpp code/Intersection.cpp code/Datastructure.cpp)

# Include the CImg and stb directories
target_include_directories(main PRIVATE ${CMAKE_BINARY_DIR}/_deps/cimg-src)
target_include_directories(main PRIVATE ${CMAKE_BINARY_DIR}/_deps/stb-src)

# Link the libraries to the executable
target_link_libraries(main PRIVATE glm::glm tinyobjloader)
