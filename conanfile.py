from conan import ConanFile
from conan.tools.cmake import cmake_layout


class CarnotConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps", "CMakeToolchain"

    requires = (
        "glew/2.2.0",
        "glfw/3.4",
        "imgui/cci.20230105+1.89.2.docking",
        "stb/cci.20230920",
        "assimp/5.4.1",
        "sfml/2.6.1",
        "glm/1.0.1",
    )

    def layout(self):
        cmake_layout(self)
