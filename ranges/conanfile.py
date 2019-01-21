from conans import ConanFile, CMake

class ConanMgsRanges(ConanFile):
    name = "mgs_ranges"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"

    def build_requirements(self):
        self.build_requires("mgs_cmake/%s@mgs/testing" % self.version)
        if self.develop:
            self.build_requires("mgs_test_helpers/%s@mgs/testing" % self.version)

    def requirements(self):
        self.requires("mgs_meta/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        build_tests = self.develop and self.should_build
        cmake.definitions["BUILD_TESTING"] = build_tests
        if self.should_configure:
            cmake.configure()
        if build_tests:
            cmake.build()
        if self.develop and self.should_test:
            cmake.test()
        if self.should_install:
            cmake.install()

    def package_id(self):
        self.info.header_only()