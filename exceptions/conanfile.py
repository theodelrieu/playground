from conans import ConanFile, CMake

class ConanMgsExceptions(ConanFile):
    name = "mgs_exceptions"
    version = "0.1"
    generators = "cmake"
    requires = None
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
