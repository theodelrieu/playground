from conans import ConanFile, CMake

class ConanMgsRanges(ConanFile):
    name = "mgs_ranges"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler", "cppstd"

    def build_requirements(self):
        self.build_requires("mgs_cmake/%s" % self.version)
        if self.develop:
            self.build_requires("mgs_test_helpers/%s" % self.version)
            self.build_requires("Catch2/2.7.2@catchorg/stable")

    def requirements(self):
        self.requires("mgs_meta/%s" % self.version)
        self.requires("mgs_config/%s" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"

        cmake.configure()
        cmake.build()
        cmake.install()

    def package_id(self):
        self.info.header_only()
