
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_demo.cpp
)

#target_link_libraries(imgui
#    PRIVATE
#        glfw
#)

target_include_directories(imgui
    PUBLIC
        imgui
        imgui/backends
)


#install(TARGETS imgui EXPORT ImGui-SFML)

#set(IMGUI_SFML_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
#set(IMGUI_SFML_INSTALL OFF CACHE BOOL "" FORCE)
#set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui/)

add_subdirectory(imgui-sflm)

add_library(objLoader INTERFACE)

target_include_directories(objLoader
    INTERFACE
        obj_loader/Source/
)
