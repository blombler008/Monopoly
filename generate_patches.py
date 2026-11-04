import os
import json
import subprocess
from os.path import join, isfile, relpath, basename

PATCH_ROOT = "patches"
MODIFIED_ROOT = join(PATCH_ROOT, "modified")

OUTPUT_JSON = join(
    PATCH_ROOT,
    "patched_files.json"
)

patched_files = []

LIBRARIES = [
    "lvgl",
    "TFT_eSPI",
    "ESP32-audioI2S"
]


def find_library_file(libname, filename):
    """
    Sucht Datei rekursiv in der installierten Library.
    """

    lib_root = join(
        ".pio",
        "libdeps",
        "release",
        libname
    )

    for dirpath, _, files in os.walk(lib_root):
        if filename in files:
            return join(
                dirpath,
                filename
            )

    return None


def generate_patch(
    libname,
    filename,
    relative_path
):
    original = find_library_file(
        libname,
        filename
    )

    modified = join(
        MODIFIED_ROOT,
        libname,
        relative_path
    )

    if not original:
        print(
            f"[GEN] Original fehlt: "
            f"{libname}/{filename}"
        )
        
        empty = join(
            PATCH_ROOT,
            "__empty__"
        )

        if not isfile(empty):
            with open(empty, "w"):
                pass

        original = empty
         

    if not isfile(modified):
        print(
            f"[GEN] Modified fehlt: "
            f"{modified}"
        )

        return

    patch_name = (
        f"{libname}_"
        f"{filename.replace('.', '_')}"
        f".patch"
    )

    patch_path = join(
        PATCH_ROOT,
        patch_name
    )

    print(
        f"[GEN] Erzeuge Patch: "
        f"{patch_name}"
    )

    with open(
        patch_path,
        "w",
        encoding="utf-8",
        newline="\n"
    ) as out:

        subprocess.run(
            [
                # "git",
                "diff",
                "-u",
                # "--no-index",
                # "--ignore-cr-at-eol",
                original,
                modified
            ],
            stdout=out
        )

    patched_files.append(
        {
            "lib": libname,
            "file": filename,
            "path": relative_path.replace(
                "\\",
                "/"
            ),
            "patch": patch_name
        }
    )


def scan_lib(libname):
    root = join(
        MODIFIED_ROOT,
        libname
    )

    if not os.path.isdir(root):
        print(
            f"[GEN] Überspringe {libname} "
            f"(kein modified Ordner)"
        )
        return

    for dirpath, _, files in os.walk(root):

        for filename in files:

            full_path = join(
                dirpath,
                filename
            )

            relative_path = relpath(
                full_path,
                root
            )

            generate_patch(
                libname,
                filename,
                relative_path
            )


def main():
    print(
        "[GEN] Starte Patch-Erzeugung..."
    )

    patched_files.clear()

    for lib in LIBRARIES:
        scan_lib(lib)

    print(
        "[GEN] Speichere JSON-Liste..."
    )

    with open(
        OUTPUT_JSON,
        "w",
        encoding="utf-8"
    ) as f:
        json.dump(
            patched_files,
            f,
            indent=4
        )

    print(
        "[GEN] Fertig!"
    )


if __name__ == "__main__":
    main()