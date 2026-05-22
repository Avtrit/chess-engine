# Chess Engine

A chess game built in C++ using SFML. Play a two player game of chess with move highlighting and turn enforcement.


## TASKS FOR THE PROJECT 22/05/26

- limit the use of ai only to sfml
- implement the board with pieces
- write the moves for the pieces 
- change the cursor when moving a piece
- highlight the squares when moving the pieces
- highlight the squares red if a capture is possibe
- implement check
- implement chekmate and stalemate 

## TIMELINE OF THE PROJECT(for personal record, plz feel free to skip it)
### 19/05/26
- made the basic board structure via a string and sfml
- copied the pieces images from lichess
### 20/05/26
- implemented the moves of each piece
- black and white seperately did not have to implement capture got covered in valid moves itself
- created current turn boolean
### 21/05/26
- wrote comments till the above part
- implemented pawn promotion
- implemented pawn promotion with a drop down
### 22/05/26
- implemented check(with a popup)
- implemented checkmate and stalemate


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
