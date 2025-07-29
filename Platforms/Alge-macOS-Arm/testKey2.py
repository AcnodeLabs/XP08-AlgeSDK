#!/usr/local/bin/python3
import subprocess
import sys

subprocess.run(["/usr/bin/osascript", "/Users/aliveb/REPOS/XP08-AlgeSDK/Platforms/Alge-macOS-Arm/send-key-and-restore.applescript", sys.argv[1], sys.argv[2]])

