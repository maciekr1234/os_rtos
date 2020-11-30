
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

RTOS_INCLUDE_DIR = realpath(join("..", "include", "rtos"))
FREE_RTOS_PLUS_POSIX_PATH = realpath(join("..", "lib", "FreeRTOS-Plus-POSIX"))

global_env.Append(
    CPPDEFINES=[

        # ("OS_RTOS_CMSIS1", 0),
        # ("OS_RTOS_CMSIS2", 1),

    ],
    LINKFLAGS=[

    ]

)

env.Append(CPPPATH=[
    # join(FREE_RTOS_PLUS_POSIX_PATH, "include"),
    # join(FREE_RTOS_PLUS_POSIX_PATH, "include", "private"),
    # join(FREE_RTOS_PLUS_POSIX_PATH, "FreeRTOS-Plus-POSIX", "include"),
    # join(FREE_RTOS_PLUS_POSIX_PATH, "FreeRTOS-Plus-POSIX", "source"),
    # join(FREE_RTOS_PLUS_POSIX_PATH, "include", "FreeRTOS_POSIX"),
    #
    # include/rtos:
    #
    # join(RTOS_INCLUDE_DIR, "detail"),
    # join(RTOS_INCLUDE_DIR, "detail", "bits"),
])
