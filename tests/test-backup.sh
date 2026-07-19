#!/usr/bin/env bash
# YummyOS - Backup Utility Test Suite

set -euo pipefail

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YUMMY_ROOT="$(dirname "$TEST_DIR")"
BACKUP_SCRIPT="${YUMMY_ROOT}/config/usr/bin/yummy-backup"

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

cleanup_temp_dir() {
    local dir="$1"
    if [[ -d "$dir" ]]; then
        rm -rf "$dir"
    fi
}

# --- Test Suite ---

echo "=== YummyOS Backup Utility Tests ==="
echo ""

# Test 1: Script exists and is executable
echo "[Test 1] Script exists and is executable"
if [[ -f "$BACKUP_SCRIPT" ]]; then
    assert_true "Backup script file exists" 0
else
    assert_true "Backup script file exists" 1
fi
if [[ -x "$BACKUP_SCRIPT" ]]; then
    assert_true "Backup script is executable" 0
else
    assert_true "Backup script is executable" 1
fi
echo ""

# Test 2: --help shows usage
echo "[Test 2] '--help' shows usage"
help_output=""
help_exit=0
help_output=$("$BACKUP_SCRIPT" --help 2>&1) || help_exit=$?
assert_exit_code "'--help' exits 0" 0 "$help_exit"
assert_contains "Shows 'usage' or 'Usage'" "usage" "$(echo "$help_output" | tr '[:upper:]' '[:lower:]')"
assert_contains "Mentions 'list' subcommand" "list" "$help_output"
assert_contains "Mentions 'create' subcommand" "create" "$help_output"
assert_contains "Mentions 'restore' subcommand" "restore" "$help_output"
echo ""

# Test 3: --version shows version
echo "[Test 3] '--version' shows version"
version_output=""
version_exit=0
version_output=$("$BACKUP_SCRIPT" --version 2>&1) || version_exit=$?
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

# Test 4: list on empty directory works
echo "[Test 4] 'yummy-backup list' on empty directory"
TEMP_BACKUP_DIR=$(mktemp -d)
trap "cleanup_temp_dir '$TEMP_BACKUP_DIR'" EXIT

export YUMMY_BACKUP_DIR="$TEMP_BACKUP_DIR"
list_output=""
list_exit=0
list_output=$("$BACKUP_SCRIPT" list 2>&1) || list_exit=$?
assert_exit_code "'list' exits 0 on empty directory" 0 "$list_exit"

TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ -z "$list_output" ]] || echo "$list_output" | grep -qiE '(no backups|empty|none|0 backups)'; then
    echo "  PASS: Empty directory shows appropriate message"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Empty directory message not found"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 5: Create a backup
echo "[Test 5] Create a backup"
SOURCE_DIR=$(mktemp -d)
echo "test file content" > "$SOURCE_DIR/test-file.txt"
echo "another file" > "$SOURCE_DIR/another-file.conf"

create_exit=0
"$BACKUP_SCRIPT" create "$SOURCE_DIR" 2>&1 || create_exit=$?
assert_exit_code "'create' exits 0" 0 "$create_exit"

backup_count=$(find "$TEMP_BACKUP_DIR" -maxdepth 1 -mindepth 1 -type d 2>/dev/null | wc -l)
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$backup_count" -ge 1 ]]; then
    echo "  PASS: Backup directory was created ($backup_count backups)"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: No backup directories found after create"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi

# Check that list now shows something
list_output=$("$BACKUP_SCRIPT" list 2>&1) || true
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ -n "$list_output" ]]; then
    echo "  PASS: 'list' shows backups after creating one"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: 'list' is empty after creating a backup"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 6: Restore a backup
echo "[Test 6] Restore a backup"
RESTORE_TARGET=$(mktemp -d)

restore_exit=0
"$BACKUP_SCRIPT" restore "$RESTORE_TARGET" 2>&1 || restore_exit=$?
assert_exit_code "'restore' exits 0" 0 "$restore_exit"

# Verify restored files exist
if [[ -f "$RESTORE_TARGET/test-file.txt" ]]; then
    restored_content=$(cat "$RESTORE_TARGET/test-file.txt")
    assert_equals "Restored file has correct content" "test file content" "$restored_content"
else
    TOTAL_COUNT=$((TOTAL_COUNT + 1))
    echo "  FAIL: Restored file 'test-file.txt' not found"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi

if [[ -f "$RESTORE_TARGET/another-file.conf" ]]; then
    assert_true "Restored file 'another-file.conf' exists" 0
else
    TOTAL_COUNT=$((TOTAL_COUNT + 1))
    echo "  FAIL: Restored file 'another-file.conf' not found"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Test 7: Invalid subcommand shows error
echo "[Test 7] Invalid subcommand shows error"
invalid_exit=0
"$BACKUP_SCRIPT" fly-to-the-moon 2>&1 || invalid_exit=$?
TOTAL_COUNT=$((TOTAL_COUNT + 1))
if [[ "$invalid_exit" -ne 0 ]]; then
    echo "  PASS: Invalid subcommand returns non-zero exit code"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo "  FAIL: Invalid subcommand should return non-zero exit code"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# Cleanup
cleanup_temp_dir "$SOURCE_DIR"
cleanup_temp_dir "$RESTORE_TARGET"

# --- Summary ---

echo "==========================================="
echo " Results: $PASS_COUNT passed, $FAIL_COUNT failed out of $TOTAL_COUNT tests"
echo "==========================================="

if [[ "$FAIL_COUNT" -gt 0 ]]; then
    exit 1
fi
exit 0
