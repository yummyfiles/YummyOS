#!/usr/bin/env bash
# YummyOS - Test Runner
# Runs all test suites and reports aggregate results.

set -uo pipefail

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TOTAL_PASS=0
TOTAL_FAIL=0
TOTAL_TESTS=0
SUITES_RUN=0
SUITES_FAILED=0

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

TEST_FILES=(
    "test-performance.sh"
    "test-telemetry-check.sh"
    "test-backup.sh"
    "test-theme-apply.sh"
)

echo ""
echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${BOLD}${CYAN}    YummyOS Test Suite - run-all-tests${NC}"
echo -e "${BOLD}${CYAN}============================================${NC}"
echo ""

for test_file in "${TEST_FILES[@]}"; do
    test_path="${TEST_DIR}/${test_file}"

    if [[ ! -f "$test_path" ]]; then
        echo -e "${RED}[SKIP] ${test_file} - file not found${NC}"
        continue
    fi

    if [[ ! -x "$test_path" ]]; then
        echo -e "${YELLOW}[SKIP] ${test_file} - not executable (attempting to run anyway)${NC}"
    fi

    SUITES_RUN=$((SUITES_RUN + 1))
    echo -e "${BOLD}--- Running: ${test_file} ---${NC}"
    echo ""

    suite_output=""
    suite_exit=0
    suite_output=$(bash "$test_path" 2>&1) || suite_exit=$?
    echo "$suite_output"

    # Parse pass/fail counts from suite output
    suite_pass=$(echo "$suite_output" | grep -oP 'Results: \K[0-9]+(?= passed)' || echo "0")
    suite_fail=$(echo "$suite_output" | grep -oP 'passed, \K[0-9]+(?= failed)' || echo "0")
    suite_total=$(echo "$suite_output" | grep -oP 'out of \K[0-9]+(?= tests)' || echo "0")

    TOTAL_PASS=$((TOTAL_PASS + ${suite_pass:-0}))
    TOTAL_FAIL=$((TOTAL_FAIL + ${suite_fail:-0}))
    TOTAL_TESTS=$((TOTAL_TESTS + ${suite_total:-0}))

    if [[ "$suite_exit" -ne 0 ]]; then
        SUITES_FAILED=$((SUITES_FAILED + 1))
    fi

    echo ""
done

echo ""
echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${BOLD}${CYAN}           AGGREGATE RESULTS${NC}"
echo -e "${BOLD}${CYAN}============================================${NC}"
echo ""
echo -e "  Suites run:     ${SUITES_RUN}"
echo -e "  Suites failed:  ${SUITES_FAILED}"
echo ""
echo -e "  Total tests:    ${TOTAL_TESTS}"

if [[ "$TOTAL_PASS" -gt 0 ]]; then
    echo -e "  ${GREEN}Total passed:   ${TOTAL_PASS}${NC}"
fi

if [[ "$TOTAL_FAIL" -gt 0 ]]; then
    echo -e "  ${RED}Total failed:   ${TOTAL_FAIL}${NC}"
fi

echo ""

if [[ "$TOTAL_FAIL" -eq 0 ]] && [[ "$SUITES_RUN" -gt 0 ]]; then
    echo -e "${GREEN}${BOLD}ALL TESTS PASSED${NC}"
    exit 0
elif [[ "$SUITES_RUN" -eq 0 ]]; then
    echo -e "${YELLOW}${BOLD}NO TEST SUITES WERE RUN${NC}"
    exit 1
else
    echo -e "${RED}${BOLD}SOME TESTS FAILED${NC}"
    exit 1
fi
