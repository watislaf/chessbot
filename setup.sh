#!/bin/bash

CHESS_ENGINE_BUILD_DIR_NAME="BuildChessEngine"
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

# CHECKS LINUX SUPPORT
if [[ "$(uname)" == "Darwin" ]]; then
    echo "-- MacOs in not supported"
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
    echo "-- LinuxOsFound"
elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ||  "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]] ; then
    echo "-- Windows is not supported"
    exit 1
fi

if  lscpu|grep "CPU op-mode" | grep 64-bit 1>/dev/null ; then
  echo "-- Found an x64 arch. "
else
  echo "-- This engine works only on x64 bit arch"
  exit 0
fi

if [[ ! -f "$SCRIPTPATH/unis.sh" ]]; then
  echo "#!/bin/bash" > "$SCRIPTPATH/unis.sh"
  echo "# remove all installed with project libraries" >> "$SCRIPTPATH/unis.sh"
  chmod +x "$SCRIPTPATH/unis.sh"
  echo " rm -r -f \$(ls -a)" >> "$SCRIPTPATH/unis.sh"
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
  rm  $SCRIPTPATH/venv -r
  rm  $SCRIPTPATH/"$CHESS_ENGINE_BUILD_DIR_NAME" -r
fi

# uninstall argument
if [[ "$1" == "uninstall" ]]; then
  $SCRIPTPATH/unis.sh
fi


# checks for python dep
if [[ ! -f "$SCRIPTPATH/venv/pyvenv.cfg" ]]; then
  if ! command -v pip3 &> /dev/null ; then
      sudo apt-get install python3-pip
      echo "sudo apt-get remove --auto-remove python3-pip" >> "$SCRIPTPATH/unis.sh"
  fi

  sudo pip3 install virtualenv
  virtualenv venv
  source $SCRIPTPATH/venv/bin/activate

  pip3 install pygame
  pip3 install pyinstaller
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
  echo "add pybind to extra $SCRIPTPATH/dir" | git clone $PYBIND_LINK extra/pybind11
fi


# install magics numbers algo from github
if [[ ! -f "extra/magics/magics.c" ]]; then 
    MAGIC_LINK="https://raw.githubusercontent.com/maksimKorzh/chess_programming/master/src/magics/magics.c"
    echo "-- add magic generator to $SCRIPTPATH/extra" | wget "$MAGIC_LINK" -P extra/magics
fi
 
# checks x64 version
cmake  -DCMAKE_BUILD_TYPE=Release   cmake -B$SCRIPTPATH/"$CHESS_ENGINE_BUILD_DIR_NAME" -S$SCRIPTPATH/

# checks if cmake is installed
if command -v cmake 1> /dev/null ; then
  echo "-- Found Cmake."
else
  echo "-- Cant find cmake instruction, start to download."
  sudo apt install cmake
  echo "sudo apt-get remove --auto-remove cmake" >> "$SCRIPTPATH/unis.sh"
fi


cd $SCRIPTPATH/"$CHESS_ENGINE_BUILD_DIR_NAME"
make
cd ..


export PYGAME_HIDE_SUPPORT_PROMPT=1
#export PYTHONPATH="$PYTHONPATH:$SCRIPTPATH"


source $SCRIPTPATH/venv/bin/activate

pyinstaller -p "$SCRIPTPATH/engine/output"  --add-data "$SCRIPTPATH/resources:."  $SCRIPTPATH/bin/main.py
mkdir "$SCRIPTPATH/dist/main/resources/"
for f in $( find "$SCRIPTPATH/dist/" -type f -name '*.png' ); do
  mv  "$f" -t "$SCRIPTPATH/dist/main/resources/"
done

rm "$SCRIPTPATH/dist/main/board_show.gif"
rm "$SCRIPTPATH/dist/main/bot_show.gif"
echo "bash $SCRIPTPATH/dist/main/main \$@" > "$SCRIPTPATH/dist/ChessBot"
chmod +x $SCRIPTPATH/dist/ChessBot