
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

find_package(glfw3 REQUIRED)
target_link_libraries(imgui PUBLIC glfw)

target_include_directories(imgui
    PUBLIC
        imgui
        imgui/backends
)

add_library(objLoader INTERFACE)

target_include_directories(objLoader
    INTERFACE
        obj_loader/Source/
)
