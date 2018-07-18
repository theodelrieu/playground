from conans import ConanFile, CMake

class ConanMgsBaseN(ConanFile):
    name = "mgs_base_n"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"

    def requirements(self):
        self.requires("mgs_meta/%s@mgs/testing" % self.version)
        self.requires("mgs_exceptions/%s@mgs/testing" % self.version)
        self.requires("boost_container/1.66.0@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
