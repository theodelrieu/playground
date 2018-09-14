from conans import ConanFile, CMake

class ConanMgsMeta(ConanFile):
    name = "mgs_meta"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"

    def build_requirements(self):
        pass
        # if self.develop:
        #     self.build_requires("mgs_test_helpers/%s@mgs/testing" % self.version)
        #     self.build_requires("catch2/2.2.2@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"
        cmake.definitions["CMAKE_MODULE_PATH"] = "/home/theo/Projects/playground/cmake/Modules"
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
