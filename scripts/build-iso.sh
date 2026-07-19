#!/usr/bin/env bash
#
# YummyOS ISO Build Script
# Builds a bootable ISO image using archiso
#
set -e -u -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

ISO_DIR="${PROJECT_ROOT}/build/iso"
OUT_DIR="${PROJECT_ROOT}/out"
PROFILE_DIR="${PROJECT_ROOT}/build"

ARCHISO_PATH="/usr/share/archiso"

usage() {
    cat <<EOF
Usage: $(basename "$0") [OPTIONS]

Build a YummyOS ISO image.

Options:
    -o DIR      Output directory (default: ${OUT_DIR})
    -c PROFILE  Profile directory (default: ${PROFILE_DIR})
    -h          Show this help message
EOF
    exit 0
}

while getopts "o:c:h" opt; do
    case "${opt}" in
        o) OUT_DIR="${OPTARG}" ;;
        c) PROFILE_DIR="${OPTARG}" ;;
        h) usage ;;
        *) usage ;;
    esac
done

echo "╔══════════════════════════════════════╗"
echo "║       YummyOS ISO Builder           ║"
echo "╚══════════════════════════════════════╝"
echo ""
echo "  Project root : ${PROJECT_ROOT}"
echo "  Profile dir  : ${PROFILE_DIR}"
echo "  Output dir   : ${OUT_DIR}"
echo ""

# Ensure output directory exists
mkdir -p "${OUT_DIR}"

# Check for archiso
if [ ! -d "${ARCHISO_PATH}" ]; then
    echo "ERROR: archiso not found at ${ARCHISO_PATH}"
    echo "Install with: sudo pacman -S archiso"
    exit 1
fi

# Copy archiso configs
echo "[1/5] Copying archiso configuration..."
mkdir -p "${ISO_DIR}/archiso"
cp "${ARCHISO_PATH}/configs/releng/"*.sh "${ISO_DIR}/archiso/" 2>/dev/null || true
cp "${ARCHISO_PATH}/configs/releng/"*.conf "${ISO_DIR}/archiso/" 2>/dev/null || true
cp -r "${ARCHISO_PATH}/configs/releng/"airootfs/ "${ISO_DIR}/archiso/" 2>/dev/null || true

# Merge project airootfs with archiso airootfs
echo "[2/5] Merging airootfs directories..."
if [ -d "${PROFILE_DIR}/airootfs" ]; then
    cp -r "${PROFILE_DIR}/airootfs/." "${ISO_DIR}/archiso/airootfs/"
fi

# Copy package list
echo "[3/5] Copying package list..."
cp "${PROFILE_DIR}/packages.x86_64" "${ISO_DIR}/archiso/packages.x86_64"

# Copy profiledef
echo "[4/5] Copying profile definition..."
cp "${PROFILE_DIR}/profiledef.sh" "${ISO_DIR}/archiso/profiledef.sh"

# Copy pacman.conf
if [ -f "${PROFILE_DIR}/pacman.conf/pacman.conf" ]; then
    mkdir -p "${ISO_DIR}/archiso/airootfs/etc"
    cp "${PROFILE_DIR}/pacman.conf/pacman.conf" "${ISO_DIR}/archiso/airootfs/etc/pacman.conf"
fi

# Copy EFI boot entries
if [ -d "${PROFILE_DIR}/efiboot" ]; then
    cp -r "${PROFILE_DIR}/efiboot/." "${ISO_DIR}/archiso/efiboot/"
fi

# Build the ISO
echo "[5/5] Building ISO image..."
cd "${ISO_DIR}/archiso"

mkarchiso \
    -w "${ISO_DIR}/work" \
    -D "${ISO_DIR}/efiboot" \
    -o "${OUT_DIR}" \
    "${ISO_DIR}/archiso"

echo ""
echo "╔══════════════════════════════════════╗"
echo "║       Build Complete!                ║"
echo "╚══════════════════════════════════════╝"
echo ""

ISO_FILE=$(find "${OUT_DIR}" -name "yummyos-*.iso" -type f | head -1)
if [ -n "${ISO_FILE}" ]; then
    ISO_SIZE=$(du -h "${ISO_FILE}" | cut -f1)
    echo "  ISO: ${ISO_FILE}"
    echo "  Size: ${ISO_SIZE}"
else
    echo "WARNING: No ISO file found in ${OUT_DIR}"
fi