#!/bin/bash

CHESS_ENGINE_BUILD_DIR_NAME="BuildChessEngine"
# CHECKS LINUX SUPPORT
if [[ "$(uname)" == "Darwin" ]]; then
    echo "-- MacOs in not supported"
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
    echo "-- LinuxOsFound"
elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ||  "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]] ; then
    echo "-- Windows is not supported"
    exit 1
fi

if [[ ! -f "./unis.sh" ]]; then
  echo "#!/bin/bash" > "./unis.sh"
  echo "# remove all installed with project libraries" >> "./unis.sh"
  chmod +x "./unis.sh"
fi

# --help argument
if [[ "$1" == "help" ]]; then
  echo $'/t' "help" $'/t' "information"
  echo $'/t' "reinstall" $'/t' "set up project again"
  echo $'/t' "uninstall" $'/t' "remove all installed with project libraries"
  exit 0
fi

# reinstall argument
if [[ "$1" == "reinstall" ]]; then
  rm  ./venv -r
  rm  ./"$CHESS_ENGINE_BUILD_DIR_NAME" -r
fi

# uninstall argument
if [[ "$1" == "uninstall" ]]; then
  ./unis.sh
fi


# checks for python dep
if [[ ! -f "./venv/pyvenv.cfg" ]]; then
  if ! command -v pip3 &> /dev/null ; then
      sudo apt-get install python3-pip
      echo "sudo apt-get remove --auto-remove python3-pip" >> "./unis.sh"
  fi

  sudo pip3 install virtualenv
  virtualenv venv
  source ./venv/bin/activate

  pip3 install pygame
  echo "-- Do you want to setup chess.com Bot? (Y/N) "
  read -p "---- if not, then only Playable chess board could be used." yn
  case $yn in
      [Yy]* )   pip3 install pyautogui
                pip3 install opencv-python
                pip3 install mss;;

      [Nn]* ) ;;
          * ) echo "-- Please answer yes or no.";;
  esac
else
  echo "-- Python is already configured. "
fi

# download pybind11
if [[ ! -f "extra/pybind11/LICENSE" ]]; then
  PYBIND_LINK="https://github.com/pybind/pybind11.git"
  rm extra/pybind11 -r
  echo "add pybind to extra ./dir" | git clone $PYBIND_LINK extra/pybind11
fi


# install magics numbers algo from github
if [[ ! -f "extra/magics/magics.c" ]]; then
    MAGIC_LINK="https://raw.githubusercontent.com/maksimKorzh/chess_programming/master/src/magics/magics.c" 
    echo "-- add magic generator to ./extra" | wget "$MAGIC_LINK" -P extra/magics
fi
  
# checks x64 version
if  lscpu|grep "CPU op-mode" | grep 64-bit 1>/dev/null ; then
  echo "-- Found an x64 arch. "
  cmake  -DCMAKE_BUILD_TYPE=Release -D ARCH=64 cmake -B./"$CHESS_ENGINE_BUILD_DIR_NAME" -S./

else
  echo "-- Cant find an x64 arch, your chess algorithm will be slow down. "
  cmake  -DCMAKE_BUILD_TYPE=Release -D ARCH=32 cmake -B./"$CHESS_ENGINE_BUILD_DIR_NAME" -S./
fi

# checks if cmake is installed
if command -v cmake 1> /dev/null ; then
  echo "-- Found Cmake."
else
  echo "-- Cant find cmake instruction, start to download."
  sudo apt install cmake
  echo "sudo apt-get remove --auto-remove cmake" >> "./unis.sh"
fi


cd ./"$CHESS_ENGINE_BUILD_DIR_NAME"
make
cd ..


echo "#!/bin/bash" > ChessBot
echo "export PYGAME_HIDE_SUPPORT_PROMPT=1" >> ChessBot
echo "export PYTHONPATH=\"\$PYTHONPATH:bin/bash\"" >> ChessBot
echo "./venv/bin/python3 ./bin/main.py \$@" >> ChessBot
chmod +x ChessBot

echo " rm -r -f $(ls)" >> "./unis.sh"
