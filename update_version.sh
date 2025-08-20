#!/bin/bash

set -e
VERSION_FILE="resources/version.conf"
BUILD_INFO_FILE="resources/build_info.conf"
GIT_BUILD_MSG="Built from Git source at $(date)"
NON_GIT_BUILD_MSG="(*built from non-Git environment $(date), version may not be correct)"


if command -v git &> /dev/null && git rev-parse --is-inside-work-tree &> /dev/null; then

    echo "Git environment detected. Overwriting version files."
    GIT_VERSION=$(git describe --tags | sed -E 's/-g([0-9a-f]{3}).*/-g\1/')
    echo "$GIT_VERSION" > "$VERSION_FILE"
    echo "$GIT_BUILD_MSG" > "$BUILD_INFO_FILE"

else
    echo "Non-Git environment detected. Overwriting with fallback versions."
    echo "$NON_GIT_BUILD_MSG" > "$BUILD_INFO_FILE"

fi
exit 0