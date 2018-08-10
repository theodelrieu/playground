from conans import ConanFile, CMake

class ConanMgsAdaptersBase(ConanFile):
    name = "mgs_adapters_base"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"

    def requirements(self):
        self.requires("mgs_meta/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
