from conans import ConanFile, CMake

class ConanMgs(ConanFile):
    name = "mgs"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt", "test/*"
    settings = "os", "build_type", "arch", "compiler", "cppstd"

    def requirements(self):
        self.requires("mgs_base16/%s@mgs/testing" % self.version)
        self.requires("mgs_base32/%s@mgs/testing" % self.version)
        self.requires("mgs_base32hex/%s@mgs/testing" % self.version)
        self.requires("mgs_base64/%s@mgs/testing" % self.version)
        self.requires("mgs_base64url/%s@mgs/testing" % self.version)
        self.requires("mgs_exceptions/%s@mgs/testing" % self.version)
        self.requires("mgs_cmake/%s@mgs/testing" % self.version)
        self.requires("mgs_binary_to_base/%s@mgs/testing" % self.version)
        self.requires("mgs_codecs/%s@mgs/testing" % self.version)
        self.requires("mgs_config/%s@mgs/testing" % self.version)
        self.requires("mgs_ranges/%s@mgs/testing" % self.version)

    def build(self):
        pass

    def package(self):
        pass

    def package_id(self):
        self.info.header_only()
