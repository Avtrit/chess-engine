#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
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
void addRookMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
    for(int r = row-1; r >= 0; r--){
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
    for(int r = row+1; r < 8; r++){
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
    for(int c = col-1; c >= 0; c--){
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
    for(int c = col+1; c < 8; c++){
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

void addBishopMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
    for(int i = 1; row-i >= 0 && col-i >= 0; i++){
        if(board[row-i][col-i] == ""){
            moves.push_back({row-i, col-i});
        }
        else if(board[row-i][col-i][0] != myColor){
            moves.push_back({row-i, col-i});
            break;
        }
        else{
            break;
        }
    }
    for(int i = 1; row-i >= 0 && col+i < 8; i++){
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
    for(int i = 1; row+i < 8 && col-i >= 0; i++){
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
    for(int i = 1; row+i < 8 && col+i < 8; i++){
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

void addKnightMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
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

void addKingMoves(vector<pair<int,int>>& moves, int row, int col, char myColor){
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
    }
    else if(piece == "wR" || piece == "bR"){
        addRookMoves(moves, row, col, myColor);
    }
    else if(piece == "wB" || piece == "bB"){
        addBishopMoves(moves, row, col, myColor);
    }
    else if(piece == "wN" || piece == "bN"){
        addKnightMoves(moves, row, col, myColor);
    }
    else if(piece == "wQ" || piece == "bQ"){
        addRookMoves(moves, row, col, myColor);
        addBishopMoves(moves, row, col, myColor);
    }
    else if(piece == "wK" || piece == "bK"){
        addKingMoves(moves, row, col, myColor);
    }
    return moves;
}

int main(){
    RenderWindow window(VideoMode({800, 800}), "Chess Engine");
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
    while(window.isOpen()){
        while(const optional event = window.pollEvent()){
            if(event->is<Event::Closed>()){
                window.close();
            }
             else if(event->is<Event::MouseButtonPressed>()){
                auto mouse = Mouse::getPosition(window);
                int col = mouse.x / TILE_SIZE;
                int row = mouse.y / TILE_SIZE;
                if(!selected){
                    if(board[row][col] != "" && board[row][col][0] == currentTurn){ // add this check
                        selected = true;
                        selectedRow = row;
                        selectedCol = col;
                        validMoves = getValidMoves(row, col);
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
                        board[row][col] = board[selectedRow][selectedCol];
                        board[selectedRow][selectedCol] = "";
                        currentTurn = (currentTurn == 'w') ? 'b' : 'w'; // switch turn
                    }

                    selected = false;
                    validMoves.clear();
                }
            }
        }
        window.clear();
        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                RectangleShape tile(Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                if((row + col) % 2 == 0){
                    tile.setFillColor(Color(240, 217, 181));
                }
                else{
                    tile.setFillColor(Color(181, 136, 99));
                }
                window.draw(tile);
                if(selected && row == selectedRow && col == selectedCol){
                    RectangleShape highlight(Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                    highlight.setFillColor(Color(255, 255, 0, 100));
                    window.draw(highlight);
                }
                for(auto& [r, c] : validMoves){
                    if(row == r && col == c){
                        RectangleShape highlight(Vector2f(TILE_SIZE, TILE_SIZE));
                        highlight.setPosition(Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                        highlight.setFillColor(Color(0, 255, 0, 100));
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
        window.display();
    }
    return 0;
}