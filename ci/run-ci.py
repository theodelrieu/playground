import argparse
import os
import subprocess
import sys


from conans.client.command import main as main_conan
from path import Path


BINTRAY_BASE_URL = "https://api.bintray.com/conan/"

def run_cmake(*args):
    subprocess.run(["cmake", *args], check=True)


def run_conan(*args):
    try:
        main_conan(args)
    except SystemExit as e:
        if e.code != 0:
            sys.exit(e)


def add_conan_bintray_remote(org_remote):
    org, remote = org_remote.split("/")
    remote_url = "%s/%s/%s" % (BINTRAY_BASE_URL, org, remote)
    run_conan("remote", "add", remote, remote_url, "--force")


def add_conan_remotes():
    add_conan_bintray_remote("catchorg/Catch2")


def install_conan_workspace(profile):
    root_path = Path.getcwd().parent
    profiles_path = root_path / "ci" / "conan-profiles"
    run_conan("workspace", "install",
              root_path / "conanws.yml",
              "--profile", profiles_path / profile,
              "--build", "missing"
              )


def build_and_test(profile, cppstd):
    root_path = Path(__file__).abspath().parent.parent
    build_path = root_path / "build"
    build_path.makedirs_p()

    with build_path:
        add_conan_remotes()
        install_conan_workspace(profile)
        run_cmake("-GNinja", "..", "-DCMAKE_CXX_STANDARD=%s" % str(cppstd))
        run_cmake("--build", ".")
        run_cmake("--build", ".", "--target", "test")


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(title="subcommands", dest="command")

    build_and_test_parser = subparsers.add_parser("build-and-test")
    build_and_test_parser.add_argument("--profile", required=True)
    build_and_test_parser.add_argument("--cppstd", type=int, default=14)
    args = parser.parse_args()

    if args.command == "build-and-test":
        build_and_test(Path(args.profile), args.cppstd)
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
