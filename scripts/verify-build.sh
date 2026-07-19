#!/usr/bin/env bash
#
# YummyOS Build Verifier
# Validates the ISO build and checks for common issues
#
set -e -u -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

OUT_DIR="${PROJECT_ROOT}/out"
PROFILE_DIR="${PROJECT_ROOT}/build"

ERRORS=0
WARNINGS=0

error() {
    echo "  ERROR: $1"
    ((ERRORS++))
}

warn() {
    echo "  WARNING: $1"
    ((WARNINGS++))
}

info() {
    echo "  OK: $1"
}

echo "╔══════════════════════════════════════╗"
echo "║       YummyOS Build Verifier         ║"
echo "╚══════════════════════════════════════╝"
echo ""

# ── Check required files ──────────────────────────────────────────────────────
echo "── Checking required files ──"
REQUIRED_FILES=(
    "build/airootfs/etc/hostname"
    "build/airootfs/etc/hosts"
    "build/airootfs/root/customize_airootfs.sh"
    "build/pacman.conf/pacman.conf"
    "build/packages.x86_64"
    "build/profiledef.sh"
    "build/efiboot/entries/yummyos.conf"
    "config/etc/skel/.zshrc"
    "config/etc/skel/.bashrc"
)

for file in "${REQUIRED_FILES[@]}"; do
    FULL="${PROJECT_ROOT}/${file}"
    if [ -f "${FULL}" ]; then
        info "${file}"
    else
        error "Missing: ${file}"
    fi
done
echo ""

# ── Validate package list ─────────────────────────────────────────────────────
echo "── Checking packages.x86_64 ──"
PKG_FILE="${PROFILE_DIR}/packages.x86_64"
if [ -f "${PKG_FILE}" ]; then
    PKG_COUNT=$(grep -v '^#' "${PKG_FILE}" | grep -v '^$' | wc -l)
    info "${PKG_COUNT} packages defined"

    # Check for essential packages
    ESSENTIAL_PKGS=("base" "linux" "systemd-boot" "plasma-desktop" "sddm" "firefox" "pipewire" "networkmanager")
    for pkg in "${ESSENTIAL_PKGS[@]}"; do
        if grep -q "^${pkg}$" "${PKG_FILE}"; then
            info "Essential package present: ${pkg}"
        else
            warn "Essential package missing: ${pkg}"
        fi
    done
else
    error "Package list not found"
fi
echo ""

# ── Check profiledef.sh syntax ────────────────────────────────────────────────
echo "── Checking profiledef.sh ──"
PD_FILE="${PROFILE_DIR}/profiledef.sh"
if [ -f "${PD_FILE}" ]; then
    if bash -n "${PD_FILE}" 2>/dev/null; then
        info "profiledef.sh syntax valid"
    else
        error "profiledef.sh has syntax errors"
    fi
fi
echo ""

# ── Check customize_airootfs.sh syntax ────────────────────────────────────────
echo "── Checking customize_airootfs.sh ──"
CAI_FILE="${PROFILE_DIR}/airootfs/root/customize_airootfs.sh"
if [ -f "${CAI_FILE}" ]; then
    if bash -n "${CAI_FILE}" 2>/dev/null; then
        info "customize_airootfs.sh syntax valid"
    else
        error "customize_airootfs.sh has syntax errors"
    fi

    # Check for essential commands
    ESSENTIAL_CMDS=("systemctl enable" "useradd" "locale-gen" "pacman -Scc")
    for cmd in "${ESSENTIAL_CMDS[@]}"; do
        if grep -q "${cmd}" "${CAI_FILE}"; then
            info "Contains: ${cmd}"
        else
            warn "Missing command: ${cmd}"
        fi
    done
fi
echo ""

# ── Check ISO output ──────────────────────────────────────────────────────────
echo "── Checking ISO output ──"
if [ -d "${OUT_DIR}" ]; then
    ISO_FILE=$(find "${OUT_DIR}" -name "yummyos-*.iso" -type f 2>/dev/null | head -1)
    if [ -n "${ISO_FILE}" ]; then
        ISO_SIZE=$(du -h "${ISO_FILE}" | cut -f1)
        info "ISO found: ${ISO_FILE}"
        info "ISO size: ${ISO_SIZE}"

        # Check ISO size (should be > 1GB for a full KDE ISO)
        ISO_SIZE_BYTES=$(stat -c%s "${ISO_FILE}" 2>/dev/null || stat -f%z "${ISO_FILE}" 2>/dev/null || echo "0")
        if [ "${ISO_SIZE_BYTES}" -lt 1073741824 ]; then
            warn "ISO is smaller than 1GB - may be incomplete"
        fi

        # Verify ISO magic bytes
        MAGIC=$(xxd -l 32 "${ISO_FILE}" 2>/dev/null | head -1)
        if echo "${MAGIC}" | grep -q "EL TORITO"; then
            info "Valid ISO 9660/El Torito header detected"
        fi
    else
        warn "No ISO file found in ${OUT_DIR}"
    fi
else
    info "Output directory not found (run build first)"
fi
echo ""

# ── Summary ───────────────────────────────────────────────────────────────────
echo "════════════════════════════════════════"
echo "  Results: ${ERRORS} errors, ${WARNINGS} warnings"
if [ "${ERRORS}" -eq 0 ]; then
    echo "  Build verification PASSED"
else
    echo "  Build verification FAILED"
    exit 1
fi
echo "════════════════════════════════════════"