#!/usr/bin/env bash
# YummyOS Wallpaper Generator
# Requires: ImageMagick 7+ (magick/convert)
#
# Usage: ./generate-wallpapers.sh [output_dir]
# Default output: config/usr/share/wallpapers/YummyOS/contents/images/

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT_BASE="${1:-$SCRIPT_DIR/../../config/usr/share/wallpapers/YummyOS/contents/images}"

ACCENT="#4CC9F0"
SECONDARY="#7B2CBF"
DARK_START="#0a0a1a"
DARK_END="#1a1a2e"
LIGHT_BG="#f0f4f8"

# Verify ImageMagick is installed
if ! command -v convert &>/dev/null && ! command -v magick &>/dev/null; then
    echo "Error: ImageMagick is required but not found."
    echo "Install it with: sudo apt install imagemagick"
    exit 1
fi

# Use 'convert' or 'magick' depending on installation
CONVERT="convert"
if command -v magick &>/dev/null; then
    CONVERT="magick"
fi

mkdir -p "$OUTPUT_BASE"

echo "=== YummyOS Wallpaper Generator ==="
echo "Output directory: $OUTPUT_BASE"
echo ""

# ---------------------------------------------------------------
# Helper: generate abstract geometric elements on a transparent canvas
# then composite onto a gradient background.
# ---------------------------------------------------------------

generate_dark_wallpaper() {
    local width="$1"
    local height="$2"
    local label="${width}x${height}"
    local out="$OUTPUT_BASE/yummyos-dark-${label}.png"

    echo "Generating dark wallpaper: $label ..."

    local scale_w=$(( width / 20 ))
    local scale_h=$(( height / 20 ))
    local min_dim=$(( scale_w < scale_h ? scale_w : scale_h ))

    local font_size_large=$(( min_dim * 3 ))
    local font_size_small=$(( min_dim ))

    $CONVERT \
        -size "${width}x${height}" \
        gradient:"${DARK_START}-${DARK_END}" \
        -size "${width}x${height}" \
        \( -size "${width}x${height}" xc:none \
           -fill "${ACCENT}22" \
           -draw "circle $(( width * 3 / 10 )),$(( height * 2 / 5 )) $(( width * 3 / 10 + min_dim * 4 )),$(( height * 2 / 5 + min_dim * 4 ))" \
           -fill "${SECONDARY}18" \
           -draw "circle $(( width * 7 / 10 )),$(( height * 3 / 5 )) $(( width * 7 / 10 + min_dim * 5 )),$(( height * 3 / 5 + min_dim * 5 ))" \
           -fill "${ACCENT}15" \
           -draw "circle $(( width * 1 / 5 )),$(( height * 4 / 5 )) $(( width * 1 / 5 + min_dim * 3 )),$(( height * 4 / 5 + min_dim * 3 ))" \
           -stroke "${ACCENT}30" -strokewidth 2 \
           -draw "line $(( width / 10 )),$(( height / 8 )) $(( width * 9 / 10 )),$(( height * 7 / 8 ))" \
           -draw "line $(( width * 2 / 10 )),$(( height * 7 / 8 )) $(( width * 8 / 10 )),$(( height / 8 ))" \
           -stroke "${SECONDARY}20" -strokewidth 3 \
           -draw "polyline $(( width / 4 )),$(( height / 4 )) $(( width * 3 / 8 )),$(( height / 6 )) $(( width / 2 )),$(( height / 4 ))" \
           -stroke "${ACCENT}25" -strokewidth 1 \
           -draw "line $(( width / 2 )),$(( height / 3 )) $(( width * 3 / 4 )),$(( height * 2 / 3 ))" \
           -draw "line $(( width / 3 )),$(( height / 2 )) $(( width * 2 / 3 )),$(( height / 3 ))" \
           -fill "${SECONDARY}10" \
           -draw "rectangle $(( width * 3 / 5 )),$(( height * 1 / 6 )) $(( width * 4 / 5 )),$(( height * 1 / 3 ))" \
        \) -composite \
        \( -size "${width}x${height}" xc:none \
           -fill "white" -font Helvetica-Bold \
           -pointsize "$font_size_large" \
           -gravity SouthEast \
           -annotate -"$(( min_dim ))x$(( min_dim * 2 ))+0" "YummyOS" \
           -fill "${ACCENT}40" -font Helvetica \
           -pointsize "$font_size_small" \
           -gravity SouthEast \
           -annotate -"$(( min_dim ))x$(( min_dim * 3 / 2 ))+0" "v1.0" \
        \) -composite \
        -quality 95 \
        "$out"

    echo "  -> $out"
}

