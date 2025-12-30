
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_demo.cpp
)

target_link_libraries(imgui
    PUBLIC
        glfw
)

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
