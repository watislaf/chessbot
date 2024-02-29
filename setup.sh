#!/bin/bash

# checks if cmake is installed
if command -v cmake 1> /dev/null ; then
  echo "-- Found Cmake."
else
  echo "-- Cant find cmake, download cmake first"
  exit 0
fi


CHESS_ENGINE_BUILD_DIR_NAME="BuildChessEngine"
SCRIPTPATH=$PWD

pip3 install virtualenv
virtualenv venv
source $SCRIPTPATH/venv/bin/activate

pip3 install pygame
pip3 install PySide2
pip3 install pyinstaller
pip3 install pyautogui
pip3 install opencv-python
pip3 install mss

PYBIND_LINK="https://github.com/pybind/pybind11.git"
rm extra/pybind11 -r
echo "add pybind to extra $SCRIPTPATH/extra" | git clone $PYBIND_LINK extra/pybind11

MAGIC_LINK="https://raw.githubusercontent.com/maksimKorzh/chess_programming/master/src/magics/magics.c"
echo "-- add magic generator to $SCRIPTPATH/extra" | wget "$MAGIC_LINK" -P extra/magics

# checks x64 version
cmake  -DCMAKE_BUILD_TYPE=Release   cmake -B$SCRIPTPATH/"$CHESS_ENGINE_BUILD_DIR_NAME" -S$SCRIPTPATH/



cd $SCRIPTPATH/"$CHESS_ENGINE_BUILD_DIR_NAME"
make
cd ..

source $SCRIPTPATH/venv/bin/activate

#pyinstaller  --onefile --noconfirm   --add-data "$SCRIPTPATH/resources/:." -p  "$SCRIPTPATH/engine/output"   $SCRIPTPATH/bin/main.py

if [[ -f $SCRIPTPATH/bin/play_session_capture/chess_board.py ]]; then
       pyinstaller  --noconfirm \
       --add-data  "$SCRIPTPATH/venv/lib/python3.8/site-packages/opencv_python.libs/:." \
       --add-data  "$SCRIPTPATH/venv/lib/python3.8/site-packages/cv2/qt/plugins/platforms/:." \
       --add-data "$SCRIPTPATH/resources/*.png:." \
        -p  "$SCRIPTPATH/engine/output"   $SCRIPTPATH/bin/main.py

        mkdir "$SCRIPTPATH/dist/main/cv2/qt/"
        mkdir "$SCRIPTPATH/dist/main/cv2/qt/fonts"
        for f in $( find "$SCRIPTPATH/venv/lib/python3.8/site-packages/cv2/qt/fonts" -type f -name '*.ttf' ); do
          mv  "$f" -t "$SCRIPTPATH/dist/main/cv2/qt/fonts"
        done
        mv "$SCRIPTPATH/dist/main/main" "$SCRIPTPATH/dist/main/ChessBot"
        cd "$SCRIPTPATH/dist/main/"
        zip -r ChessBot_x64Linux.zip ./
        mv ./ChessBot_x64Linux.zip ./../../

else
         pyinstaller  --onefile --noconfirm \
         --add-data "$SCRIPTPATH/resources/*.png:." \
         -p  "$SCRIPTPATH/engine/output"   $SCRIPTPATH/bin/main.py
         cd "$SCRIPTPATH/dist/"
         zip -r Engine_x64Linux.zip ./
         mv ./ChessBot_x64Linux.zip ./../

fi;
