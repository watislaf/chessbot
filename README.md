# ChessBot  <img src="https://user-images.githubusercontent.com/45079123/147250964-53ae261a-dbf4-4c46-9a4d-3f1f3d6d2ab8.png" width="40 px">     [![en](https://img.shields.io/badge/lang-en-yellow.svg)](README.md)


Bot to play chess on [chess.com](https://www.qt.io) for me  (cause
i am to lazy to learn how to bee good at chess)(Linux only).

Current ELO rating is **2014**.
___

## You can try out the Engine . 

<p align=center>
  <img  src="https://github.com/watislaf/chessbot/blob/main/resources/board_show.gif" width="40%"> 
  </p>
  
## Or compete it with other players

<p align=center>
  <img  src="https://github.com/watislaf/chessbot/blob/main/resources/bot_show.gif"  width="70% " >
  </p>

### Features
* 64 bit board representation. 
* The code is divided into three parts: self-written chess engine in C++, board
  visualisation and computer vision on python.
* The code was optimized by blood and sweat to keep your computer from exploding
  during chess game.
* After you build the project , there is an uninstall script, to remove all
  redundant libraries (and the project 🥲).
* You Build it on Windows (as well as a) so that's +1 (one more) reason to
  install linux.

### Authors

* Vladislav Kozulin ([@watislaf](https://github.com/watislaf))

## Development status

My goal waas 2000 ELO on chess.com, and still i'm not banned, so i think this is sukcess.

## Build 

Download source files.

```bat
  git clone https://github.com/watislaf/chessbot.git # install
  cd ./chessbot
```
if you don't have git, just download source from [this page](https://github.com/watislaf/chessbot/releases/tag/V1.0.1600Elo).

Next, there is a small script to build the project.

If you don't need the bot part, but want a taste of a losing game in chess,
press No, so the OpenCv library will not be downloaded.

```bat
  bash ./setup.sh      
```
 Build the project, answer No only if you do not Bot version
 
## Or just Install 

Check [releases](https://github.com/watislaf/chessbot/releases), and download latest ChessBot.zip

## Run

To run a board-game:

```bat
    ./ChessBot  --mode=board 
                --white_player=[human|engine] # default is human
                --black_player=[human|engine] # default is engine
                --lvl=[easy|bullet|blitz|rapid]  # 0,1,3,10 min games. default is bullet
```

Or to run the second mode (only if you pressed Yes in setup.sh script). Check settings.png file with your chess.com settings.

```bat
    ./ChessBot --mode=chesscom                 
                --level=[bullet|blitz|rapid]  
```

## Reinstall and uninstall

If you pressed No, but changed your mind, type

```bat
  bash ./setup.sh reinstall 
```
Here you can answer Yes again.

## Uninstall
It is also possible to uninstall the script (and all downloaded with this
project libraries).   

```bat
  bash ./unis.sh  
```
