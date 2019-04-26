from conans import ConanFile, CMake

class ConanMgsTestHelpers(ConanFile):
    name = "mgs_test_helpers"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt", "src/*"
    settings = "os", "arch", "build_type", "compiler", "cppstd"

    def requirements(self):
        self.requires("catch2/2.2.2@bincrafters/stable")
        self.requires("mgs_config/%s@mgs/testing" % self.version)

    def build_requirements(self):
        self.build_requires("mgs_cmake/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
