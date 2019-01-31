from conans import ConanFile, CMake

class ConanMgsTestHelpers(ConanFile):
    name = "mgs_test_helpers"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt", "src/*"

    def requirements(self):
        self.requires("catch2/2.2.2@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
