import os
import stat
import sys
import urllib.request


DMENV_VERSION = "0.20.0"
DMENV_URL = "https://github.com/TankerHQ/dmenv/"


def get_dmenv_filename(platform):
    if platform in ["win32", "cygwin"]:
        return "dmenv-windows.exe"
    elif platform == "darwin":
        return "dmenv-osx"
    elif platform == "linux":
        return "dmenv-linux"
    else:
        raise Exception("Unsupported plaftorm: %s" % platform)

def main():
    dmenv_filename = get_dmenv_filename(sys.platform)
    dmenv_download_url = "%s/releases/download/v%s/%s" % (DMENV_URL, DMENV_VERSION, dmenv_filename)
    response = urllib.request.urlopen(dmenv_download_url)
    data = response.read()
    if dmenv_filename == "dmenv-windows.exe":
        filename = "dmenv.exe"
    else:
        filename = "dmenv"
    with open(filename, "wb") as f:
        f.write(data)
    if sys.platform != "win32":
        os.chmod("dmenv", stat.S_IRWXU | stat.S_IRGRP | stat.S_IROTH)
    print("dmenv installed at:", os.getcwd())


if __name__ == "__main__":
    main()
