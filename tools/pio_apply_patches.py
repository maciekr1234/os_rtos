import subprocess
import sys
import re
from os import listdir, makedirs, path
from os.path import join, realpath, isdir, isfile
from shutil import move, copy2

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
platform = env.PioPlatform()


FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoststm32")
assert isdir(FRAMEWORK_DIR)

CATEGORY_RE = r"[0-9]+\-(\w+)"

category_path = {
    "framework": realpath(FRAMEWORK_DIR)
}

patches = []


for file in listdir(join(".", "patches")):
    if not file.endswith(".patch"):
        continue
    _dir = category_path.get(re.findall(CATEGORY_RE, file)[-1])
    if not isdir(_dir):
        continue
    patches.append(
        (path.abspath(_dir),
         path.abspath(join(".", "patches", file)))
    )


print(patches)

for patch in patches:
    try:
        status = subprocess.check_output(
            [
                "git", "-C", patch[0], "apply", "--check", patch[1]
            ], stderr=subprocess.STDOUT,)
        if status:
            print("patch {} can't be applied".format(patch[1]))
            continue

        status = subprocess.check_output(
            [
                "git", "-C", patch[0], "apply", patch[1]
            ]
        )

    except subprocess.CalledProcessError as e:
        continue
        # print("Failed command: {}".format(e.cmd))
