import sys

from conans.client.command import main as main_conan


PACKAGES = [
    "cmake",
    "config",
    "meta",
    "exceptions",
    "codecs",
    "config",
    "base_n",
    "base16",
    "base32",
    "base32hex",
    "base64",
    "base64url",
]


def run_conan(*args):
    try:
        main_conan(args)
    except SystemExit as e:
        if e.code != 0:
            sys.exit(e)


def main():
    for pkg in PACKAGES:
        run_conan("create", pkg, "--build", "mgs_%s" % pkg, "-s", "compiler.cppstd=14")
    run_conan("create", "all", "--build", "mgs", "-s", "compiler.cppstd=14")


if __name__ == "__main__":
    main()
