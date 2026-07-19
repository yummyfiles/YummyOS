#!/usr/bin/env bash
#
# YummyOS Build Cleaner
# Removes all build artifacts and temporary files
#
set -e -u -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "╔══════════════════════════════════════╗"
echo "║       YummyOS Build Cleaner          ║"
echo "╚══════════════════════════════════════╝"
echo ""
echo "  Cleaning project: ${PROJECT_ROOT}"
echo ""

# Directories to clean
CLEAN_DIRS=(
    "${PROJECT_ROOT}/build/iso"
    "${PROJECT_ROOT}/out"
    "${PROJECT_ROOT}/build/airootfs/var"
    "${PROJECT_ROOT}/build/airootfs/tmp"
)

for dir in "${CLEAN_DIRS[@]}"; do
    if [ -d "${dir}" ]; then
        echo "  Removing: ${dir}"
        rm -rf "${dir}"
    fi
done

# Remove generated files
CLEAN_FILES=(
    "${PROJECT_ROOT}/build/airootfs/etc/machine-id"
    "${PROJECT_ROOT}/build/airootfs/etc/shadow"
    "${PROJECT_ROOT}/build/airootfs/etc/gshadow"
)

for file in "${CLEAN_FILES[@]}"; do
    if [ -f "${file}" ]; then
        echo "  Removing: ${file}"
        rm -f "${file}"
    fi
done

# Remove any leftover squashfs images
find "${PROJECT_ROOT}" -name "*.fsimg" -delete 2>/dev/null || true
find "${PROJECT_ROOT}" -name "*.iso" -delete 2>/dev/null || true

echo ""
echo "Done! Build directory cleaned."
echo ""