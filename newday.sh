#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: ./newday.sh <year> <day>"
    exit 1
fi

YEAR=$1
DAY=$(printf "%d" $2)  # Convert to integer to strip leading zeros
DAY_PADDED=$(printf "%02d" $DAY)  # Pad to 2 digits

# Validate year must be current year
CURRENT_YEAR=$(date +%Y)
if [ "$YEAR" -ne "$CURRENT_YEAR" ]; then
    echo "Error: ${YEAR} is not the current year"
    exit 1
fi

# Validate day (1-25)
if [ "$DAY" -lt 1 ] || [ "$DAY" -gt 25 ]; then
    echo "Error: Day must be between 1 and 25"
    exit 1
fi

YEAR_DIR="${YEAR}"
DAY_DIR="${YEAR_DIR}/day${DAY_PADDED}"

# Create year directory if it doesn't exist
if [ ! -d "$YEAR_DIR" ]; then
    mkdir -p "$YEAR_DIR"
fi

# Create day directory if it doesn't exist
if [ ! -d "$DAY_DIR" ]; then
    mkdir -p "$DAY_DIR"
fi

# Create solution.cpp from template
SOLUTION_FILE="${DAY_DIR}/solution.cpp"
if [ ! -f "$SOLUTION_FILE" ]; then
    sed -e "s/{{YEAR}}/${YEAR}/g" -e "s/{{DAY}}/${DAY}/g" \
        templates/solution.cpp.template > "$SOLUTION_FILE"
fi

# Create problem.md from template
PROBLEM_FILE="${DAY_DIR}/problem.md"
if [ ! -f "$PROBLEM_FILE" ]; then
    sed -e "s/{{YEAR}}/${YEAR}/g" -e "s/{{DAY}}/${DAY}/g" \
        templates/problem.md.template > "$PROBLEM_FILE"
fi

# Create empty input.txt
INPUT_FILE="${DAY_DIR}/input.txt"
if [ ! -f "$INPUT_FILE" ]; then
    touch "$INPUT_FILE"
fi
