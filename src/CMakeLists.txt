
find_package(glm CONFIG REQUIRED)
find_package(glew REQUIRED)
find_package(imgui REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

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
        assimp::assimp 
        imgui
        objLoader
        glfw
) 

add_subdirectory(gui)
add_subdirectory(material)
add_subdirectory(object)
add_subdirectory(render)
add_subdirectory(script)
