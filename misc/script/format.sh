#!/bin/bash

CLANG_STYLE=--style=file:misc/script/clang-format.yaml

find include -name "*.h" | xargs clang-format -i ${CLANG_STYLE}
find common -name "*.h" -o -name "*.c" | xargs clang-format -i ${CLANG_STYLE}
find dft -name "*.h" -o -name "*.c" | xargs clang-format -i ${CLANG_STYLE}
find test -name "*.h" -o -name "*.c" | xargs clang-format -i ${CLANG_STYLE}
find util -name "*.h" -o -name "*.c" | xargs clang-format -i ${CLANG_STYLE}
find misc -name "*.h" -o -name "*.c" | xargs clang-format -i ${CLANG_STYLE}
