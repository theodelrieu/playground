from conans import ConanFile, CMake

class ConanMgsTestHelpers(ConanFile):
    name = "mgs_test_helpers"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"

    def requirements(self):
        self.requires("catch2/2.2.2@bincrafters/stable")
        self.requires("mgs_base_n/%s@mgs/testing" % self.version)
        self.requires("mgs_meta/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
