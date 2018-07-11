from conans import ConanFile

class ConanMgsAll(ConanFile):
    generators = "cmake"

    def build_requirements(self):
        self.build_requires("catch2/2.2.2@bincrafters/stable")

    def requirements(self):
        version = "0.1"

        self.requires("mgs_base64/%s@mgs/testing" % version)
        self.requires("mgs_iterators/%s@mgs/testing" % version)
