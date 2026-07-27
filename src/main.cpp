#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <atomic>
using namespace std;
using namespace sf;

const int TILE_SIZE = 100;

// declaring the board as a string 
string board[8][8] = {
    {"bR","bN","bB","bQ","bK","bB","bN","bR"},
    {"bP","bP","bP","bP","bP","bP","bP","bP"},
    {"","","","","","","",""},
    {"","","","","","","",""},
    {"","","","","","","",""},
    {"","","","","","","",""},
    {"wP","wP","wP","wP","wP","wP","wP","wP"},
    {"wR","wN","wB","wQ","wK","wB","wN","wR"}
};

//moves for different pieces
void RookMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
    for(int r = row-1; r >= 0; r--){//up for white, down for black
        if(board[r][col] == ""){
            moves.push_back({r, col});// can move and even conitnue moving if piece is our color 
        }
        else if(board[r][col][0] != myColor){
            moves.push_back({r, col});
            break;// last sqaure to move if the piece is not of our color
        }
        else{
            break;//stop if piece is our color
        }
    }
    for(int r = row+1; r < 8; r++){//up for black, down for white 
        if(board[r][col] == ""){
            moves.push_back({r, col});
        }
        else if(board[r][col][0] != myColor){
            moves.push_back({r, col});
            break; 
        }
        else{
            break;
        }
    }
    for(int c = col-1; c >= 0; c--){//left
        if(board[row][c] == ""){
            moves.push_back({row, c});
        }
        else if(board[row][c][0] != myColor){
            moves.push_back({row, c});
            break;
        }
        else{
            break;
        }
    }
    for(int c = col+1; c < 8; c++){//right 
        if(board[row][c] == ""){
            moves.push_back({row, c});
        }
        else if(board[row][c][0] != myColor){
            moves.push_back({row, c});
            break;
        }
        else{
            break;
        }
    }
}

void BishopMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
    for(int i = 1; row-i >= 0 && col-i >= 0; i++){// north west 
        if(board[row-i][col-i] == ""){
            moves.push_back({row-i, col-i});//can move and even conitnue moving if piece is our color 
        }
        else if(board[row-i][col-i][0] != myColor){
            moves.push_back({row-i, col-i});
            break;//last sqaure to move if the piece is not of our color
        }
        else{
            break;//stop if piece is our color
        }
    }
    for(int i = 1; row-i >= 0 && col+i < 8; i++){// north east 
        if(board[row-i][col+i] == ""){
            moves.push_back({row-i, col+i});
        }
        else if(board[row-i][col+i][0] != myColor){
            moves.push_back({row-i, col+i});
            break;
        }
        else{
            break;
        }
    }
    for(int i = 1; row+i < 8 && col-i >= 0; i++){ //south west 
        if(board[row+i][col-i] == ""){
            moves.push_back({row+i, col-i});
        }
        else if(board[row+i][col-i][0] != myColor){
            moves.push_back({row+i, col-i});
            break;
        }
        else{
            break;
        }
    }
    for(int i = 1; row+i < 8 && col+i < 8; i++){ //south east 
        if(board[row+i][col+i] == ""){
            moves.push_back({row+i, col+i});
        }
        else if(board[row+i][col+i][0] != myColor){
            moves.push_back({row+i, col+i});
            break;
        }
        else{
            break;
        }
    }
}

void KnightMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
    int jumps[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for(auto& j : jumps){
        int r = row + j[0], c = col + j[1];
        if(r >= 0 && r < 8 && c >= 0 && c < 8){
            if(board[r][c] == "" || board[r][c][0] != myColor){
                moves.push_back({r, c});
            }
        }
    }
}

void KingMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
    for(int dr = -1; dr <= 1; dr++){
        for(int dc = -1; dc <= 1; dc++){
            if(dr == 0 && dc == 0){
                continue;
            }
            int r = row+dr, c = col+dc;
            if(r >= 0 && r < 8 && c >= 0 && c < 8){
                if(board[r][c] == "" || board[r][c][0] != myColor){
                    moves.push_back({r, c});
                }
            }
        }
    }
}

bool pawn_double = false;
bool enPassantPossible = false;
int enPassantRow = -1;
int enPassantCol = -1;

bool wK_moved = false, wR_left_moved = false, wR_right_moved = false;
bool bK_moved = false, bR_left_moved = false, bR_right_moved = false;

int halfMoveClock = 0;
map<string, int> positionHistory;

// Forward declarations
bool isKinginCheck(char color);
bool isSquareAttacked(int row,int col, char attackerColor);

string getBoardStateString(char turn) {
    string state = "";
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j] == "") state += ".";
            else state += board[i][j];
        }
    }
    state += turn;
    state += wK_moved ? "0" : "1";
    state += wR_left_moved ? "0" : "1";
    state += wR_right_moved ? "0" : "1";
    state += bK_moved ? "0" : "1";
    state += bR_left_moved ? "0" : "1";
    state += bR_right_moved ? "0" : "1";
    if(enPassantPossible){
        state += to_string(enPassantRow) + to_string(enPassantCol);
    } else {
        state += "X";
    }
    return state;
}

