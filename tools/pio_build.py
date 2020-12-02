import os
from os.path import join, realpath, isdir, isfile

env = DefaultEnvironment()


def get_lib_dir(env, lib_name=""):
    ret = None
    for src_dir in env.GetLibSourceDirs():
        if (isdir(src_dir)):
            for lib in os.listdir(realpath(src_dir)):
                lib_dir = realpath(join(src_dir, lib))
                if isdir(lib_dir) and (lib.lower().strip(" ").startswith(lib_name.strip(" ").lower())):
                    ret = lib_dir
    return ret


# private library flags
# for item in env.get("CPPDEFINES", []):
#     if isinstance(item, tuple) and item[0] == "HAL":
#         env.Append(CPPPATH=[realpath(join("hal", item[1]))])
#         env.Replace(SRC_FILTER=["+<*>", "-<hal>", "+<%s>" % join("hal", item[1])])
#         break
def apply_patches():
    STM32DUINO_FREERTOS_DIR = env.get_lib_dir("STM32duino FreeRTOS")
    PATCHES = [
        (join(STM32DUINO_FREERTOS_DIR, "portable", "MemMang",
              "heap_useNewlib.c"), "heap_useNewlib.patch")
    ]
    for patch in PATCHES:
        if not isinstance(patch, tuple):
            continue

        if isfile(patch[0]) and isfile(patch[1]):
            env.Execute("patch -s -f \"%s\" %s" % (patch[0], patch[1]))


# print("apply_patches")
# apply_patches()


INCLUDE_DIR = realpath(join("..", "include"))
RTOS_INCLUDE_DIR = join(INCLUDE_DIR, "rtos")

# """
#   "-Wl,--wrap=malloc",
#   "-Wl,--wrap=_malloc_r",
#   "-Wl,--wrap=realloc",
#   "-Wl,--wrap=_realloc_r",
#   "-Wl,--wrap=calloc",
#   "-Wl,--wrap=_calloc_r",
#   "-Wl,--wrap=free" ,
#   "-Wl,--wrap=_free_r",
# "-Wl,--wrap=pvPortMalloc",
# "-Wl,--wrap=vPortFree",
# """

env.Append(
    BUILD_UNFLAGS=[
        # "-std=gnu++14", "-std=gnu11",
    ],
    BUILD_FLAGS=[
        # "-std=gnu++2a", "-std=gnu17",
    ],
    CPPDEFINES=[

        # ("OS_RTOS_CMSIS1", 0),
        # ("OS_RTOS_CMSIS2", 1),

    ],
    LINKFLAGS=[
        # "-Wl,--wrap=malloc",
        # "-Wl,--wrap=_malloc_r",

    ]

)

env.Append(CPPPATH=[


])
