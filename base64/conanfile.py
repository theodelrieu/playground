from conans import ConanFile, CMake

class ConanMgsBase64(ConanFile):
    name = "mgs_base64"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"

    # only useful for local dev
    def build_requirements(self):
        self.build_requires("mgs_test_helpers/%s@mgs/testing" % self.version)

    def requirements(self):
        self.requires("mgs_meta/%s@mgs/testing" % self.version)
        self.requires("mgs_base_n/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()