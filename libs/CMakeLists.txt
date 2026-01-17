
add_library(objLoader INTERFACE)

target_include_directories(objLoader
    INTERFACE
        obj_loader/Source/
)

add_library(imgui_backend INTERFACE)

target_include_directories(imgui_backend
    INTERFACE
        imgui_backend/
)
