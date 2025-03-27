#pragma once
#include "wallkickdata.h"
#include "piecesdata.h"

Piecesdata pieces = init_tetrominoes();
KickData kickdata_Ipiece = init_walkkickdata_Ipiece();
KickData kickdata_therest = init_walkkickdata_therest(); 

class Tetromino{
    int type;
    int color;
    Piece piece;

    bool checkCollision(int newx, int newy, vector<vector<int>> image, vector<vector<int>> board){
        for (int i=0; i<image.size(); i++){
            for (int j=0; j<image[i].size(); j++){
                if (image[i][j] != 0){
                    if (newx+j < 0 || newx+j >= board[0].size() || newy+i >= board.size() || newy+i <0){
                        return true;
                    }
                    if (board[newy+i][newx+j] != 0){
                        return true;
                    }
                }
            }
        }
        return false;
    }

public:
    int rotation;
    int x;
    int y;
    Tetromino(){}
    Tetromino(int type, int rotation, int x, int y){
        this->type = type;
        this->rotation = rotation;
        this->x = x;
        this->y = y;
        this->piece = pieces[type];
    }

    int getType(){return type;}
    int getRotation(){return rotation;}
    void setRotation(int rotation){this->rotation = rotation;}
    int getX(){return x;}
    int getY(){return y;}
    int getColor(){return color;}
    vector<vector<int>> getImage(){return piece[rotation];}

    bool rotate(int direction, vector<vector<int>> board){
        int i;
        int index = rotation;
        if (direction > 0){
            i = 0;
            index = rotation;
        }
        else{
            i = 1;
            index = (rotation+direction);
            if (index < 0){
                index = 3;
            }
        }
        int wkd1;
        int wkd2;
        int xd = (rotation+direction)%4;
        if (xd<0){
            xd = 3;
        }
        if (type == 0){
            for(int j= 0; j<5;j++){
                wkd1 = get<0>(kickdata_Ipiece[index][i][j]);
                wkd2 = get<1>(kickdata_Ipiece[index][i][j]);

                if (!checkCollision(x+wkd1, y+wkd2, piece[xd], board)){
                    rotation = (rotation+direction)%4;
                    if (rotation < 0){
                        rotation = 3;
                    }
                    x += wkd1;
                    y += wkd2;
                    return true;
                }
            }
        }
        else{
            for(int j= 0; j < 5; j++){
                wkd1 = get<0>(kickdata_therest[index][i][j]);
                wkd2 = get<1>(kickdata_therest[index][i][j]);
                if (!checkCollision(x+wkd1, y+wkd2, piece[xd], board)){
                    rotation = (rotation+direction)%4;
                    if (rotation < 0){
                        rotation = 3;
                    }
                    x += wkd1;
                    y += wkd2;
                    return true;
                }
            }
        }
        return false;
    }
};