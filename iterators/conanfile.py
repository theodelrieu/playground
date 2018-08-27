from conans import ConanFile, CMake

class ConanMgsIterators(ConanFile):
    name = "mgs_iterators"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"

    def build_requirements(self):
        if self.develop:
            self.build_requires("mgs_test_helpers/%s@mgs/testing" % self.version)

    def requirements(self):
        self.requires("mgs_meta/%s@mgs/testing" % self.version)
        self.requires("mgs_adapters_base/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
