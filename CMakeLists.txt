cmake_minimum_required(VERSION 3.14)
project(MyGameEngineIDE)

message("Using C compiler: ${CMAKE_C_COMPILER}")
message("Using CXX compiler: ${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение SFML заголовок + библиотеки
# Путь к SFML
set(SFML_DIR ${PROJECT_SOURCE_DIR}/external/sfml)

# Пути к заголовкам SFML
include_directories(${SFML_DIR}/include)

# Библиотеки SFML
set(SFML_LIB_GRAPHICS ${SFML_DIR}/lib/libsfml-graphics.a)
set(SFML_LIB_WINDOW   ${SFML_DIR}/lib/libsfml-window.a)
set(SFML_LIB_SYSTEM   ${SFML_DIR}/lib/libsfml-system.a)

# Подключение ImGui
include_directories(${PROJECT_SOURCE_DIR}/external/imgui)
include_directories(${PROJECT_SOURCE_DIR}/external/imgui-sfml)

# Подключение json
include_directories(${PROJECT_SOURCE_DIR}/external/json)

file(GLOB IMGUI_SOURCES
    "${PROJECT_SOURCE_DIR}/external/imgui/*.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui-sfml/ImGui-SFML.cpp"
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

# Твой движок
add_executable(MyGameEngineIDE
    src/main.cpp
    src/Entity.cpp
    src/EntityManager.cpp
    src/WindowManager.cpp
    src/Renderer.cpp
    src/InputManager.cpp
    src/Physics.cpp
    src/EditorUI.cpp
    src/ResourceManager.cpp
    src/WindowRenderer.cpp
    src/ProjectManager.cpp
)

target_include_directories(MyGameEngineIDE PRIVATE
    src
    include
    ${SFML_DIR}/include
    external/imgui
    external/imgui-sfml
    external/json
)

# Линковка SFML и ImGui
target_link_libraries(MyGameEngineIDE
    ImGui
    ${SFML_LIB_GRAPHICS}
    ${SFML_LIB_WINDOW}
    ${SFML_LIB_SYSTEM}
    opengl32.lib
    glu32.lib
)