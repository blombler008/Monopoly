import os
import json
from os.path import join, isfile
import subprocess

PATCH_ROOT = "patches"
ORIGINAL_ROOT = "patches/original"
MODIFIED_ROOT = "patches/modified"

OUTPUT_JSON = join(PATCH_ROOT, "patched_files.json")

patched_files = []

def generate_patch(libname, filename):
    original = join(ORIGINAL_ROOT, libname, filename)
    modified = join(MODIFIED_ROOT, libname, filename)

    if not isfile(original):
        print(f"[GEN] Original fehlt: {original}")
        return

    if not isfile(modified):
        print(f"[GEN] Modified fehlt: {modified}")
        return

    patch_name = f"{libname}_{filename.replace('.', '_')}.patch"
    patch_path = join(PATCH_ROOT, patch_name)

    print(f"[GEN] Erzeuge Patch: {patch_name}")

    # diff -u original modified > patch
    with open(patch_path, "w") as out:
        subprocess.run(["diff", "-u", original, modified], stdout=out)

    patched_files.append({
        "lib": libname,
        "file": filename,
        "patch": patch_name
    })

def scan_lib(libname):
    lib_original = join(ORIGINAL_ROOT, libname)
    for filename in os.listdir(lib_original):
        generate_patch(libname, filename)

def main():
    print("[GEN] Starte Patch-Erzeugung...")

    scan_lib("lvgl")
    scan_lib("TFT_eSPI")
    scan_lib("ESP32-audioI2S")

    print("[GEN] Speichere JSON-Liste...")

    with open(OUTPUT_JSON, "w") as f:
        json.dump(patched_files, f, indent=4)

    print("[GEN] Fertig!")

if __name__ == "__main__":
    main()
