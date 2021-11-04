#!/bin/bash

if [[ "$(uname)" == "Darwin" ]]; then
    echo "-- MacOsFound"
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
    echo "-- LinuxOsFound"
elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" or  "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]] ; then
    echo "Windows is not supported"
    exit 1
fi

echo "pybind11 installing" | git clone https://github.com/pybind/pybind11.git extra
