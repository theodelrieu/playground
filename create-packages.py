import sys

from conans.client.command import main as main_conan


PACKAGES = [
    "cmake",
    "config",
    "test_helpers",
    "meta",
    "exceptions",
    "ranges",
    "codecs",
    "binary_to_base",
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
        run_conan("create", pkg, "mgs/testing", "--build", "mgs_%s" % pkg, "-s", "cppstd=14")
    run_conan("create", "all", "mgs/testing", "--build", "mgs", "-s", "cppstd=14")


if __name__ == "__main__":
    main()
