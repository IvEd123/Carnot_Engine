add_library(core
    resource.cpp
    resource.h
    resource_handle.h
    resource_manager.cpp
    resource_manager.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(assimp REQUIRED)
find_package(stb REQUIRED)

target_link_libraries( core
    PUBLIC 
        assimp::assimp 
        objLoader
        stb::stb
) 

target_link_libraries(${APP} PRIVATE core)