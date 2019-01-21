from conans import ConanFile, CMake

class ConanMgsConcepts(ConanFile):
    name = "mgs_concepts"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"
    
    def build_requirements(self):
        self.build_requires("mgs_cmake/%s@mgs/testing" % self.version)
        if self.develop:
            self.build_requires("mgs_test_helpers/%s@mgs/testing" % self.version)
            self.build_requires("mgs_meta/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
