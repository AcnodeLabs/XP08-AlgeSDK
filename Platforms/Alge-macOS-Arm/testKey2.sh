#!/bin/bash

APP_NAME="TextEdit"
SCRIPT_PATH="/Users/aliveb/REPOS/XP08-AlgeSDK/Platforms/Alge-macOS-Arm/send-key-and-restore.applescript"

for ((i=1; i<=1; i++)); do
    for letter in {a..g}; do
        time /usr/bin/osascript "$SCRIPT_PATH" "$APP_NAME" "$letter"
    done
done
