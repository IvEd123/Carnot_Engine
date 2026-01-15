
find_package(glm CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

set(APP "${PROJECT_NAME}_app")

add_executable(${APP})

target_sources(${APP}
    PRIVATE
        main.cpp
)

add_custom_command(
    TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${APP}>/resources
)

target_compile_definitions(${APP} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)

target_link_libraries( ${APP}
    PUBLIC 
        GLEW::GLEW 
        glm::glm 
        imgui::imgui
        glfw
) 

add_subdirectory(gui)
add_subdirectory(material)
add_subdirectory(object)
add_subdirectory(render)
add_subdirectory(script)

add_subdirectory(core)
