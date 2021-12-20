#!/bin/bash
# TODO CHECK IF OPEN CV REQUIERD ,  CHECK x86 arm

# CHECKS LINUX SUPPORT
if [[ "$(uname)" == "Darwin" ]]; then
    echo "-- MacOsFound, but application is not tasted for MAC"
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
    echo "-- LinuxOsFound"
elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ||  "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]] ; then
    echo "Windows is not supported"
    exit 1
fi

if [[ ! -f "extra/pybind11/LICENSE" ]]; then
  PYBIND_LINK="https://github.com/pybind/pybind11.git"
  rm extra/pybind11 -r
  echo "add pybind to extra ./dir" | git clone PYBIND_LINK extra/pybind11
fi
if [[ ! -f "extra/magics/magics.c" ]]; then
  MAGIC_LINK="https://raw.githubusercontent.com/maksimKorzh/chess_programming/master/src/magics/magics.c"
  rm extra/magics -r
  echo "add magic generator to ./extra" | wget "$MAGIC_LINK" -P extra/magics
fi

if [[ ! -f "venv/venv.cfg" ]]; then
  sudo pip3 install virtualenv
  virtualenv venv

  sudo apt-get install python3-pip
  sudo pip3 install pygame
  pip3 install pyautogui
  pip3 install opencv-python
  pip3 install mss
fi

