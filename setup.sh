#!/bin/bash

if [[ "$(uname)" == "Darwin" ]]; then
    echo "-- MacOsFound"
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
    echo "-- LinuxOsFound"
elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ||  "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]] ; then
    echo "Windows is not supported"
    exit 1
fi

if [[ ! -f "extra/pybind11/LICENSE" ]]; then
  rm extra/pybind11 -r
  echo "pybind11 installing" | git clone https://github.com/pybind/pybind11.git extra/pybind11
fi

if [[ -f "venv/venv.cfg" ]]; then
  sudo pip3 install virtualenv
  virtualenv venv

  sudo apt-get install python3-pip
  sudo pip3 install pygame
  pip3 install pyautogui
  pip3 install opencv-python
  pip3 install mss
fi


