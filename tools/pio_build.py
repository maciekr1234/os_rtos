
from os.path import join, realpath

Import("env")
global_env = DefaultEnvironment()

# private library flags
# for item in env.get("CPPDEFINES", []):
#     if isinstance(item, tuple) and item[0] == "HAL":
#         env.Append(CPPPATH=[realpath(join("hal", item[1]))])
#         env.Replace(SRC_FILTER=["+<*>", "-<hal>", "+<%s>" % join("hal", item[1])])
#         break


# pass flags to a global build environment (for all libraries, etc)
INCLUDE_DIR = realpath(join("..","include"))
SRC_DIR = realpath(join("..","src"))
RTOS_INCLUDE_DIR = join(INCLUDE_DIR, "rtos")

global_env.Append(
    BUILD_UNFLAGS=[
        "-std=gnu++14", "-std=gnu11",
    ],
    BUILD_FLAGS=[
        "-std=gnu++2a", "-std=gnu17",
    ],
    CPPDEFINES=[

        # ("OS_RTOS_CMSIS1", 0),
        # ("OS_RTOS_CMSIS2", 1),

    ],
    LINKFLAGS=[
            "-Wl,--wrap=malloc",
            "-Wl,--wrap=_malloc_r"
    ]

)

env.Append(CPPPATH=[


])
