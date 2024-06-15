cmake_minimum_required(VERSION 3.10)

# Указываем имя проекта
project(Carnot)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем директории с исходными файлами и заголовочными файлами
include_directories(libs/glew/include)
include_directories(libs/glm)
include_directories(libs/imgui)
include_directories(libs/objLoader)
include_directories(libs/sfml/include)
include_directories(libs/SOIL2/src)

# Добавляем все исходные файлы
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Устанавливаем директории для выходных файлов в зависимости от конфигурации
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dir/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/dir/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dir/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/dir/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dir/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/dir/Release)

# Создаем исполняемый файл из исходных файлов
add_executable(Carnot ${SOURCES})

# Указываем пути к библиотекам и сами библиотеки
link_directories(libs/sfml/lib)
link_directories(libs/SOIL2/lib)

# Link libraries conditionally based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(Carnot PRIVATE
        kernel32.lib
        user32.lib
        gdi32.lib
        winspool.lib
        comdlg32.lib
        advapi32.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        odbc32.lib
        odbccp32.lib
        glew32s.lib
        sfml-graphics-s-d.lib
        sfml-window-s-d.lib
        sfml-system-s-d.lib
        opengl32.lib
        freetype.lib
        winmm.lib
    )
else()
    target_link_libraries(Carnot PRIVATE
        kernel32.lib
        user32.lib
        gdi32.lib
        winspool.lib
        comdlg32.lib
        advapi32.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        odbc32.lib
        odbccp32.lib
        glew32s.lib
        sfml-graphics-s.lib
        sfml-window-s.lib
        sfml-system-s.lib
        opengl32.lib
        freetype.lib
        winmm.lib
    )
endif()
