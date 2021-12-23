# ChessBot

[![en](https://img.shields.io/badge/lang-en-yellow.svg)](README.md)

Bot on **Linux** to play chess on [chess.com](https://www.qt.io) for me  (cause
i am to lazy to learn how to bee good at chess).

Current ELO rating is **1456**.
___

#### You can try out the Engine or compete it with other players.

<p align="center">
<img src="https://user-images.githubusercontent.com/45079123/147157396-f7b8a9df-eb38-4eb6-9d52-bf376e4eda51.png" width="500 px">
<img src="https://user-images.githubusercontent.com/45079123/147156687-36e98daf-9ffc-49ea-bd2c-0f997b0d3b96.png" width="500 px" >
</p>

### Features

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

The development continues. My goal is 2000 ELO on chess.com, and I won't stop
until I reach it (or get banned).

## Install and run the project

Download source files.

```bash
  $ git-clone https://github.com/watislaf/chessbot.git # install
  $ cd ./chessbot
```

Next, there is a small script to build the project.

If you don't need the bot part, but want a taste of a losing game in chess,
press No, so the OpenCv library will not be downloaded.

```bash
  $ ./setup.sh       # run to buil the project and answer No only if you do not need 
```

At the end, to run a board-game:

```bash
  $ ./ChessBot  --mode=board 
                --white_player=[human|engine] # default is human
                --black_player=[human|engine]# default is engine
                --level=[bullet|blitz|rapid]  # 1,3,10 min games. default is bullet
```

Or to run the second mode (only if you pressed Yes in setup.sh script). Check settings.png file with your chess.com settings.

```bash
  $ ./ChessBot --mode=chesscom                 
                --level=[bullet|blitz|rapid]  
```

## Reinstall and uninstall

If you pressed No, but changed your mind, type

```bash
  $ ./setup.sh reinstall # and answer you can answer Yes
```

It is also possible to uninstall the script (and all downloaded with this
project libraries).   

```bash
  $ ./setup.sh uninstall 
```