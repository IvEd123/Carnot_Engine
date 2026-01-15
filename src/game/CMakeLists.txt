target_sources(${APP}
    PRIVATE
        scene_loader.cpp
        scene_loader.h
)

target_include_directories(${APP} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
