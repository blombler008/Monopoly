from os.path import join, isfile
import json

Import("env")  # type: ignore

PATCH_ROOT = "patches"

PATCH_JSON = join(
    PATCH_ROOT,
    "patched_files.json"
)

PATCH_CMD = (
    '"C:/Program Files/Git/usr/bin/patch.exe"'
)
GIT_CMD = (
    '"C:/Program Files/Git/bin/git.exe"'
)

def apply_patch(entry):
    libname = entry["lib"]
    filename = entry["file"]
    relpath = entry["path"]
    patchfile = entry["patch"]

    print(
        f"[PATCH] Preparing to apply "
        f"{patchfile} to "
        f"{libname}/{relpath}"
    )

    lib_root = join(
        env.GetProjectOption(
            "libdeps_dir",
            ".pio/libdeps"
        ),
        env["PIOENV"]
    )

    pkg_dir = join(
        lib_root,
        libname
    )

    patch_path = join(
        PATCH_ROOT,
        patchfile
    )

    target = join(
        pkg_dir,
        *relpath.split("/")
    )

    if not isfile(patch_path): 
    
        print(
            f"[PATCH] Creating "
            f"{target}"
        )

        os.makedirs(
            os.path.dirname(target),
            exist_ok=True
        )

        with open(
            target,
            "w",
            encoding="utf-8"
        ):
            pass

    flag_name = (
        relpath
        .replace("/", "_")
        .replace("\\", "_")
        .replace(".", "_")
    )

    flag = join(
        pkg_dir,
        f".patched_{flag_name}"
    )

    if isfile(flag):
        print(
            f"[PATCH] Already patched: "
            f"{relpath}"
        )
        return

    print(
        f"[PATCH] Applying "
        f"{patchfile} to "
        f"{target}"
    )

    # using: this instaed of the other one@ 
    # cmd = (
    #     f'{GIT_CMD} '
    #     f'apply '
    #     f'--ignore-space-change '
    #     f'--ignore-whitespace '
    #     f'"{patch_path}"'
    # )

    cmd = (
        f'{PATCH_CMD} '
        f'--binary '
        f'--force '
        f'"{target}" '
        f'"{patch_path}"'
    )

    result = env.Execute(cmd)  # type: ignore

    if result == 0:
        with open(flag, "w") as f:
            f.write("patched")


def main():
    print(
        "[PATCH] Loading patch list..."
    )

    if not isfile(PATCH_JSON):
        print(
            f"[PATCH] Missing "
            f"{PATCH_JSON}"
        )
        return

    with open(
        PATCH_JSON,
        "r",
        encoding="utf-8"
    ) as f:
        patch_list = json.load(f)

    print(
        "[PATCH] Applying patches..."
    )

    for entry in patch_list:
        apply_patch(entry)

    print(
        "[PATCH] Done."
    )


main()