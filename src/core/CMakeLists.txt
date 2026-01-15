add_library(core
    application.cpp
    application.h
    resource.cpp
    resource.h
    resource_handle.h
    resource_manager.cpp
    resource_manager.h
    ui_system.cpp
    ui_system.h
    window_system.cpp
    window_system.h
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

find_package(assimp REQUIRED)
find_package(stb REQUIRED)

target_link_libraries( core
    PUBLIC 
        assimp::assimp 
        GLEW::GLEW
        glfw
        glm::glm
        objLoader
        imgui
        stb::stb
) 

target_link_libraries(${APP} PRIVATE core)
