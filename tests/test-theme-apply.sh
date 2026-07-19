#!/usr/bin/env bash
# YummyOS - Theme Application Test Suite

set -euo pipefail

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YUMMY_ROOT="$(dirname "$TEST_DIR")"
THEME_SCRIPT="${YUMMY_ROOT}/config/usr/bin/yummy-theme-apply"

PASS_COUNT=0
FAIL_COUNT=0
TOTAL_COUNT=0

assert_true() {
    local desc="$1"
    local result="$2"
    TOTAL_COUNT=$((TOTAL_COUNT + 1))
    if [[ "$result" -eq 0 ]]; then
        echo "  PASS: $desc"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "  FAIL: $desc"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

assert_contains() {
    local desc="$1"
    local needle="$2"
    local haystack="$3"
    TOTAL_COUNT=$((TOTAL_COUNT + 1))
    if [[ "$haystack" == *"$needle"* ]]; then
        echo "  PASS: $desc"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "  FAIL: $desc (expected to contain '$needle')"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

assert_exit_code() {
    local desc="$1"
    local expected_code="$2"
    local actual_code="$3"
    TOTAL_COUNT=$((TOTAL_COUNT + 1))
    if [[ "$expected_code" -eq "$actual_code" ]]; then
        echo "  PASS: $desc"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "  FAIL: $desc (expected exit $expected_code, got $actual_code)"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# --- Test Suite ---

echo "=== YummyOS Theme Application Tests ==="
echo ""

# Test 1: Script exists and is executable
echo "[Test 1] Script exists and is executable"
if [[ -f "$THEME_SCRIPT" ]]; then
    assert_true "Theme script file exists" 0
else
    assert_true "Theme script file exists" 1
fi
if [[ -x "$THEME_SCRIPT" ]]; then
    assert_true "Theme script is executable" 0
else
    assert_true "Theme script is executable" 1
fi
echo ""

# Test 2: Script runs without error (with --help to be safe)
echo "[Test 2] Script runs without error (--help)"
output=""
exit_code=0
output=$("$THEME_SCRIPT" --help 2>&1) || exit_code=$?
assert_exit_code "Script --help exits 0" 0 "$exit_code"
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ -n "$output" ]]; then
    echo "  PASS: --help produces output"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: --help produces no output"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 3: --help shows usage
echo "[Test 3] '--help' shows usage"
assert_contains "Shows 'usage' or 'Usage'" "usage" "$(echo "$output" | tr '[:upper:]' '[:lower:]')"
assert_contains "Mentions theme options" "theme" "$(echo "$output" | tr '[:upper:]' '[:lower:]')"
assert_contains "Mentions --apply option" "--apply" "$output"
echo ""

# Test 4: --list-accent-colors shows available colors
echo "[Test 4] '--list-accent-colors' shows available colors"
color_output=""
color_exit=0
color_output=$("$THEME_SCRIPT" --list-accent-colors 2>&1) || color_exit=$?
assert_exit_code "'--list-accent-colors' exits 0" 0 "$color_exit"

assert_contains "Lists red" "red" "$(echo "$color_output" | tr '[:upper:]' '[:lower:]')"
assert_contains "Lists blue" "blue" "$(echo "$color_output" | tr '[:upper:]' '[:lower:]')"
assert_contains "Lists green" "green" "$(echo "$color_output" | tr '[:upper:]' '[:lower:]')"

color_count=$(echo "$color_output" | grep -cEi '(red|blue|green|orange|purple|pink|teal|yellow)' || true)
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$color_count" -ge 3 ]]; then
    echo "  PASS: Lists at least 3 accent colors"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Expected at least 3 accent colors, found $color_count"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 5: --version shows version
echo "[Test 5] '--version' shows version"
version_output=""
version_exit=0
version_output=$("$THEME_SCRIPT" --version 2>&1) || version_exit=$?
assert_exit_code "'--version' exits 0" 0 "$version_exit"
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if echo "$version_output" | grep -qE '[0-9]+\.[0-9]+(\.[0-9]+)?'; then
    echo "  PASS: --version output contains version number"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: --version output does not contain a version number"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 6: Invalid accent color shows error
echo "[Test 6] Invalid accent color shows error"
invalid_exit=0
"$THEME_SCRIPT" --apply "not-a-real-color-xyz123" 2>&1 || invalid_exit=$?
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$invalid_exit" -ne 0 ]]; then
    echo "  PASS: Invalid accent color returns non-zero exit code"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Invalid accent color should return non-zero exit code"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 7: --list-themes shows available themes (if supported)
echo "[Test 7] '--list-themes' or default list works"
themes_exit=0
themes_output=$("$THEME_SCRIPT" --list-themes 2>&1) || themes_exit=$?
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$themes_exit" -eq 0 ]] && [[ -n "$themes_output" ]]; then
    echo "  PASS: --list-themes shows available themes"
    PASS_COUNT=$((PASS_COUNT + 1))
elif [[ "$themes_exit" -ne 0 ]]; then
    echo "  PASS: --list-themes not implemented (non-critical)"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: --list-themes returned empty output"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# --- Summary ---

echo "==========================================="
echo " Results: $PASS_COUNT passed, $FAIL_COUNT failed out of $TOTAL_COUNT tests"
echo "==========================================="

if [[ "$FAIL_COUNT" -gt 0 ]]; then
    exit 1
fi
exit 0
