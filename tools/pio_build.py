import os
from os.path import sep, join, realpath, isdir, isfile

Import("env")
global_env = DefaultEnvironment()
# global_env.SConscript("pio_apply_patches.py")


def get_lib_dir(env, lib_name=""):
    ret = None
    for src_dir in env.GetLibSourceDirs():
        if (isdir(src_dir)):
            for lib in os.listdir(realpath(src_dir)):
                lib_dir = realpath(join(src_dir, lib))
                if isdir(lib_dir) and (lib.lower().strip(" ").startswith(lib_name.strip(" ").lower())):
                    ret = lib_dir
    return ret


INCLUDE_DIR = realpath(join("..", "include"))

env.Append(
    CPPPATH=[
        join(INCLUDE_DIR, "os", "freertos"),
    ],
)


# env.Replace(SRC_FILTER=["+<*>", "-<thirdparty/>","+<thirdparty/freertos/src/>"])

# env.Replace(SRC_FILTER=["-<thirdparty/>", "-<thirdparty/freertos/portable/>", "+<thirdparty/freertos/src/>"])
