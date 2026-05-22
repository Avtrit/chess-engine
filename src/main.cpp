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
    else if(piece == "wK" || piece == "bK"){
        KingMoves(moves, row, col, myColor);
    }
    return moves;
}

bool isSquareAttacked(int row,int col, char attackerColor){
    vector<pair<int,int>> moves;
    for(int i = 0 ; i<8; i++){
        for(int j = 0 ; j<8 ; j++){
            if(board[i][j][0] == attackerColor){
                string piece = board[i][j];
                //if attacking piece is a pawn
                if(board[i][j]=="wP"){
                    if((i == (row + 1)) && ((j == (col - 1))||(j == (col + 1)))){
                        return true;
                    }
                }
                if(board[i][j]=="bP"){
                    if((i == (row - 1)) && ((j == (col - 1) )||(j == (col + 1)))){
                        return true;
                    }
                }
                //othercases
                if(piece == "wR" || piece == "bR"){
                    RookMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wB" || piece == "bB"){
                    BishopMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wN" || piece == "bN"){
                    KnightMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wQ" || piece == "bQ"){
                    RookMoves(moves, i, j, attackerColor);
                    BishopMoves(moves, i, j, attackerColor);
                }
                else if(piece == "wK" || piece == "bK"){
                    KingMoves(moves, i, j, attackerColor);
                }
            }
        }
    }
    for(auto&p : moves){
        if((row == p.first) && (col== p.second)){
            return true;
        }
    }
    return false;
}

pair<int,int> findKing(char color){
    for(int i= 0 ; i<8 ; i++){
        for(int j = 0 ; j< 8;j++){
            if(board[i][j][0]==color && board[i][j][1]=='K'){
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

int main(){
    RenderWindow window(VideoMode({800, 800}), "Chess Engine");//window created 
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

    Font font;
    if(!font.openFromFile("C:\\Chess Engine\\src\\arial.ttf")){
    //could not load
    };

    // promotion variables
    bool promoting = false;
    int promotionRow = -1, promotionCol = -1;
    char promotionColor = ' ';

    //loading cursor types 
    auto defaultCursor = Cursor::createFromSystem(Cursor::Type::Arrow);
    auto grabCursor = Cursor::createFromSystem(Cursor::Type::Hand);

    while(window.isOpen()){
        while(const optional event = window.pollEvent()){
            if(event->is<Event::Closed>()){
                window.close();
            }
            else if(event->is<Event::MouseButtonPressed>()){
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

                        if(isKinginCheck(currentTurn)){
                            // undo the move
                            board[selectedRow][selectedCol] = movedPiece;
                            board[row][col] = capturedPiece;
                        }
                        else{
                            currentTurn = (currentTurn == 'w') ? 'b' : 'w';
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
        window.display();
    }
    return 0;
}