#!/bin/bash

python3 misc/script/cmakelists.py -r -t '${COMMON_OBJLIB}' common
python3 misc/script/cmakelists.py -r -t '${COMMON_OBJLIB}' include
python3 misc/script/cmakelists.py -r -t '${OBJLIB}' dft
python3 misc/script/cmakelists.py -r -t '${UTIL_OBJLIB}' util
python3 misc/script/cmakelists.py -r -t '${TEST}' test
python3 misc/script/cmakelists.py -r -t '${COMPARE}' misc/compare
python3 misc/script/cmakelists.py -r -t '${BENCH}' misc/bench
