import argparse
import os
import subprocess
import sys

from path import Path


def run_cmd(*args):
    subprocess.run([*args], check=True)


def install_conan_workspace():
    cwd = Path.getcwd()
    run_cmd("conan", "workspace", "install", cwd.parent / "conanws.yml")


def run_cmake(*args):
    run_cmd("cmake", *args)


def build_and_test():
    root_path = Path(__file__).abspath().parent.parent
    build_path = root_path / "build"
    build_path.makedirs_p()

    with build_path:
        install_conan_workspace()
        run_cmake("-GNinja", "..")
        run_cmake("--build", ".")
        run_cmake("--build", ".", "--target", "test")


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(title="subcommands", dest="command")

    subparsers.add_parser("build-and-test")
    args = parser.parse_args()

    if args.command == "build-and-test":
        build_and_test()
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
