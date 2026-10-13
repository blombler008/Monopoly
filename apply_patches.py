from os.path import join, isfile
from os import walk
import json
import shutil
Import("env")  # type: ignore

PATCH_ROOT = "patches"
MODIFIED_ROOT = "patches/modified"
PATCH_JSON = join(PATCH_ROOT, "patched_files.json")

# Windows-kompatibler Patch-Befehl
PATCH_CMD = '"C:/Program Files/Git/usr/bin/patch.exe"'  # unter Linux einfach "patch"

def find_file_recursive(root, filename):
    for dirpath, _, files in walk(root):
        if filename in files:
            return join(dirpath, filename)
    return None

def ensure_file_exists(pkg_dir, libname, filename):
    modified_path = join(MODIFIED_ROOT, libname, filename)
    if not isfile(modified_path):
        print(f"[PATCH] Missing modified file: {modified_path}")
        return None

    target = find_file_recursive(pkg_dir, filename)
    if target:
        return target

    print(f"[PATCH] Creating missing file: {filename}")
    target = join(pkg_dir, filename)
    shutil.copyfile(modified_path, target)
    return target

def apply_patch(entry):
    libname = entry["lib"]
    filename = entry["file"]
    patchfile = entry["patch"]

    print(f"[PATCH] Preparing to apply {patchfile} to {libname}/{filename}")

    # Library-Pfad korrekt bestimmen
    lib_root = join(env.GetProjectOption("libdeps_dir", ".pio/libdeps"), env["PIOENV"]) # type: ignore
    pkg_dir = join(lib_root, libname)

    if not pkg_dir or not isfile(join(PATCH_ROOT, patchfile)):
        print(f"[PATCH] Missing patch or library folder for {patchfile}")
        return

    flag = join(pkg_dir, f".patched_{filename}")
    if isfile(flag):
        print(f"[PATCH] Already patched: {filename}")
        return

    patch_path = join(PATCH_ROOT, patchfile)
    target = ensure_file_exists(pkg_dir, libname, filename)

    if not target:
        print(f"[PATCH] Could not create or find {filename}")
        return

    print(f"[PATCH] Applying {patchfile} to {target}")

    # Non-interactive patch (keine Eingabe nötig)
    cmd = f'{PATCH_CMD} --force "{target}" "{patch_path}"'
    env.Execute(cmd)  # type: ignore

    with open(flag, "w") as f:
        f.write("patched")

def main():
    print("[PATCH] Loading patch list...")
    with open(PATCH_JSON, "r") as f:
        patch_list = json.load(f)

    print("[PATCH] Applying patches...")
    for entry in patch_list:
        apply_patch(entry)

    print("[PATCH] Done.")

main()
