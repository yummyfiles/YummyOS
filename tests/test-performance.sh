#!/usr/bin/env bash
# YummyOS - Performance Profile Script Test Suite

set -euo pipefail

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YUMMY_ROOT="$(dirname "$TEST_DIR")"
PERF_SCRIPT="${YUMMY_ROOT}/config/usr/bin/yummy-performance"

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

assert_equals() {
    local desc="$1"
    local expected="$2"
    local actual="$3"
    TOTAL_COUNT=$((TOTAL_COUNT + 1))
    if [[ "$expected" == "$actual" ]]; then
        echo "  PASS: $desc"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "  FAIL: $desc (expected: '$expected', got: '$actual')"
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

run_perf() {
    "$PERF_SCRIPT" "$@" 2>&1
}

get_exit_code() {
    "$@" 2>/dev/null; echo $?
}

# --- Test Suite ---

echo "=== YummyOS Performance Profile Tests ==="
echo ""

# Test 1: Script exists and is executable
echo "[Test 1] Script exists and is executable"
if [[ -f "$PERF_SCRIPT" ]]; then
    assert_true "Performance script file exists" 0
else
    assert_true "Performance script file exists" 1
fi
if [[ -x "$PERF_SCRIPT" ]]; then
    assert_true "Performance script is executable" 0
else
    assert_true "Performance script is executable" 1
fi
echo ""

# Test 2: yummy-performance list outputs all 4 profiles
echo "[Test 2] 'yummy-performance list' outputs all 4 profiles"
output=$(run_perf list)
assert_contains "Contains 'powersave'" "powersave" "$output"
assert_contains "Contains 'balanced'" "balanced" "$output"
assert_contains "Contains 'performance'" "performance" "$output"
assert_contains "Contains 'custom'" "custom" "$output"

profile_count=$(echo "$output" | grep -cE '^\s*(powersave|balanced|performance|custom)' || true)
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$profile_count" -ge 4 ]]; then
    echo "  PASS: Lists at least 4 profiles"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Expected at least 4 profiles, found $profile_count"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 3: yummy-performance get returns current profile
echo "[Test 3] 'yummy-performance get' returns current profile"
output=$(run_perf get)
exit_code=$(get_exit_code run_perf get)
assert_exit_code "'yummy-performance get' exits 0" 0 "$exit_code"
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ -n "$output" ]] && echo "$output" | grep -qiE '(powersave|balanced|performance|custom)'; then
    echo "  PASS: 'get' returns a valid profile name"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: 'get' did not return a recognized profile name"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 4: Invalid profile name shows error
echo "[Test 4] Invalid profile name shows error"
exit_code=$(get_exit_code run_perf set turbo-mega-super-mode)
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$exit_code" -ne 0 ]]; then
    echo "  PASS: Invalid profile returns non-zero exit code"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Invalid profile should return non-zero exit code"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi

output=$(run_perf set turbo-mega-super-mode 2>&1 || true)
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if echo "$output" | grep -qiE '(error|invalid|unknown|not found)'; then
    echo "  PASS: Invalid profile shows error message"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Invalid profile should show error message"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 5: yummy-performance --help shows usage
echo "[Test 5] 'yummy-performance --help' shows usage"
output=$(run_perf --help)
exit_code=$(get_exit_code run_perf --help)
assert_exit_code "'--help' exits 0" 0 "$exit_code"
assert_contains "Shows usage/help text" "usage" "$(echo "$output" | tr '[:upper:]' '[:lower:]')"
assert_contains "Mentions 'list' subcommand" "list" "$output"
assert_contains "Mentions 'get' subcommand" "get" "$output"
assert_contains "Mentions 'set' subcommand" "set" "$output"
echo ""

# Test 6: yummy-performance --version shows version
echo "[Test 6] 'yummy-performance --version' shows version"
output=$(run_perf --version)
exit_code=$(get_exit_code run_perf --version)
assert_exit_code "'--version' exits 0" 0 "$exit_code"
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if echo "$output" | grep -qE '[0-9]+\.[0-9]+(\.[0-9]+)?'; then
    echo "  PASS: --version output contains version number"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: --version output does not contain a version number"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 7: Subcommands with no arguments run without crashing
echo "[Test 7] Subcommands run without crashing"
exit_code_list=$(get_exit_code run_perf list)
assert_exit_code "'list' exits 0" 0 "$exit_code_list"

exit_code_get=$(get_exit_code run_perf get)
assert_exit_code "'get' exits 0" 0 "$exit_code_get"
echo ""

# --- Summary ---

echo "==========================================="
echo " Results: $PASS_COUNT passed, $FAIL_COUNT failed out of $TOTAL_COUNT tests"
echo "==========================================="

if [[ "$FAIL_COUNT" -gt 0 ]]; then
    exit 1
fi
exit 0
