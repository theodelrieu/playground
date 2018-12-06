import sys

from conans.client.command import main as main_conan


PACKAGES = [
    "adapters",
    "adapters_base",
    "base16",
    "base32",
    "base32hex",
    "base64",
    "base64url",
    "binary_to_text",
    "cmake",
    "codecs_base",
    "exceptions",
    "meta",
    "test_helpers",
]


def run_conan(*args):
    try:
        main_conan(args)
    except SystemExit as e:
        if e.code != 0:
            sys.exit(e)


def main():
    for pkg in PACKAGES:
        run_conan("export", pkg, "mgs/testing")
    run_conan("create", "all", "mgs/testing", "--build")


if __name__ == "__main__":
    main()
