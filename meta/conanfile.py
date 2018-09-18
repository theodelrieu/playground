from conans import ConanFile, CMake

class ConanMgsMeta(ConanFile):
    name = "mgs_meta"
    version = "0.1"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"

    def build_requirements(self):
        self.build_requires("mgs_cmake/%s@mgs/testing" % self.version)
        if self.develop:
            self.build_requires("mgs_test_helpers/%s@mgs/testing" % self.version)
            self.build_requires("catch2/2.2.2@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        build_tests = self.develop and self.should_build
        if not build_tests:
            cmake.definitions["BUILD_TESTING"] = "OFF"
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
