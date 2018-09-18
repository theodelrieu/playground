from conans import ConanFile

class ConanMgsAll(ConanFile):
    name = "mgs"
    version = "0.1"

    def requirements(self):
        self.requires("mgs_binary_to_text/%s@mgs/testing" % self.version)
        self.requires("mgs_base64/%s@mgs/testing" % self.version)
        self.requires("mgs_base64url/%s@mgs/testing" % self.version)
        self.requires("mgs_base32/%s@mgs/testing" % self.version)
        self.requires("mgs_base32hex/%s@mgs/testing" % self.version)
        self.requires("mgs_base16/%s@mgs/testing" % self.version)
