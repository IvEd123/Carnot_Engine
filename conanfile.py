import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy

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

    def generate(self):
        to_copy = ["*glfw*", "*opengl3*"]

        for dep in to_copy:
            copy(self, dep, os.path.join(self.dependencies["imgui"].package_folder,
                "res", "bindings"), os.path.join(self.source_folder, "libs", "imgui_backend"))

    def layout(self):
        cmake_layout(self)
