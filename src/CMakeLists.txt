
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(assimp REQUIRED)
find_package(sfml REQUIRED)

set(APP "${PROJECT_NAME}_app")

add_executable(${APP})

target_sources(${APP}
    PRIVATE
        main.cpp
)

target_link_libraries( ${APP}
    PUBLIC 
        GLEW::GLEW 
        glm::glm 
        imgui::imgui 
        assimp::assimp 
        sfml::sfml
        imgui
        objLoader
        ImGui-SFML::ImGui-SFML
) 

add_subdirectory(gui)
add_subdirectory(material)
add_subdirectory(object)
add_subdirectory(render)
add_subdirectory(script)
