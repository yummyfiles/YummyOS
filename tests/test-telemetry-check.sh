#!/usr/bin/env bash
# YummyOS - Telemetry Check Script Test Suite

set -euo pipefail

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YUMMY_ROOT="$(dirname "$TEST_DIR")"
TELEMETRY_SCRIPT="${YUMMY_ROOT}/config/usr/bin/yummy-telemetry-check"

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

echo "=== YummyOS Telemetry Check Tests ==="
echo ""

# Test 1: Script exists
echo "[Test 1] Script exists and is executable"
if [[ -f "$TELEMETRY_SCRIPT" ]]; then
    assert_true "Telemetry check script file exists" 0
else
    assert_true "Telemetry check script file exists" 1
fi
if [[ -x "$TELEMETRY_SCRIPT" ]]; then
    assert_true "Telemetry check script is executable" 0
else
    assert_true "Telemetry check script is executable" 1
fi
echo ""

# Test 2: Script runs without error
echo "[Test 2] Script runs without error"
output=""
exit_code=0
output=$("$TELEMETRY_SCRIPT" 2>&1) || exit_code=$?
assert_exit_code "Script exits with code 0" 0 "$exit_code"
echo ""

# Test 3: Output contains expected section headers
echo "[Test 3] Output contains expected section headers"
assert_contains "Contains 'Telemetry' or 'telemetry' header" "elemetry" "$output"
assert_contains "Contains 'Privacy' or 'privacy' section" "rivacy" "$output"
assert_contains "Output is not empty" "true" "$( [[ -n "$output" ]] && echo 'true' || echo '' )"
echo ""

# Test 4: Output is human-readable (contains text, not binary garbage)
echo "[Test 4] Output is human-readable"
line_count=$(echo "$output" | wc -l)
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$line_count" -gt 0 ]]; then
    echo "  PASS: Output contains $line_count lines"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Output appears empty or unreadable"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi

# Check no null bytes or control characters
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if echo "$output" | grep -Pq '[\x00-\x08\x0e-\x1f]'; then
    echo "  FAIL: Output contains non-printable characters"
    FAIL_COUNT=$((FAIL_COUNT + 1))
else
    echo "  PASS: Output contains only printable characters"
    PASS_COUNT=$((PASS_COUNT + 1))
fi
echo ""

# Test 5: Script handles --help gracefully
echo "[Test 5] Script handles --help gracefully"
help_output=""
help_exit=0
help_output=$("$TELEMETRY_SCRIPT" --help 2>&1) || help_exit=$?
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$help_exit" -eq 0 ]]; then
    echo "  PASS: --help exits 0"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: --help should exit 0 (got $help_exit)"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
assert_contains "--help output is non-empty" "true" "$( [[ -n "$help_output" ]] && echo 'true' || echo '' )"
echo ""

# Test 6: Script handles invalid flags gracefully
echo "[Test 6] Script handles invalid flags gracefully"
invalid_exit=0
"$TELEMETRY_SCRIPT" --nonexistent-flag-xyz 2>/dev/null || invalid_exit=$?
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$invalid_exit" -ne 0 ]] || echo "$("$TELEMETRY_SCRIPT" --nonexistent-flag-xyz 2>&1)" | grep -qiE '(error|usage|unknown|invalid|help)'; then
    echo "  PASS: Invalid flag produces an error or help message"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Invalid flag should produce error or help message"
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
