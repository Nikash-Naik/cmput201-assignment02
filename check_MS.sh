#!/bin/bash
# Milestone check script for CMPUT 201 Assignment 2.
# The milestone program reads its test case on STDIN rather than taking a
# file argument, so each case is piped in.

compilation_flags="-g -Wall -Werror -std=c99"
source_path="./src/assg.c"
testcases_dir="./Testcases/Milestone Test Cases"
exe_path="./assg_ms"
time_limit=10

# macOS has no GNU `timeout`; fall back to gtimeout, then to no limit.
if command -v timeout >/dev/null 2>&1; then TIMEOUT="timeout $time_limit"
elif command -v gtimeout >/dev/null 2>&1; then TIMEOUT="gtimeout $time_limit"
else TIMEOUT=""; fi

log()     { printf "[CHECK-MS] %s\n" "$1"; }
error()   { printf "[CHECK-MS] Error: %s\n" "$1"; exit 1; }

log "START MILESTONE CHECK"

[ -d "$testcases_dir" ] || error "Missing test case directory '$testcases_dir'."
[ -e "$source_path" ]   || error "Required file '$source_path' not found."

if gcc ${compilation_flags} "$source_path" -o "$exe_path"; then
    log "Compiled with no warnings."
else
    error "Compilation failed."
fi

fail=0
for input_path in "$testcases_dir"/*-input.txt; do
    base=${input_path%-input.txt}
    output_path="$base-output.txt"
    student_path="$base-student.txt"
    [ -e "$output_path" ] || { log "No expected output for '$input_path'. Skipping."; continue; }

    log "Testing $(basename "$base")..."
    $TIMEOUT "$exe_path" < "$input_path" > "$student_path" 2>&1
    code=$?
    if [ $code -eq 124 ]; then
        log "Program timed out."; fail=1; continue
    elif [ $code -ne 0 ]; then
        log "Program exited with non-zero code $code."; fail=1; continue
    fi

    if diff -u "$output_path" "$student_path"; then
        log "$(basename "$base"): output matches."
    else
        log "$(basename "$base"): OUTPUT MISMATCH (diff above)."; fail=1
    fi
done

rm -f "$exe_path"
if [ $fail -eq 0 ]; then
    log "END: All milestone test cases passed!"
else
    error "Some milestone test cases failed."
fi
