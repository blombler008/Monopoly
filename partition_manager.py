Import('env') # type: ignore
import os.path
import sys

# add esp32partitiontool to path
sys.path.append(os.path.abspath( env.PioPlatform().get_package_dir("tool-esp32partitiontool") )) # type: ignore
# import module
from esp32partitiontool import * # type: ignore
# run module
load_pm(env) # type: ignore