generate_light_wallpaper() {
    local width="$1"
    local height="$2"
    local label="${width}x${height}"
    local out="$OUTPUT_BASE/yummyos-light-${label}.png"

    echo "Generating light wallpaper: $label ..."

    local min_dim=$(( width < height ? width : height ))
    local font_size_large=$(( min_dim / 8 ))
    local font_size_small=$(( min_dim / 20 ))

    $CONVERT \
        -size "${width}x${height}" \
        gradient:"#e8eef4-${LIGHT_BG}" \
        \( -size "${width}x${height}" xc:none \
           -fill "${ACCENT}15" \
           -draw "circle $(( width * 2 / 5 )),$(( height * 3 / 7 )) $(( width * 2 / 5 + min_dim / 5 )),$(( height * 3 / 7 + min_dim / 5 ))" \
           -fill "${SECONDARY}10" \
           -draw "circle $(( width * 7 / 10 )),$(( height * 4 / 7 )) $(( width * 7 / 10 + min_dim / 4 )),$(( height * 4 / 7 + min_dim / 4 ))" \
           -stroke "${ACCENT}20" -strokewidth 2 \
           -draw "line $(( width / 6 )),$(( height / 5 )) $(( width * 5 / 6 )),$(( height * 4 / 5 ))" \
           -stroke "${SECONDARY}15" -strokewidth 2 \
           -draw "line $(( width * 5 / 6 )),$(( height / 5 )) $(( width / 6 )),$(( height * 4 / 5 ))" \
           -stroke "${ACCENT}15" -strokewidth 1 \
           -draw "polyline $(( width / 3 )),$(( height / 3 )) $(( width / 2 )),$(( height / 5 )) $(( width * 2 / 3 )),$(( height / 3 ))" \
        \) -composite \
        \( -size "${width}x${height}" xc:none \
           -fill "#1a1a2e" -font Helvetica-Bold \
           -pointsize "$font_size_large" \
           -gravity SouthEast \
           -annotate -"$(( min_dim / 30 ))x$(( min_dim / 15 ))+0" "YummyOS" \
        \) -composite \
        -quality 95 \
        "$out"

    echo "  -> $out"
}

generate_minimal_wallpaper() {
    local width="$1"
    local height="$2"
    local label="${width}x${height}"
    local out="$OUTPUT_BASE/yummyos-minimal-${label}.png"

    echo "Generating minimal wallpaper: $label ..."

    local min_dim=$(( width < height ? width : height ))
    local font_size_large=$(( min_dim / 10 ))
    local bar_h=$(( height / 3 ))
    local bar_y=$(( height / 2 - bar_h / 2 ))

    $CONVERT \
        -size "${width}x${height}" \
        "xc:${DARK_START}" \
        \( -size "${width}x${bar_h}" \
           gradient:"${ACCENT}-${SECONDARY}" \
        \) -gravity Center -geometry "+0+0" -composite \
        -blur 0x60 \
        \( -size "${width}x${height}" xc:none \
           -fill "white" -font Helvetica-Bold \
           -pointsize "$font_size_large" \
           -gravity Center \
           -annotate +0+0 "YummyOS" \
        \) -composite \
        -quality 95 \
        "$out"

    echo "  -> $out"
}

# ---------------------------------------------------------------
# Generate all wallpapers
# ---------------------------------------------------------------

echo "--- Dark wallpapers ---"
for res in "1920x1080" "2560x1440" "3840x2160"; do
    w="${res%x*}"
    h="${res#*x}"
    generate_dark_wallpaper "$w" "$h"
done

echo ""
echo "--- Light wallpaper ---"
generate_light_wallpaper 1920 1080

echo ""
echo "--- Minimal wallpaper ---"
generate_minimal_wallpaper 1920 1080

echo ""
echo "=== Done! Generated wallpapers in: $OUTPUT_BASE ==="
ls -lh "$OUTPUT_BASE"/*.png 2>/dev/null || true
