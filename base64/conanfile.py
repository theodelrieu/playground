from conans import ConanFile, CMake

class ConanMgsBase64(ConanFile):
    name = "mgs_base64"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt", "test/*"
    settings = "os", "arch", "build_type", "compiler", "cppstd"

    # only useful for local dev (tests)
    def build_requirements(self):
        self.build_requires("mgs_cmake/%s@mgs/testing" % self.version)
        if self.develop:
            self.build_requires("mgs_test_helpers/%s@mgs/testing" % self.version)
            self.build_requires("catch2/2.2.2@bincrafters/stable")
            self.build_requires("mgs_meta/%s@mgs/testing" % self.version)

    def requirements(self):
        self.requires("mgs_binary_to_base/%s@mgs/testing" % self.version)
        self.requires("mgs_codecs/%s@mgs/testing" % self.version)
        self.requires("mgs_ranges/%s@mgs/testing" % self.version)
        self.requires("mgs_config/%s@mgs/testing" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"

        cmake.configure()
        cmake.build()
        cmake.install()

    def package_id(self):
        self.info.header_only()
