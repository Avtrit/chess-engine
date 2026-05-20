# Chess Engine

A chess game built in C++ using SFML. Play a two player game of chess with move highlighting and turn enforcement.

## Features

- Full chess board with all pieces
- Valid move generation for all pieces (pawns, rooks, bishops, knights, queens, kings)
- Click to select a piece, green squares show valid moves
- Selected piece highlighted in yellow
- Turn based system, white moves first
- Prevents moving to illegal squares

## Dependencies

- [SFML](https://www.sfml-dev.org/) — Simple and Fast Multimedia Library

## Installation

### 1. Install SFML
Download and install SFML from https://www.sfml-dev.org/download.php

### 2. Clone the repository
git clone https://github.com/Avtrit/chess-engine.git
cd chess-engine

### 3. Compile
g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -o chess

### 4. Run
./chess

## How to Play

1. Click on any of your pieces to select it
2. Green squares will appear showing where it can move
3. Click a green square to move there
4. White goes first, then turns alternate between white and black

## Project Structure

chess-engine/
├── main.cpp        # Main source file
├── pieces/         # Piece images
└── README.md

## Credits

Piece images sourced from Lichess (lichess.org)
https://github.com/lichess-org/lila/tree/master/public/piece

## Planned Features

- Check and checkmate detection
- Castling
- En passant
- Pawn promotion
- AI opponent
