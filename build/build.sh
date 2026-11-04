#!/bin/bash
cd "$(dirname "$0")/.."
clang++ -std=c++20 -O2 -Wall -Wno-comment -Iinclude -Isrc/update \
    main.cpp src/memory/memory.cpp src/memory/instance.cpp \
    -o build/violations
codesign --force --sign - --entitlements signing/entitlements.plist --timestamp=none build/violations
codesign --force --sign - --options runtime --entitlements signing/entitlements.plist --timestamp=none /Applications/Roblox.app
