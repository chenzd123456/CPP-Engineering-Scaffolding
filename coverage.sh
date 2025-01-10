#!/bin/bash

set -e

# Clean up previous coverage data
rm -rf build
rm -rf result
mkdir -p build
mkdir -p result

# Configure with coverage flags
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage" ..

# Build project
make -j$(nproc)

# Run tests
ctest --output-on-failure

# Generate coverage data
lcov --capture --directory . --output-file coverage.info

# Filter system headers
lcov --remove coverage.info '/usr/*' '*/test/*' -o filtered_coverage.info

# Generate HTML report
genhtml filtered_coverage.info --output-directory ../result/coverage_report

# Move coverage info to result directory
mv filtered_coverage.info ../result/coverage.info

echo "Coverage report generated at result/coverage_report/index.html"
