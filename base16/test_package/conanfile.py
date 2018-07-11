from conans import ConanFile, CMake, tools
import os


class ConanMgsBase16Test(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if tools.cross_building(self.settings):
            assert(os.path.exists(os.path.join("bin", "example")))
        else:
            self.run(".%sexample" % os.sep, cwd="bin")
