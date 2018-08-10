from conans import ConanFile

class ConanMgsAll(ConanFile):
    generators = "cmake"

    def build_requirements(self):
        self.build_requires("catch2/2.2.2@bincrafters/stable")

    def requirements(self):
        version = "0.1"

        self.requires("mgs_base64/%s@mgs/testing" % version)
        self.requires("mgs_base64url/%s@mgs/testing" % version)
        self.requires("mgs_base32/%s@mgs/testing" % version)
        self.requires("mgs_base32hex/%s@mgs/testing" % version)
        self.requires("mgs_base16/%s@mgs/testing" % version)
        # TODO remove this from all? We'll see later on if users should use it directly or not
        self.requires("mgs_codecs_base/%s@mgs/testing" % version)