bool isInsufficientMaterial() {
    int wN = 0, wB = 0, bN = 0, bB = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            string p = board[i][j];
            if(p == "") continue;
            if(p[1] == 'P' || p[1] == 'R' || p[1] == 'Q'){
                return false;
            }
            if(p == "wN") wN++;
            else if(p == "wB") wB++;
            else if(p == "bN") bN++;
            else if(p == "bB") bB++;
        }
    }
    if((wN + wB + bN + bB) <= 1){
        return true;
    }
    return false;
}

vector<pair<int,int>> getValidMoves(int row, int col){
    vector<pair<int,int>> moves;
    string piece = board[row][col];
    char myColor = piece[0];

    if(piece == "wP"){
        if(row-1 >= 0 && board[row-1][col] == ""){
            moves.push_back({row-1, col});
        }
        if(row == 6 && board[row-1][col] == "" && board[row-2][col] == ""){
            moves.push_back({row-2, col});
        }
        if(row-1 >= 0 && col-1 >= 0 && board[row-1][col-1] != "" && board[row-1][col-1][0] == 'b'){
            moves.push_back({row-1, col-1});
        }
        if(row-1 >= 0 && col+1 < 8 && board[row-1][col+1] != "" && board[row-1][col+1][0] == 'b'){
            moves.push_back({row-1, col+1});
        }
        if(enPassantPossible && row == 3 && enPassantRow == 2 && (enPassantCol == col - 1 || enPassantCol == col + 1)){
            moves.push_back({enPassantRow, enPassantCol});
        }
    }
    else if(piece == "bP"){
        if(row+1 < 8 && board[row+1][col] == ""){
            moves.push_back({row+1, col});
        }
        if(row == 1 && board[row+1][col] == "" && board[row+2][col] == ""){
            moves.push_back({row+2, col});
        }
        if(row+1 < 8 && col-1 >= 0 && board[row+1][col-1] != "" && board[row+1][col-1][0] == 'w'){
            moves.push_back({row+1, col-1});
        }
        if(row+1 < 8 && col+1 < 8 && board[row+1][col+1] != "" && board[row+1][col+1][0] == 'w'){
            moves.push_back({row+1, col+1});
        }
        if(enPassantPossible && row == 4 && enPassantRow == 5 && (enPassantCol == col - 1 || enPassantCol == col + 1)){
            moves.push_back({enPassantRow, enPassantCol});
        }
    }
    else if(piece == "wR" || piece == "bR"){
        RookMoves(moves, row, col, myColor);
    }
    else if(piece == "wB" || piece == "bB"){
        BishopMoves(moves, row, col, myColor);
    }
    else if(piece == "wN" || piece == "bN"){
        KnightMoves(moves, row, col, myColor);
    }
    else if(piece == "wQ" || piece == "bQ"){// rook and bishop moves 
        RookMoves(moves, row, col, myColor);
        BishopMoves(moves, row, col, myColor);
    }
    else if(piece == "wK"){
        KingMoves(moves, row, col, myColor);
        if(!wK_moved && !isKinginCheck('w')){
            if(!wR_right_moved && board[7][7] == "wR" && board[7][5] == "" && board[7][6] == "" && !isSquareAttacked(7, 5, 'b') && !isSquareAttacked(7, 6, 'b')){
                moves.push_back({7, 6});
            }
            if(!wR_left_moved && board[7][0] == "wR" && board[7][1] == "" && board[7][2] == "" && board[7][3] == "" && !isSquareAttacked(7, 2, 'b') && !isSquareAttacked(7, 3, 'b')){
                moves.push_back({7, 2});
            }
        }
    }
    else if(piece == "bK"){
        KingMoves(moves, row, col, myColor);
        if(!bK_moved && !isKinginCheck('b')){
            if(!bR_right_moved && board[0][7] == "bR" && board[0][5] == "" && board[0][6] == "" && !isSquareAttacked(0, 5, 'w') && !isSquareAttacked(0, 6, 'w')){
                moves.push_back({0, 6});
            }
            if(!bR_left_moved && board[0][0] == "bR" && board[0][1] == "" && board[0][2] == "" && board[0][3] == "" && !isSquareAttacked(0, 2, 'w') && !isSquareAttacked(0, 3, 'w')){
                moves.push_back({0, 2});
            }
        }
    }
    return moves;
}

