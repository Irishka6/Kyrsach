#!/bin/bash
echo "Compiling GoogleTest..."

# Находим все .cc файлы кроме gtest-all.cc
for file in src/*.cc; do
    if [[ "$file" != "src/gtest-all.cc" ]]; then
        filename=$(basename "$file" .cc)
        echo "Compiling $filename..."
        g++ -std=c++11 -I./include -I./ -c "$file" -o "${filename}.o"
    fi
done

echo "Creating libraries..."
ar rvs libgtest.a gtest.o gtest-death-test.o gtest-filepath.o gtest-port.o gtest-printers.o gtest-test-part.o gtest-typed-test.o
ar rvs libgtest_main.a gtest.o gtest-death-test.o gtest-filepath.o gtest-port.o gtest-printers.o gtest-test-part.o gtest-typed-test.o gtest_main.o

echo "Done!"
