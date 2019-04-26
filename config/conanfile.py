from conans import ConanFile, CMake

class ConanMgsConfig(ConanFile):
    name = "mgs_config"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler", "cppstd"

    def build_requirements(self):
        self.build_requires("mgs_cmake/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)

        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