bool isSquareAttacked(int row,int col, char attackerColor){
    vector<pair<int,int>> moves;
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            if(board[i][j] != "" &&
               board[i][j][0] == attackerColor){

                string piece = board[i][j];
                if(piece == "wP"){
                    if((i == row + 1) && (j == col - 1 || j == col + 1)){
                        return true;
                    }
                }
                else if(piece == "bP"){
                    if((i == row - 1) && (j == col - 1 || j == col + 1)){
                        return true;
                    }
                }
                else if(piece == "wR" || piece == "bR"){
                    moves.clear();
                    RookMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wB" || piece == "bB"){
                    moves.clear();
                    BishopMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wN" || piece == "bN"){
                    moves.clear();
                    KnightMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wQ" || piece == "bQ"){
                    moves.clear();
                    RookMoves(moves, i, j, attackerColor);
                    BishopMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wK" || piece == "bK"){
                    moves.clear();
                    KingMoves(moves, i, j, attackerColor);
                }

                for(auto &p : moves){
                    if(row == p.first &&
                       col == p.second){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

pair<int,int> findKing(char color){
    for(int i= 0 ; i<8 ; i++){
        for(int j = 0 ; j< 8;j++){
            if(board[i][j] !="" && board[i][j][0]==color && board[i][j][1]=='K'){
                return {i,j};
            }
        }
    }
    return{-1,-1};
}

bool isKinginCheck(char color){
    pair<int,int> k;
    k = findKing(color);

    char enemy = (color == 'w') ? 'b' : 'w';

    return isSquareAttacked(k.first, k.second, enemy);
}

bool anyValidMoves(char color){
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){

            if(board[i][j] != "" && board[i][j][0] == color){

                vector<pair<int,int>> moves = getValidMoves(i,j);

                for(auto &[r,c] : moves){

                    string movedPiece = board[i][j];
                    string capturedPiece = board[r][c];

                    bool isEnPassant = false;
                    if(movedPiece[1] == 'P' && c != j && capturedPiece == ""){
                        isEnPassant = true;
                        board[i][c] = ""; // remove the captured pawn
                    }

                    // make move
                    board[r][c] = movedPiece;
                    board[i][j] = "";

                    bool isCastling = false;
                    int rookFromC = -1, rookToC = -1;
                    if(movedPiece[1] == 'K' && (c == j + 2 || c == j - 2)){
                        isCastling = true;
                        if(c == 6){ rookFromC = 7; rookToC = 5; } // Kingside
                        else if(c == 2){ rookFromC = 0; rookToC = 3; } // Queenside
                        
                        board[r][rookToC] = board[r][rookFromC];
                        board[r][rookFromC] = "";
                    }

                    // check legality
                    bool illegal = isKinginCheck(color);

                    // undo move
                    board[i][j] = movedPiece;
                    board[r][c] = capturedPiece;
                    if(isEnPassant){
                        board[i][c] = (color == 'w') ? "bP" : "wP";
                    }
                    if(isCastling){
                        board[r][rookFromC] = board[r][rookToC];
                        board[r][rookToC] = "";
                    }

                    // if king safe -> at least one legal move exists
                    if(!illegal){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isCheckmate(char color){
    return isKinginCheck(color) && !anyValidMoves(color);
}

bool isStalemate(char color){
    return !isKinginCheck(color) && !anyValidMoves(color);
}

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
};

vector<Move> getAllLegalMoves(char color) {
    vector<Move> legalMoves;
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            if(board[i][j] != "" && board[i][j][0] == color){
                vector<pair<int,int>> pseudoMoves = getValidMoves(i,j);
                for(auto &[r,c] : pseudoMoves){
                    string movedPiece = board[i][j];
                    string capturedPiece = board[r][c];
                    bool isEnPassant = false;
                    if(movedPiece[1] == 'P' && c != j && capturedPiece == ""){
                        isEnPassant = true;
                        board[i][c] = ""; 
                    }
                    bool isCastling = false;
                    int rookFromC = -1, rookToC = -1;
                    if(movedPiece[1] == 'K' && (c == j + 2 || c == j - 2)){
                        isCastling = true;
                        if(c == 6){ rookFromC = 7; rookToC = 5; } 
                        else if(c == 2){ rookFromC = 0; rookToC = 3; } 
                        board[r][rookToC] = board[r][rookFromC];
                        board[r][rookFromC] = "";
                    }
                    board[r][c] = movedPiece;
                    board[i][j] = "";
                    bool illegal = isKinginCheck(color);
                    board[i][j] = movedPiece;
                    board[r][c] = capturedPiece;
                    if(isEnPassant){
                        board[i][c] = (color == 'w') ? "bP" : "wP";
                    }
                    if(isCastling){
                        board[r][rookFromC] = board[r][rookToC];
                        board[r][rookToC] = "";
                    }
                    if(!illegal){
                        legalMoves.push_back({i, j, r, c});
                    }
                }
            }
        }
    }
    return legalMoves;
}

const int pawnEvalWhite[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {5,  5, 10, 25, 25, 10,  5,  5},
    {0,  0,  0, 20, 20,  0,  0,  0},
    {5, -5,-10,  0,  0,-10, -5,  5},
    {5, 10, 10,-20,-20, 10, 10,  5},
    {0,  0,  0,  0,  0,  0,  0,  0}
};

const int knightEval[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int bishopEvalWhite[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

const int rookEvalWhite[8][8] = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 5, 10, 10, 10, 10, 10, 10,  5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    { 0,  0,  0,  5,  5,  0,  0,  0}
};

const int evalQueen[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    { 0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};

const int kingEvalWhite[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20}
};

int getPieceValue(string p) {
    if(p == "") return 0;
    if(p[1] == 'P') return 100;
    if(p[1] == 'N') return 300;
    if(p[1] == 'B') return 300;
    if(p[1] == 'R') return 500;
    if(p[1] == 'Q') return 900;
    if(p[1] == 'K') return 100000;
    return 0;
}

void orderMoves(vector<Move>& moves) {
    vector<pair<int, Move>> scoredMoves;
    for(Move m : moves) {
        int score = 0;
        string movedPiece = board[m.fromRow][m.fromCol];
        string capturedPiece = board[m.toRow][m.toCol];
        
        if (capturedPiece != "") {
            score = 10 * getPieceValue(capturedPiece) - getPieceValue(movedPiece);
        }
        if (movedPiece[1] == 'P' && (m.toRow == 0 || m.toRow == 7)) {
            score += 900; 
        }
        scoredMoves.push_back({score, m});
    }
    sort(scoredMoves.begin(), scoredMoves.end(), [](const pair<int, Move>& a, const pair<int, Move>& b) {
        return a.first > b.first;
    });
    for(size_t i = 0; i < moves.size(); i++) {
        moves[i] = scoredMoves[i].second;
    }
}

int evaluateBoard() {
    int score = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            string p = board[i][j];
            if(p == "") continue;
            bool isWhite = (p[0] == 'w');
            int row = isWhite ? i : (7 - i);

            int val = 0;
            if(p[1] == 'P') val = 100 + pawnEvalWhite[row][j];
            else if(p[1] == 'N') val = 300 + knightEval[row][j];
            else if(p[1] == 'B') val = 300 + bishopEvalWhite[row][j];
            else if(p[1] == 'R') val = 500 + rookEvalWhite[row][j];
            else if(p[1] == 'Q') val = 900 + evalQueen[row][j];
            else if(p[1] == 'K') val = 100000 + kingEvalWhite[row][j];

            if(isWhite) score += val;
            else score -= val;
        }
    }
    return score;
}

int nodesEvaluated = 0;

int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer) {
    nodesEvaluated++;
    char color = isMaximizingPlayer ? 'w' : 'b';
    if(isCheckmate(color)) return isMaximizingPlayer ? -999999 : 999999;
    if(isStalemate(color) || isInsufficientMaterial() || halfMoveClock >= 100) return 0;
    if (depth == 0) return evaluateBoard();

    vector<Move> moves = getAllLegalMoves(color);
    orderMoves(moves);
    int bestEval = isMaximizingPlayer ? -1e9 : 1e9;

    for (Move m : moves) {
        string movedPiece = board[m.fromRow][m.fromCol];
        string capturedPiece = board[m.toRow][m.toCol];
        bool prevEP = enPassantPossible;
        int prevEPR = enPassantRow, prevEPC = enPassantCol;
        bool prev_wK = wK_moved, prev_wR_l = wR_left_moved, prev_wR_r = wR_right_moved;
        bool prev_bK = bK_moved, prev_bR_l = bR_left_moved, prev_bR_r = bR_right_moved;
        int prevHMC = halfMoveClock;
        
        bool isEnPassant = false;
        if(movedPiece[1] == 'P' && m.toCol != m.fromCol && capturedPiece == ""){
            isEnPassant = true;
            board[m.fromRow][m.toCol] = ""; 
        }
        bool isCastling = false;
        int rookFromC = -1, rookToC = -1;
        if (movedPiece[1] == 'K' && abs(m.toCol - m.fromCol) == 2) {
            isCastling = true;
            if (m.toCol == 6) { rookFromC = 7; rookToC = 5; } 
            else if (m.toCol == 2) { rookFromC = 0; rookToC = 3; } 
            board[m.toRow][rookToC] = board[m.toRow][rookFromC];
            board[m.toRow][rookFromC] = "";
        }
        board[m.toRow][m.toCol] = movedPiece;
        board[m.fromRow][m.fromCol] = "";
        
        if(movedPiece == "wP" && m.toRow == 0) board[m.toRow][m.toCol] = "wQ";
        else if(movedPiece == "bP" && m.toRow == 7) board[m.toRow][m.toCol] = "bQ";

        if (movedPiece[1] == 'P' || capturedPiece != "") halfMoveClock = 0;
        else halfMoveClock++;

        enPassantPossible = false;
        if (movedPiece[1] == 'P' && abs(m.toRow - m.fromRow) == 2) {
            enPassantPossible = true;
            enPassantRow = (m.fromRow + m.toRow) / 2;
            enPassantCol = m.fromCol;
        }

        if (movedPiece == "wK") wK_moved = true;
        else if (movedPiece == "bK") bK_moved = true;
        else if (movedPiece == "wR"){
            if (m.fromRow == 7 && m.fromCol == 0) wR_left_moved = true;
            if (m.fromRow == 7 && m.fromCol == 7) wR_right_moved = true;
        }
        else if (movedPiece == "bR"){
            if (m.fromRow == 0 && m.fromCol == 0) bR_left_moved = true;
            if (m.fromRow == 0 && m.fromCol == 7) bR_right_moved = true;
        }

        int eval = minimax(depth - 1, alpha, beta, !isMaximizingPlayer);

        board[m.fromRow][m.fromCol] = movedPiece;
        board[m.toRow][m.toCol] = capturedPiece;
        if(isEnPassant) board[m.fromRow][m.toCol] = (color == 'w') ? "bP" : "wP";
        if(isCastling){
            board[m.toRow][rookFromC] = board[m.toRow][rookToC];
            board[m.toRow][rookToC] = "";
        }
        enPassantPossible = prevEP; enPassantRow = prevEPR; enPassantCol = prevEPC;
        wK_moved = prev_wK; wR_left_moved = prev_wR_l; wR_right_moved = prev_wR_r;
        bK_moved = prev_bK; bR_left_moved = prev_bR_l; bR_right_moved = prev_bR_r;
        halfMoveClock = prevHMC;

        if (isMaximizingPlayer) {
            bestEval = max(bestEval, eval);
            alpha = max(alpha, eval);
        } else {
            bestEval = min(bestEval, eval);
            beta = min(beta, eval);
        }
        if (beta <= alpha) break;
    }
    return bestEval;
}

Move findBestMove(char color, int depth) {
    vector<Move> moves = getAllLegalMoves(color);
    orderMoves(moves);
    Move bestMove = moves.empty() ? Move{-1,-1,-1,-1} : moves[0];
    bool isMaximizing = (color == 'w');
    int bestEval = isMaximizing ? -1e9 : 1e9;
    int alpha = -1e9;
    int beta = 1e9;
    nodesEvaluated = 0;

    for (Move m : moves) {
        string movedPiece = board[m.fromRow][m.fromCol];
        string capturedPiece = board[m.toRow][m.toCol];
        bool prevEP = enPassantPossible;
        int prevEPR = enPassantRow, prevEPC = enPassantCol;
        bool prev_wK = wK_moved, prev_wR_l = wR_left_moved, prev_wR_r = wR_right_moved;
        bool prev_bK = bK_moved, prev_bR_l = bR_left_moved, prev_bR_r = bR_right_moved;
        int prevHMC = halfMoveClock;
        
        bool isEnPassant = false;
        if(movedPiece[1] == 'P' && m.toCol != m.fromCol && capturedPiece == ""){
            isEnPassant = true;
            board[m.fromRow][m.toCol] = ""; 
        }
        bool isCastling = false;
        int rookFromC = -1, rookToC = -1;
        if (movedPiece[1] == 'K' && abs(m.toCol - m.fromCol) == 2) {
            isCastling = true;
            if (m.toCol == 6) { rookFromC = 7; rookToC = 5; } 
            else if (m.toCol == 2) { rookFromC = 0; rookToC = 3; } 
            board[m.toRow][rookToC] = board[m.toRow][rookFromC];
            board[m.toRow][rookFromC] = "";
        }
        board[m.toRow][m.toCol] = movedPiece;
        board[m.fromRow][m.fromCol] = "";
        
        if(movedPiece == "wP" && m.toRow == 0) board[m.toRow][m.toCol] = "wQ";
        else if(movedPiece == "bP" && m.toRow == 7) board[m.toRow][m.toCol] = "bQ";

        if (movedPiece[1] == 'P' || capturedPiece != "") halfMoveClock = 0;
        else halfMoveClock++;

        enPassantPossible = false;
        if (movedPiece[1] == 'P' && abs(m.toRow - m.fromRow) == 2) {
            enPassantPossible = true;
            enPassantRow = (m.fromRow + m.toRow) / 2;
            enPassantCol = m.fromCol;
        }
        if (movedPiece == "wK") wK_moved = true;
        else if (movedPiece == "bK") bK_moved = true;
        else if (movedPiece == "wR"){
            if (m.fromRow == 7 && m.fromCol == 0) wR_left_moved = true;
            if (m.fromRow == 7 && m.fromCol == 7) wR_right_moved = true;
        }
        else if (movedPiece == "bR"){
            if (m.fromRow == 0 && m.fromCol == 0) bR_left_moved = true;
            if (m.fromRow == 0 && m.fromCol == 7) bR_right_moved = true;
        }

        int eval = minimax(depth - 1, alpha, beta, !isMaximizing);

        board[m.fromRow][m.fromCol] = movedPiece;
        board[m.toRow][m.toCol] = capturedPiece;
        if(isEnPassant) board[m.fromRow][m.toCol] = (color == 'w') ? "bP" : "wP";
        if(isCastling){
            board[m.toRow][rookFromC] = board[m.toRow][rookToC];
            board[m.toRow][rookToC] = "";
        }
        enPassantPossible = prevEP; enPassantRow = prevEPR; enPassantCol = prevEPC;
        wK_moved = prev_wK; wR_left_moved = prev_wR_l; wR_right_moved = prev_wR_r;
        bK_moved = prev_bK; bR_left_moved = prev_bR_l; bR_right_moved = prev_bR_r;
        halfMoveClock = prevHMC;

        if (isMaximizing) {
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = m;
            }
            alpha = max(alpha, eval);
        } else {
            if (eval < bestEval) {
                bestEval = eval;
                bestMove = m;
            }
            beta = min(beta, eval);
        }
    }
    return bestMove;
}

int main(){
    RenderWindow window(VideoMode({1100, 800}), "Chess Engine");//window created 
    map<string, Texture> textures;
    //declaring array pieces 
    string pieces[] = {"wK","wQ","wR","wB","wN","wP",
                       "bK","bQ","bR","bB","bN","bP"};
    for(auto& p : pieces){
        Texture t;
        if(t.loadFromFile("pieces/" + p + ".png")){
            textures[p] = move(t);
        }
    }
    bool selected = false;
    int selectedRow = -1, selectedCol = -1;
    vector<pair<int,int>> validMoves;

    char currentTurn = 'w';
    bool aiEnabled = true;
    char aiColor = 'b';

    bool gameOver = false;
    string gameResult = "";
    
    positionHistory.clear();
    halfMoveClock = 0;
    positionHistory[getBoardStateString(currentTurn)]++;

    Font font;
    if(!font.openFromFile("src/arial.ttf")){
        //so that it does not show a warning 
    };

    // promotion variables
    bool promoting = false;
    int promotionRow = -1, promotionCol = -1;
    char promotionColor = ' ';

    //loading cursor types 
    auto defaultCursor = Cursor::createFromSystem(Cursor::Type::Arrow);
    auto grabCursor = Cursor::createFromSystem(Cursor::Type::Hand);

    float whiteTime = 600.0f;
    float blackTime = 600.0f;
    Clock gameClock;

    atomic<bool> isAiThinking(false);
    atomic<bool> aiMoveReady(false);
    Move aiBestMove{-1,-1,-1,-1};

    while(window.isOpen()){
        float dt = gameClock.restart().asSeconds();
        if (!gameOver) {
            if (currentTurn == 'w') whiteTime -= dt;
            else blackTime -= dt;

            if (whiteTime <= 0) {
                whiteTime = 0;
                gameOver = true;
                gameResult = "Timeout! Black Wins!";
            } else if (blackTime <= 0) {
                blackTime = 0;
                gameOver = true;
                gameResult = "Timeout! White Wins!";
            }
        }

        while(const optional event = window.pollEvent()){
            if(event->is<Event::Closed>()){
                window.close();
            }
            else if(event->is<Event::MouseButtonPressed>() && !gameOver && currentTurn != aiColor){
                auto mouse = Mouse::getPosition(window);
                int col = mouse.x / TILE_SIZE;
                int row = mouse.y / TILE_SIZE;

                if(promoting){
                    string options[] = {"Q","R","B","N"};
                    for(int i = 0; i < 4; i++){
                        if(col == promotionCol && row == i){
                            board[promotionRow][promotionCol] = string(1, promotionColor) + options[i];
                            promoting = false;
                        }
                    }
                    selected = false;
                    validMoves.clear();
                }
                else if(!selected){
                    if(board[row][col] != "" && board[row][col][0] == currentTurn){
                        selected = true;
                        selectedRow = row;
                        selectedCol = col;
                        validMoves = getValidMoves(row, col);
                        window.setMouseCursor(*grabCursor);// changes the cursor to hand cursor 
                    }
                }
                else{
                    // check if the move is in validMoves
                    bool isValid = false;
                    for(auto& [r, c] : validMoves){
                        if(r == row && c == col){
                            isValid = true;
                            break;
                        }
                    }

                    if(isValid){
                        string movedPiece = board[selectedRow][selectedCol];
                        string capturedPiece = board[row][col];

                        board[row][col] = movedPiece;
                        board[selectedRow][selectedCol] = "";

                        bool isEnPassant = false;
                        if(movedPiece[1] == 'P' && col != selectedCol && capturedPiece == ""){
                            isEnPassant = true;
                            board[selectedRow][col] = ""; // remove the captured pawn
                        }

                        bool isCastling = false;
                        int rookFromC = -1, rookToC = -1;
                        if (movedPiece[1] == 'K' && (col == selectedCol + 2 || col == selectedCol - 2)) {
                            isCastling = true;
                            if (col == 6) { rookFromC = 7; rookToC = 5; } // Kingside
                            else if (col == 2) { rookFromC = 0; rookToC = 3; } // Queenside
                            
                            board[row][rookToC] = board[row][rookFromC];
                            board[row][rookFromC] = "";
                        }

                        if(isKinginCheck(currentTurn)){
                            // undo the move
                            board[selectedRow][selectedCol] = movedPiece;
                            board[row][col] = capturedPiece;
                            if(isEnPassant){
                                board[selectedRow][col] = (currentTurn == 'w') ? "bP" : "wP";
                            }
                            if(isCastling){
                                board[row][rookFromC] = board[row][rookToC];
                                board[row][rookToC] = "";
                            }
                        }
                        else{
                            if (movedPiece[1] == 'P' || capturedPiece != "") {
                                halfMoveClock = 0;
                                positionHistory.clear(); // pawn moves and captures make previous positions unreachable
                            } else {
                                halfMoveClock++;
                            }

                            if (movedPiece == "wK") wK_moved = true;
                            else if (movedPiece == "bK") bK_moved = true;
                            else if (movedPiece == "wR"){
                                if (selectedRow == 7 && selectedCol == 0) wR_left_moved = true;
                                if (selectedRow == 7 && selectedCol == 7) wR_right_moved = true;
                            }
                            else if (movedPiece == "bR"){
                                if (selectedRow == 0 && selectedCol == 0) bR_left_moved = true;
                                if (selectedRow == 0 && selectedCol == 7) bR_right_moved = true;
                            }

                            enPassantPossible = false;

                            if(movedPiece == "wP" && selectedRow == 6 && row == 4){
                                enPassantPossible = true;
                                enPassantRow = 5;
                                enPassantCol = col;
                            }

                            if(movedPiece == "bP" && selectedRow == 1 && row == 3){
                                enPassantPossible = true;
                                enPassantRow = 2;
                                enPassantCol = col;
                            }

                            currentTurn = (currentTurn == 'w') ? 'b' : 'w';
                            string currentState = getBoardStateString(currentTurn);
                            positionHistory[currentState]++;

                            if(isCheckmate(currentTurn)){
                                gameOver = true;
                                gameResult =(currentTurn == 'w')
                                    ? "Checkmate! Black Wins!"
                                    : "Checkmate! White Wins!";
                            }
                            else if(isStalemate(currentTurn)){
                                gameOver = true;
                                gameResult = "Stalemate!";
                            } 
                            else if(halfMoveClock >= 100){
                                gameOver = true;
                                gameResult = "Draw by 50-Move Rule!";
                            }
                            else if(positionHistory[currentState] >= 3){
                                gameOver = true;
                                gameResult = "Draw by Repetition!";
                            }
                            else if(isInsufficientMaterial()){
                                gameOver = true;
                                gameResult = "Draw: Insufficient Material!";
                            } // draw

                            // check for pawn promotion
                            if(board[row][col] == "wP" && row == 0){
                                promoting = true;
                                promotionRow = row;
                                promotionCol = col;
                                promotionColor = 'w';
                            }
                            else if(board[row][col] == "bP" && row == 7){
                                promoting = true;
                                promotionRow = row;
                                promotionCol = col;
                                promotionColor = 'b';
                            }
                        }
                    }

                    selected = false;
                    validMoves.clear();
                    window.setMouseCursor(*defaultCursor); // changing cursor back to arrow
                }
            }
        }

        if (!gameOver && aiEnabled && currentTurn == aiColor) {
            if (!isAiThinking && !aiMoveReady) {
                isAiThinking = true;
                thread([&]() {
                    aiBestMove = findBestMove(aiColor, 5);
                    aiMoveReady = true;
                    isAiThinking = false;
                }).detach();
            }
        }

        if (aiMoveReady) {
            Move best = aiBestMove;
            aiMoveReady = false;
            
            if (best.fromRow != -1) {
                string movedPiece = board[best.fromRow][best.fromCol];
                string capturedPiece = board[best.toRow][best.toCol];
                
                bool isEnPassant = false;
                if(movedPiece[1] == 'P' && best.toCol != best.fromCol && capturedPiece == ""){
                    isEnPassant = true;
                    board[best.fromRow][best.toCol] = ""; 
                }

                bool isCastling = false;
                int rookFromC = -1, rookToC = -1;
                if (movedPiece[1] == 'K' && abs(best.toCol - best.fromCol) == 2) {
                    isCastling = true;
                    if (best.toCol == 6) { rookFromC = 7; rookToC = 5; } 
                    else if (best.toCol == 2) { rookFromC = 0; rookToC = 3; } 
                    board[best.toRow][rookToC] = board[best.toRow][rookFromC];
                    board[best.toRow][rookFromC] = "";
                }

                board[best.toRow][best.toCol] = movedPiece;
                board[best.fromRow][best.fromCol] = "";
                
                if(movedPiece == "wP" && best.toRow == 0) board[best.toRow][best.toCol] = "wQ";
                else if(movedPiece == "bP" && best.toRow == 7) board[best.toRow][best.toCol] = "bQ";

                if (movedPiece[1] == 'P' || capturedPiece != "") {
                    halfMoveClock = 0;
                    positionHistory.clear();
                } else {
                    halfMoveClock++;
                }

                if (movedPiece == "wK") wK_moved = true;
                else if (movedPiece == "bK") bK_moved = true;
                else if (movedPiece == "wR"){
                    if (best.fromRow == 7 && best.fromCol == 0) wR_left_moved = true;
                    if (best.fromRow == 7 && best.fromCol == 7) wR_right_moved = true;
                }
                else if (movedPiece == "bR"){
                    if (best.fromRow == 0 && best.fromCol == 0) bR_left_moved = true;
                    if (best.fromRow == 0 && best.fromCol == 7) bR_right_moved = true;
                }

                enPassantPossible = false;
                if (movedPiece[1] == 'P' && abs(best.toRow - best.fromRow) == 2) {
                    enPassantPossible = true;
                    enPassantRow = (best.fromRow + best.toRow) / 2;
                    enPassantCol = best.fromCol;
                }

                currentTurn = (currentTurn == 'w') ? 'b' : 'w';
                string currentState = getBoardStateString(currentTurn);
                positionHistory[currentState]++;

                if(isCheckmate(currentTurn)){
                    gameOver = true;
                    gameResult = (currentTurn == 'w') ? "Checkmate! Black Wins!" : "Checkmate! White Wins!";
                }
                else if(isStalemate(currentTurn)){
                    gameOver = true;
                    gameResult = "Stalemate!";
                } 
                else if(halfMoveClock >= 100){
                    gameOver = true;
                    gameResult = "Draw by 50-Move Rule!";
                }
                else if(positionHistory[currentState] >= 3){
                    gameOver = true;
                    gameResult = "Draw by Repetition!";
                }
                else if(isInsufficientMaterial()){
                    gameOver = true;
                    gameResult = "Draw: Insufficient Material!";
                }
            }
        }

        window.clear();
        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                RectangleShape tile(Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                if((row + col) % 2 == 0){
                    tile.setFillColor(Color(237, 230, 247));
                }
                else{
                    tile.setFillColor(Color(130, 100, 180));
                }
                window.draw(tile);
                if(selected && row == selectedRow && col == selectedCol){
                    RectangleShape highlight(Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                    highlight.setFillColor(Color(255, 255, 0, 100));
                    window.draw(highlight);
                }
                for(auto& [r, c] : validMoves){
                    RectangleShape highlight(Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(
                        Vector2f(col * TILE_SIZE,
                                row * TILE_SIZE)
                    );
                    if(board[r][c] != ""){
                        highlight.setFillColor(
                            Color(255, 0, 0, 120)
                        );
                    }
                    else{
                        highlight.setFillColor(
                            Color(0, 255, 0, 100)
                        );
                    }
                    if(row == r && col == c){
                        window.draw(highlight);
                    }
                }
                string piece = board[row][col];
                if(!piece.empty() && textures.count(piece)){
                    Sprite sprite(textures[piece]);
                    auto size = textures[piece].getSize();
                    sprite.setScale(Vector2f(
                        (float)TILE_SIZE / size.x,
                        (float)TILE_SIZE / size.y
                    ));
                    sprite.setPosition(Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                    window.draw(sprite);
                }
            }
        }

        if(isKinginCheck(currentTurn)){
            RectangleShape overlay(Vector2f(800, 50));
            overlay.setPosition(Vector2f(0, 375));
            overlay.setFillColor(Color(0, 0, 0, 200));
            window.draw(overlay);

            Text checkText(font);
            checkText.setString(currentTurn == 'w' ? "White is in Check!" : "Black is in Check!");
            checkText.setCharacterSize(36);
            checkText.setFillColor(Color::Red);
            checkText.setStyle(Text::Bold);

            auto bounds = checkText.getLocalBounds();
            checkText.setOrigin(bounds.size / 2.f);
            checkText.setPosition(Vector2f(400, 400));

            window.draw(checkText);
        }

        // draw promotion popup
        if(promoting){
            string options[] = {"Q","R","B","N"};
            for(int i = 0; i < 4; i++){
                RectangleShape bg(Vector2f(TILE_SIZE, TILE_SIZE));
                bg.setPosition(Vector2f(promotionCol * TILE_SIZE,i* TILE_SIZE));
                bg.setFillColor(Color(50, 50, 50, 220));
                window.draw(bg);

                string pieceName = string(1, promotionColor) + options[i];
                if(textures.count(pieceName)){
                    Sprite sprite(textures[pieceName]);
                    auto size = textures[pieceName].getSize();
                    sprite.setScale(Vector2f(
                        (float)TILE_SIZE / size.x,
                        (float)TILE_SIZE / size.y
                    ));
                    sprite.setPosition(Vector2f(promotionCol * TILE_SIZE, i * TILE_SIZE));
                    window.draw(sprite);
                }
            }
        }
        if(gameOver){

            RectangleShape overlay(Vector2f(800, 80));

            overlay.setPosition(Vector2f(0, 360));

            overlay.setFillColor(Color(0,0,0,220));

            window.draw(overlay);

            Text endText(font);

            endText.setString(gameResult);

            endText.setCharacterSize(40);

            endText.setFillColor(Color::Yellow);

            endText.setStyle(Text::Bold);

            auto bounds = endText.getLocalBounds();

            endText.setOrigin(bounds.size / 2.f);

            endText.setPosition(Vector2f(400,400));

            window.draw(endText);
        }

        // --- Sidebar UI ---
        RectangleShape sidebar(Vector2f(300, 800));
        sidebar.setPosition(Vector2f(800, 0));
        sidebar.setFillColor(Color(40, 40, 45));
        window.draw(sidebar);

        auto formatTime = [](float t) {
            int mins = max(0, (int)t / 60);
            int secs = max(0, (int)t % 60);
            char buf[10];
            snprintf(buf, sizeof(buf), "%02d:%02d", mins, secs);
            return string(buf);
        };

        Text wTimeText(font);
        wTimeText.setString("White Time:\n  " + formatTime(whiteTime));
        wTimeText.setCharacterSize(30);
        wTimeText.setFillColor(Color::White);
        wTimeText.setPosition(Vector2f(820, 600));
        window.draw(wTimeText);

        Text bTimeText(font);
        bTimeText.setString("Black Time:\n  " + formatTime(blackTime));
        bTimeText.setCharacterSize(30);
        bTimeText.setFillColor(Color::White);
        bTimeText.setPosition(Vector2f(820, 100));
        window.draw(bTimeText);

        if (isAiThinking) {
            Text aiText(font);
            aiText.setString("AI is thinking...");
            aiText.setCharacterSize(24);
            aiText.setFillColor(Color::Yellow);
            aiText.setPosition(Vector2f(820, 350));
            window.draw(aiText);
        }

        window.display();
    }
    return 0;
}