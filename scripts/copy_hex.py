import shutil
from os.path import join

Import("env")

def copy_hex_file(source, target, env):
    hex_path = join(env.subst("$BUILD_DIR"), env.subst("$PROGNAME") + ".hex")
    dest_path = join(env.subst("$PROJECT_DIR"), "sim_circuit/proteus", env.subst("$PROGNAME") + ".hex")
    shutil.copyfile(hex_path, dest_path)
    print("Copied HEX to: " + dest_path)

env.AddPostAction("$BUILD_DIR/${PROGNAME}.hex", copy_hex_file)
