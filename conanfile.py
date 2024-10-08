from conan import ConanFile
from conan.tools.cmake import cmake_layout

class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def configure(self):
        if self.settings.build_type != "Release":
            self.settings.build_type = "Release"

    def requirements(self):
        self.requires("glm/cci.20230113")
        self.requires("glfw/3.4")

    def layout(self):
        cmake_layout(self)
    